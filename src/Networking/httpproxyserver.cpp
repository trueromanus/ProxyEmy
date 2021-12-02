#include "httpproxyserver.h"
#include <QtConcurrent>
#include <QFutureWatcher>
#include <QFuture>
#include <QByteArray>

HttpProxyServer::HttpProxyServer(QObject *parent) : QTcpServer(parent)
{
}

void HttpProxyServer::setConfiguration(ConfigurationViewModel *configuration) noexcept
{
    if (m_configuration == configuration) return;

    m_configuration = configuration;
    emit configurationChanged();
}

void HttpProxyServer::startServer()
{
    if (m_configuration->port() <= 0) {
        qInfo() << "Port not specified";
        return;
    }
    auto port = m_configuration->port();
    if (!listen(QHostAddress::Any, port)) {
        qInfo() << "Error while trying start listening on port " << port;
        return;
    }

    qInfo() << "Server started listening on port " << port;
}

void HttpProxyServer::stopServer()
{
    if (!isListening()) return;

    close();
}

void HttpProxyServer::incomingConnection(qintptr socketDescriptor)
{
    QFuture<QString> data = QtConcurrent::run(&HttpProxyServer::processSocket, this, socketDescriptor);
    QFutureWatcher<QString> dataWatcher(this);
    connect(
        &dataWatcher,
        &QFutureWatcher<QString>::finished,
        this,
        [data] {
            auto output = data.result();

            qDebug() << " output " << output;

        }
    );
    dataWatcher.setFuture(data);
}

QString HttpProxyServer::processSocket(int socket)
{
    QTcpSocket tcpSocket;
    if (!tcpSocket.setSocketDescriptor(socket)) {
        //TODO: added error
        //tcpSocket.error()
        return "";
    }

    tcpSocket.waitForReadyRead();
    auto firstLine = tcpSocket.readLine(3500);
    auto request = tcpSocket.readAll();

    auto parts = firstLine.split(' ');
    auto currentRoute = parts[1];

    auto mapping = m_configuration->getMappingByRoute(currentRoute);
    if (mapping == nullptr) {
        //TODO: return empty result
        return "";
    }

    QTcpSocket innerTcpSocket;
    innerTcpSocket.connectToHost(mapping->getExternalHost(), mapping->getExternalPort());
    if (!innerTcpSocket.waitForConnected()) {
        qDebug() << "Failed connect" << innerTcpSocket.errorString();
        return "";
    }
    parts[1] = mapping->mapLocalToExternal(currentRoute).toUtf8();
    innerTcpSocket.write(parts.join(' '));
    innerTcpSocket.write(request);
    innerTcpSocket.waitForReadyRead();
    auto response = innerTcpSocket.readAll();

    if (response.contains("Accept-Ranges: bytes")) {
        while (true) {
            innerTcpSocket.waitForReadyRead(10);
            auto bytesCount = innerTcpSocket.bytesAvailable();
            if (bytesCount == 0 || innerTcpSocket.atEnd()) break;

            auto bytes = innerTcpSocket.read(bytesCount);
            response.append(bytes);
        }
    }

    tcpSocket.write(response);
    tcpSocket.waitForBytesWritten(2000);

    tcpSocket.disconnectFromHost();

    return "";
}

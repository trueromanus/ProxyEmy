#include "httpproxyserver.h"
#include <QtConcurrent>
#include <QFutureWatcher>
#include <QFuture>
#include <QByteArray>
#include <QTextStream>

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
    QFuture data = QtConcurrent::run(&HttpProxyServer::processSocket, this, socketDescriptor);
    /*QFutureWatcher<QString> dataWatcher(this);
    connect(
        &dataWatcher,
        &QFutureWatcher<QString>::finished,
        this,
        [data] {
            auto output = data.result();

            qDebug() << " output " << output;

        }
    );
    dataWatcher.setFuture(data);*/
}

void HttpProxyServer::processSocket(int socket)
{
    qDebug() << "incoming connection " << socket;
    QTcpSocket tcpSocket;
    if (!tcpSocket.setSocketDescriptor(socket)) {
        qWarning() << "Error while try read socket descriptor: " << tcpSocket.errorString();
        return;
    }

    tcpSocket.waitForReadyRead();
    auto firstLine = tcpSocket.readLine(3500);
    qDebug() << firstLine;
    auto request = tcpSocket.readAll();

    auto parts = firstLine.split(' ');
    if (parts.count() != 3) {
        qWarning() << "Error while try read request first line from socket " << socket;
        closeSocket(tcpSocket);
        return;
    }
    auto currentRoute = parts[1];

    auto mapping = m_configuration->getMappingByRoute(currentRoute);
    if (mapping == nullptr) {
        tcpSocket.write(m_EmptyResponse.toUtf8());
        closeSocket(tcpSocket);
        return;
    }

    QTcpSocket* innerTcpSocket = createSocket(*mapping);
    if (innerTcpSocket == nullptr) return;

    parts[1] = mapping->mapLocalToExternal(currentRoute).toUtf8();

    innerTcpSocket->write(parts.join(' '));
    QTextStream requestData(request);
    QString currentLine;
    while (requestData.readLineInto(&currentLine)) {
        if (currentLine.startsWith("Host: ")) {
            auto mappedHost = "Host: " + mapping->getExternalHost() + ":" + QString::number(mapping->getExternalPort()) + "\r\n";
            innerTcpSocket->write(mappedHost.toUtf8());
        } else {
            auto line = currentLine + "\r\n";
            innerTcpSocket->write(line.toUtf8());
        }
    }

    innerTcpSocket->waitForReadyRead();
    auto response = innerTcpSocket->readAll();

    if (response.contains("Accept-Ranges: bytes")) {
        while (true) {
            innerTcpSocket->waitForReadyRead(1000);
            auto bytesCount = innerTcpSocket->bytesAvailable();
            if (bytesCount == 0 || innerTcpSocket->atEnd()) break;

            auto bytes = innerTcpSocket->read(bytesCount);
            response.append(bytes);
        }
    }

    innerTcpSocket->disconnectFromHost();

    tcpSocket.write(response);
    closeSocket(tcpSocket);

    return;
}

void HttpProxyServer::closeSocket(QTcpSocket &socket)
{
    socket.waitForBytesWritten(2000);
    socket.disconnectFromHost();
}

QTcpSocket* HttpProxyServer::createSocket(const RouteMapping &mapping)
{
    auto isSecure = mapping.isExternalSecure();
    if (isSecure) {
        auto socket = new QSslSocket();
        socket->connectToHostEncrypted(mapping.getExternalHost(), mapping.getExternalPort());
        if (!socket->waitForEncrypted(2000)) {
            qWarning() << "Error while TLS handshake " << mapping.getExternalHost() << mapping.getExternalPort() << " " << socket->errorString();
            return nullptr;
        }
        return socket;
    } else {
        auto socket = new QTcpSocket();
        socket->connectToHost(mapping.getExternalHost(), mapping.getExternalPort());
        if (!socket->waitForConnected()) {
            qWarning() << "Failed connect" << mapping.getExternalHost() << mapping.getExternalPort() << " " << socket->errorString();
            return nullptr;
        }
        return socket;
    }

    return nullptr;
}

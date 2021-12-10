/*
    ProxyEmy local proxy server
    Copyright (C) 2022 Roman Vladimirov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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
    emit serverStartedChanged();
}

void HttpProxyServer::stopServer()
{
    if (!isListening()) return;

    close();
    emit serverStartedChanged();
}

void HttpProxyServer::restartServer()
{
    stopServer();
    startServer();
}

void HttpProxyServer::incomingConnection(qintptr socketDescriptor)
{
    auto future = QtConcurrent::run(&HttpProxyServer::processSocket, this, socketDescriptor);
}

void HttpProxyServer::processSocket(int socket)
{
    qDebug() << "incoming connection " << socket;
    QTcpSocket tcpSocket(this);
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

    while (true) {
        innerTcpSocket->waitForReadyRead(1000);
        auto bytesCount = innerTcpSocket->bytesAvailable();
        if (bytesCount == 0 || innerTcpSocket->atEnd()) break;

        auto bytes = innerTcpSocket->read(bytesCount);
        tcpSocket.write(bytes);
        tcpSocket.waitForBytesWritten();
    }

    tcpSocket.waitForBytesWritten(1000);

    innerTcpSocket->disconnectFromHost();
    innerTcpSocket->deleteLater();

    closeSocket(tcpSocket);
    tcpSocket.deleteLater();

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
        auto socket = new QSslSocket(this);
        socket->connectToHostEncrypted(mapping.getExternalHost(), mapping.getExternalPort());
        if (!socket->waitForEncrypted(2000)) {
            qWarning() << "Error while TLS handshake " << mapping.getExternalHost() << mapping.getExternalPort() << " " << socket->errorString();
            return nullptr;
        }
        return socket;
    } else {
        auto socket = new QTcpSocket(this);
        socket->connectToHost(mapping.getExternalHost(), mapping.getExternalPort());
        if (!socket->waitForConnected()) {
            qWarning() << "Failed connect" << mapping.getExternalHost() << mapping.getExternalPort() << " " << socket->errorString();
            return nullptr;
        }
        return socket;
    }

    return nullptr;
}

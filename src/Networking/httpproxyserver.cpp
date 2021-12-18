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
#include <QCoreApplication>

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
    QTcpSocket* tcpSocket = createSocketFromDescription(socket);
    if (tcpSocket == nullptr) return;

    RouteMapping *currentRoute = nullptr;
    QTcpSocket* innerTcpSocket = nullptr;

    while (true) {
        tcpSocket->waitForReadyRead(1000);
        auto bytesCount = tcpSocket->bytesAvailable();
        if (bytesCount == 0 || tcpSocket->atEnd()) break;

        auto bytes = tcpSocket->read(bytesCount);

        if (currentRoute == nullptr) {
            auto route = getRoute(bytes);
            if (route.isEmpty()) {
                qWarning() << "Error while try read request first line from socket " << socket;
                closeSocket(tcpSocket);
                break;
            }
            auto currentRoute = m_configuration->getMappingByRoute(route);
            if (currentRoute == nullptr) {
                tcpSocket->write(m_EmptyResponse.toUtf8());
                closeSocket(tcpSocket);
                break;
            }
            innerTcpSocket = createSocket(*currentRoute);
            if (innerTcpSocket == nullptr) {
                closeSocket(tcpSocket);
                break;
            }
            innerTcpSocket->write(replaceHost(bytes, currentRoute));
            innerTcpSocket->waitForBytesWritten(2000);
            break;
        }

        innerTcpSocket->write(bytes);
        innerTcpSocket->waitForBytesWritten(2000);
    }

    if (innerTcpSocket == nullptr) {
        qWarning() << "Error while try read request headers from socket " << socket;
        return;
    }

    while (true) {
        innerTcpSocket->waitForReadyRead(1000);
        auto bytesCount = innerTcpSocket->bytesAvailable();
        if (bytesCount == 0 || innerTcpSocket->atEnd()) break;

        auto bytes = innerTcpSocket->read(bytesCount);
        tcpSocket->write(bytes);
        tcpSocket->waitForBytesWritten();
    }

    tcpSocket->waitForBytesWritten(1000);

    innerTcpSocket->disconnectFromHost();
    delete innerTcpSocket;

    closeSocket(tcpSocket);

    return;
}

void HttpProxyServer::closeSocket(QTcpSocket* socket)
{
    socket->waitForBytesWritten(2000);
    socket->disconnectFromHost();
    delete socket;
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

bool HttpProxyServer::setupSocket(QTcpSocket &socket, const int socketDescriptor)
{
    if (!socket.setSocketDescriptor(socketDescriptor)) {
        qWarning() << "Error while try read socket descriptor: " << socket.errorString();
        return false;
    }

    return true;
}

bool HttpProxyServer::setupSslSocket(QSslSocket &socket, const int socketDescriptor)
{
    if (!socket.setSocketDescriptor(socketDescriptor)) {
        qWarning() << "Error while try read socket descriptor: " << socket.errorString();
        return false;
    }

    auto applicationPath = QCoreApplication::applicationDirPath();

    socket.setLocalCertificate(applicationPath + "/certificate/server.crt", QSsl::Pem);
    socket.setPrivateKey(applicationPath + "/certificate/server.key", QSsl::Rsa, QSsl::Pem);
    socket.setPeerVerifyMode(QSslSocket::VerifyNone);
    socket.setProtocol(QSsl::TlsV1_0OrLater);
    socket.startServerEncryption();
    return socket.waitForEncrypted(2000);
}

QTcpSocket *HttpProxyServer::createSocketFromDescription(const int socket)
{
    if (!m_configuration->isSecure()) {
        auto tcpSocket = new QTcpSocket();
        if (setupSocket(*tcpSocket, socket)) {
            return tcpSocket;
        }
        return nullptr;
    }

    auto tcpSocket = new QSslSocket();
    if (setupSslSocket(*tcpSocket, socket)) {
        return tcpSocket;
    }
    return nullptr;
}

QByteArray HttpProxyServer::getRoute(QByteArray bytes)
{
    auto index = bytes.indexOf("\r\n");
    auto routeHeader = bytes.mid(0, index);
    auto parts = routeHeader.split(' ');
    if (parts.count() != 3) return "";

    return parts[1];
}

QByteArray HttpProxyServer::replaceHost(QByteArray bytes, RouteMapping *mapping)
{
    auto index = bytes.indexOf("Host: ");
    if (index == -1) return bytes;

    auto endIndex = bytes.indexOf("\r\n", index);

    auto leftPart = bytes.mid(0, index);
    auto rightPart = bytes.mid(endIndex + 2);
    auto host = "Host: " + mapping->getExternalHost() + ":" + QString::number(mapping->getExternalPort()) + "\r\n";
    if (leftPart.contains("site-middle.jpg")) {
        qDebug() << rightPart;
    }
    return leftPart + host.toUtf8() + rightPart;
}

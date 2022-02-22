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
#include <QSslError>

HttpProxyServer::HttpProxyServer(QObject *parent) : QTcpServer(parent)
{
}

void HttpProxyServer::setConfiguration(ConfigurationViewModel *configuration) noexcept
{
    if (m_configuration == configuration) return;

    m_configuration = configuration;
    emit configurationChanged();
}

void HttpProxyServer::setNotificationhub(const NotificationHubViewModel *notificationhub) noexcept
{
    if (m_notificationHub != nullptr) return; // can set notification hub only one time
    if (m_notificationHub == notificationhub) return;

    m_notificationHub = const_cast<NotificationHubViewModel*>(notificationhub);
}

void HttpProxyServer::setRequestslog(const RequestsLogViewModel *requestslog) noexcept
{
    if (m_requestslog != nullptr) return; // can set requests log only one time
    if (m_requestslog == requestslog) return;

    m_requestslog = const_cast<RequestsLogViewModel*>(requestslog);
}

void HttpProxyServer::startServer()
{
    if (m_configuration->port() <= 0) {
        qInfo() << m_portNotSpecifiedMessage;
        m_notificationHub->pushErrorMessage(m_mainMessageTitle, m_portNotSpecifiedMessage);
        return;
    }
    auto port = m_configuration->port();
    if (!listen(QHostAddress::Any, port)) {
        auto message = m_errorWhileStartListening + QString::number(port);
        qInfo() << message;
        m_notificationHub->pushErrorMessage(m_mainMessageTitle, message);
        return;
    }

    auto listeningMessage = m_serverStartListening + QString::number(port);
    qInfo() << listeningMessage;
    m_notificationHub->pushInfoMessage(m_mainMessageTitle, listeningMessage);
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

    int64_t totalReaded = 0;

    bool webSocketRequest = false;

    while (true) {
        tcpSocket->waitForReadyRead(1000);
        auto bytesCount = tcpSocket->bytesAvailable();
        totalReaded += bytesCount;
        if (bytesCount == 0 || tcpSocket->atEnd()) {
            if (totalReaded == 0) closeSocket(tcpSocket); // if total read bytes was zero I don't think we can make something
            break;
        }

        auto bytes = tcpSocket->read(bytesCount);

        if (currentRoute == nullptr) {
            auto routeData = getRoute(bytes);
            if (routeData.count() != 3) {
                qWarning() << "Error while try read request first line from socket " << socket;
                closeSocket(tcpSocket);
                break;
            }

            webSocketRequest = isWebSocket(bytes);

            auto route = routeData[1];
            auto currentRoute = m_configuration->getMappingByRoute(route);
            if (currentRoute == nullptr) {
                tcpSocket->write(m_EmptyResponse.toUtf8());
                tcpSocket->waitForBytesWritten(1000);
                closeSocket(tcpSocket);
                break;
            }

            //add record to log if option is enabled
            if (m_configuration->isLogRequests()) m_requestslog->addRecord(route, routeData[2], routeData[0], currentRoute->externalRouteOriginal());

            innerTcpSocket = createSocket(*currentRoute);
            if (innerTcpSocket == nullptr) {
                tcpSocket->write(m_NoContentResponse.toUtf8());
                tcpSocket->waitForBytesWritten(1000);
                closeSocket(tcpSocket);
                break;
            }
            innerTcpSocket->write(replaceHost(bytes, currentRoute, routeData));
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

    bool isAcceptBytes = false;
    bool isAcceptBytesCheck = false;

    if (webSocketRequest) {
        while (true) {
            // if any from connections already closed we breaking connection
            if (innerTcpSocket->state() == QAbstractSocket::UnconnectedState || innerTcpSocket->state() == QAbstractSocket::ClosingState) break;
            if (tcpSocket->state() == QAbstractSocket::UnconnectedState || tcpSocket->state() == QAbstractSocket::ClosingState) break;

            //handle inner socket
            auto innerBytes = readAllAvailableBytesFromSocket(innerTcpSocket, 100);
            if (innerBytes.count() > 0) {
                tcpSocket->write(innerBytes);
                tcpSocket->waitForBytesWritten(200);
            }

            //handle outer socket
            auto outerBytes = readAllAvailableBytesFromSocket(tcpSocket, 100);
            if (outerBytes.count() > 0) {
                innerTcpSocket->write(outerBytes);
                innerTcpSocket->waitForBytesWritten(200);
            }
        }
    } else {
        while (true) {
            innerTcpSocket->waitForReadyRead(1000);
            auto bytesCount = innerTcpSocket->bytesAvailable();
            if (bytesCount == 0 || innerTcpSocket->atEnd()) {
                if (!isAcceptBytes) break;

                auto bytes = readAllAvailableBytesFromSocket(tcpSocket);
                if (bytes.length() == 0) break;

                innerTcpSocket->write(bytes);
                innerTcpSocket->waitForBytesWritten(2000);
                continue;
            }

            auto bytes = innerTcpSocket->read(bytesCount);

            if (!isAcceptBytesCheck) {
                isAcceptBytesCheck = true;
                isAcceptBytes = isAcceptRanges(bytes);
            }

            tcpSocket->write(bytes);
            tcpSocket->waitForBytesWritten();
        }
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

        if (m_configuration->needVerifyCertificates()) {
            socket->setPeerVerifyMode(QSslSocket::VerifyPeer);
        } else {
            socket->setPeerVerifyMode(QSslSocket::QueryPeer);
        }

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

    socket.setLocalCertificate(applicationPath + "/certificate/localhost.crt", QSsl::Pem);
    socket.setPrivateKey(applicationPath + "/certificate/localhost.key", QSsl::Rsa, QSsl::Pem);
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

QList<QByteArray> HttpProxyServer::getRoute(QByteArray bytes)
{
    auto index = bytes.indexOf("\r\n");
    auto routeHeader = bytes.mid(0, index);

    return routeHeader.split(' ');
}

bool HttpProxyServer::isWebSocket(QByteArray bytes)
{
    if (bytes.indexOf("Upgrade: websocket") > -1) return true;
    if (bytes.indexOf("Upgrade: WebSocket") > -1) return true;

    return false;
}

bool HttpProxyServer::isAcceptRanges(QByteArray bytes)
{
    return bytes.indexOf("Accept-Ranges: bytes") > -1;
}

QByteArray HttpProxyServer::readAllAvailableBytesFromSocket(QTcpSocket * socket, int delay)
{
    QByteArray result;

    while (true) {
        socket->waitForReadyRead(delay);
        auto bytesCount = socket->bytesAvailable();
        if (bytesCount == 0 || socket->atEnd()) return result;

        result.append(socket->read(bytesCount));
    }

    return result;
}

QByteArray HttpProxyServer::replaceHost(QByteArray bytes, RouteMapping *mapping, QList<QByteArray>& routeData)
{
    auto length = routeData[0].length() + routeData[1].length() + 1;
    auto urlPath = mapping->processExternalUrlPath();
    if (!urlPath.isEmpty()) {
        auto route = routeData[0] + " " + routeData.value(1).replace(0, 1, urlPath.toUtf8());
        bytes.replace(0, length, route);
    }

    auto index = bytes.indexOf("Host: ");
    if (index == -1) return bytes;

    auto endIndex = bytes.indexOf("\r\n", index);

    auto leftPart = bytes.mid(0, index);
    auto rightPart = bytes.mid(endIndex + 2);
    auto host = "Host: " + mapping->getExternalHost() + ":" + QString::number(mapping->getExternalPort()) + "\r\n";
    return leftPart + host.toUtf8() + rightPart;
}

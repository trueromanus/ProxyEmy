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


#ifndef HTTPPROXYSERVER_H
#define HTTPPROXYSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSslSocket>
#include <QList>
#include "../ViewModels/configurationviewmodel.h"

class HttpProxyServer : public QTcpServer
{
    Q_OBJECT
    Q_PROPERTY(ConfigurationViewModel* configuration READ configuration WRITE setConfiguration NOTIFY configurationChanged)
    Q_PROPERTY(bool serverStarted READ serverStarted NOTIFY serverStartedChanged)

private:
    const QString m_EmptyResponse = "HTTP/1.1 404 Not Found\r\nServer: ProxyEmy\r\n\r\n<b>404 Not Found</b>";
    ConfigurationViewModel* m_configuration { nullptr };

protected:
    void incomingConnection(qintptr socketDescriptor) override;

public:
    explicit HttpProxyServer(QObject *parent = nullptr);

    ConfigurationViewModel* configuration() const noexcept { return m_configuration; }
    void setConfiguration(ConfigurationViewModel* configuration) noexcept;

    bool serverStarted() const noexcept { return isListening(); }

    Q_INVOKABLE void startServer();
    Q_INVOKABLE void stopServer();
    Q_INVOKABLE void restartServer();

private:
    void processSocket(int socket);
    void closeSocket(QTcpSocket& socket);
    QTcpSocket* createSocket(const RouteMapping& mapping);

signals:
    void configurationChanged();
    void serverStartedChanged();

};

#endif // HTTPPROXYSERVER_H

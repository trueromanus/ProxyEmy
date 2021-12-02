#ifndef HTTPPROXYSERVER_H
#define HTTPPROXYSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include "../ViewModels/configurationviewmodel.h"

class HttpProxyServer : public QTcpServer
{
    Q_OBJECT
    Q_PROPERTY(ConfigurationViewModel* configuration READ configuration WRITE setConfiguration NOTIFY configurationChanged)

private:
    ConfigurationViewModel* m_configuration { nullptr };

protected:
    void incomingConnection(qintptr socketDescriptor) override;

public:
    explicit HttpProxyServer(QObject *parent = nullptr);

    ConfigurationViewModel* configuration() const noexcept { return m_configuration; }
    void setConfiguration(ConfigurationViewModel* configuration) noexcept;

    Q_INVOKABLE void startServer();
    Q_INVOKABLE void stopServer();

private:
    QString processSocket(int socket);

signals:
    void configurationChanged();

};

#endif // HTTPPROXYSERVER_H

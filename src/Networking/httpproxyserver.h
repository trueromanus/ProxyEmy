#ifndef HTTPPROXYSERVER_H
#define HTTPPROXYSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class HttpProxyServer : public QTcpServer
{
    Q_OBJECT

protected:
    void incomingConnection(qintptr socketDescriptor) override;

public:
    explicit HttpProxyServer(QObject *parent = nullptr);

    Q_INVOKABLE void startServer();
    Q_INVOKABLE void stopServer();

private:
    QString processSocket(int socket);


signals:

};

#endif // HTTPPROXYSERVER_H

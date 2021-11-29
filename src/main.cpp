#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "proxyemybackend.h"
#include "Networking/httpproxyserver.h"
#include "ViewModels/configurationviewmodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<ProxyEmyBackend>("ProxyEmy.Backend", 1, 0, "ProxyEmyBackend");
    qmlRegisterType<HttpProxyServer>("ProxyEmy.Backend", 1, 0, "HttpProxyServer");
    qmlRegisterType<ConfigurationViewModel>("ProxyEmy.Backend", 1, 0, "ConfigurationViewModel");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl) QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection
    );
    engine.load(url);

    return app.exec();
}

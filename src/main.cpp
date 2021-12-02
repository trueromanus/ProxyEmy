#include <QGuiApplication>
#include <QFile>
#include <QDateTime>
#include <QScopedPointer>
#include <QQmlApplicationEngine>
#include "proxyemybackend.h"
#include "Networking/httpproxyserver.h"
#include "ViewModels/configurationviewmodel.h"

QScopedPointer<QFile> LoggerFile;

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<ProxyEmyBackend>("ProxyEmy.Backend", 1, 0, "ProxyEmyBackend");
    qmlRegisterType<HttpProxyServer>("ProxyEmy.Backend", 1, 0, "HttpProxyServer");
    qmlRegisterType<ConfigurationViewModel>("ProxyEmy.Backend", 1, 0, "ConfigurationViewModel");

#ifdef RELEASE
    LoggerFile.reset(new QFile("session.log"));
    LoggerFile.data()->open(QFile::WriteOnly | QFile::Text);
    qInstallMessageHandler(messageHandler);
#endif

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

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if (context.category == nullptr) return;

    QTextStream out(LoggerFile.data());
    out << QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss]");
    switch (type)
    {
        case QtDebugMsg:
            out << "debug ";
            break;
        case QtWarningMsg:
            out << "warning ";
            break;
        case QtCriticalMsg:
            out << "critical ";
            break;
        case QtFatalMsg:
            out << "fatal ";
            break;
        case QtInfoMsg:
            out << " ";
            break;
    }

    out << msg << Qt::endl;
    out.flush();
}

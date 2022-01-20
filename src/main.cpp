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

#include <QGuiApplication>
#include <QFile>
#include <QDateTime>
#include <QScopedPointer>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include "proxyemybackend.h"
#include "Networking/httpproxyserver.h"
#include "ViewModels/configurationviewmodel.h"
#include "ViewModels/notificationhubviewmodel.h"
#include "ViewModels/requestslogviewmodel.h"
#include "ListModels/configurationaliaseslistmodel.h"
#include "ListModels/notificationpoollistmodel.h"
#include "ListModels/pagestabslistmodel.h"
#include "ListModels/requestlogslistmodel.h"

QScopedPointer<QFile> LoggerFile;

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

void registerQmlTypes();

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQuickStyle::setStyle("Universal");

    registerQmlTypes();

#ifdef QT_DEBUG
#else
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

void registerQmlTypes() {
    qmlRegisterType<ProxyEmyBackend>("ProxyEmy.Backend", 1, 0, "ProxyEmyBackend");
    qmlRegisterType<PagesTabsListModel>("ProxyEmy.Backend", 1, 0, "PagesTabsListModel");

    qmlRegisterType<HttpProxyServer>("ProxyEmy.Backend", 1, 0, "HttpProxyServer");

    qmlRegisterType<ConfigurationViewModel>("ProxyEmy.Backend", 1, 0, "ConfigurationViewModel");
    qmlRegisterType<ConfigurationMappingListModel>("ProxyEmy.Backend", 1, 0, "ConfigurationMappingListModel");
    qmlRegisterType<ConfigurationAliasesListModel>("ProxyEmy.Backend", 1, 0, "ConfigurationAliasesListModel");

    qmlRegisterType<NotificationHubViewModel>("ProxyEmy.Backend", 1, 0, "NotificationHubViewModel");
    qmlRegisterType<NotificationPoolListModel>("ProxyEmy.Backend", 1, 0, "NotificationPoolListModel");

    qmlRegisterType<RequestsLogViewModel>("ProxyEmy.Backend", 1, 0, "RequestsLogViewModel");
    qmlRegisterType<RequestLogsListModel>("ProxyEmy.Backend", 1, 0, "RequestLogsListModel");
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

#include "configurationviewmodel.h"
#include <QFile>
#include <QDebug>
#include <QCoreApplication>

ConfigurationViewModel::ConfigurationViewModel(QObject *parent) : QObject(parent)
{
    auto arguments = QCoreApplication::arguments();
    if (arguments.length() > 1) {
        auto yamlPath = arguments.value(1);
        readYaml(yamlPath);
    }
}

void ConfigurationViewModel::readYaml(const QString &path) noexcept
{
    if (!QFile::exists(path)) {
        qInfo() << "YAML file doesn't exists on path: " << path;
        return;
    }

    QFile configurationFile(path);
    if (!configurationFile.open(QIODevice::ReadOnly)) {
        qInfo() << "Can't read YAML file by path: " << path;
        return;
    }

    auto content = configurationFile.readAll();
    auto yamlRoot = YAML::Load(content.toStdString());

    const int port = yamlRoot["port"].as<int>(0);
    if (port == 0) {
        qInfo() << "YAML don't contain ot have incorrect value for `port` key" << path;
        return;
    }
}

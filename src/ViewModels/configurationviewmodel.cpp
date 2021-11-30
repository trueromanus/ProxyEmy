#include "configurationviewmodel.h"
#include <QFile>
#include <QDebug>
#include <QCoreApplication>
#include <QDir>

ConfigurationViewModel::ConfigurationViewModel(QObject *parent) : QObject(parent)
{
    auto arguments = QCoreApplication::arguments();
    if (arguments.length() > 1) {
        auto yamlPath = arguments.value(1);
        qInfo() << "YAML passed as parameter: " << yamlPath;
        readYaml(yamlPath);
        return;
    }

    auto folderYamlPath = "proxyemy.yml";
    if (QFile::exists(folderYamlPath)) {
        qInfo() << "YAML finded in current folder: " << QDir::currentPath();
        readYaml(folderYamlPath);
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
    try {
        auto yamlRoot = YAML::Load(content.toStdString());

        if (!readPort(yamlRoot)) return;
        if (!readAddresses(yamlRoot)) return;

    }  catch (const YAML::ParserException& e) {
        qInfo() << "Parse YAML file: " << QString::fromStdString(e.msg);
    }
}

bool ConfigurationViewModel::readPort(const YAML::Node &node) noexcept
{
    const int port = node["port"].as<int>(0);
    if (port == 0) {
        qInfo() << "YAML don't contain correct value for `port` key";
        return false;
    }

    return true;
}

bool ConfigurationViewModel::readAddresses(const YAML::Node &node) noexcept
{
    if (!node["addresses"]) {
        qInfo() << "YAML don't contain addresses sequence!";
        return false;
    }

    auto addressNode = node["addresses"];
    if (!addressNode.IsSequence()) {
        qInfo() << "YAML contain addresses tag but it not sequence!";
        return false;
    }

    auto it = addressNode.begin();
    while (it != addressNode.end())
    {
        auto scalarNode = *it;
        auto addressStdString = scalarNode.as<std::string>("");
        auto addressString = QString::fromStdString(addressStdString);
        auto parts = addressString.split(" ");
        if (parts.length() != 2) continue;

        m_addresses->insert(parts.first(), parts.last());
        ++it;
    }

    return !m_addresses->isEmpty();
}

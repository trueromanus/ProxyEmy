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

#include "configurationviewmodel.h"
#include <QFile>
#include <QDebug>
#include <QCoreApplication>
#include <QDir>
#include <QRegularExpression>
#include <QDesktopServices>
#include <QUrl>

ConfigurationViewModel::ConfigurationViewModel(QObject *parent) : QObject(parent)
{
    auto arguments = QCoreApplication::arguments();
    if (arguments.length() > 1) {
        auto yamlPath = arguments.value(1);
        qInfo() << "YAML passed as parameter: " << yamlPath;
        readYaml(yamlPath);
        setupYamlPath(yamlPath);
        return;
    }

    auto folderYamlPath = "proxyemy.yml";
    if (QFile::exists(folderYamlPath)) {
        qInfo() << "YAML finded in current folder: " << QDir::currentPath();
        readYaml(folderYamlPath);
        setupYamlPath(folderYamlPath);
    }

    setupRootMapping();
    m_configurationMappingListModel->setup(m_mappings);
}

RouteMapping *ConfigurationViewModel::getMappingByRoute(const QString &route)
{
    auto iterator = std::find_if(
        m_mappings->cbegin(),
        m_mappings->cend(),
        [route] (const RouteMapping* routeMapping) {
            return route.startsWith(routeMapping->localRoute());
        }
    );

    if (iterator == m_mappings->cend()) return m_rootMapping == nullptr ? nullptr : m_rootMapping;

    return *iterator;
}

void ConfigurationViewModel::openConfigFolder() const noexcept
{
#ifdef Q_OS_WIN
    auto url = "file:///" + m_pathToYaml;
#else
    auto url = "file://" + m_pathToYaml;
#endif
    QDesktopServices::openUrl(QUrl(url));
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
        if (!readMappings(yamlRoot)) return;

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

    m_port = port;

    return true;
}

bool ConfigurationViewModel::readAddresses(const YAML::Node &node) noexcept
{
    if (!node["aliases"]) {
        qInfo() << "YAML don't contain addresses sequence!";
        return true;
    }

    auto addressNode = node["aliases"];
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

bool ConfigurationViewModel::readMappings(const YAML::Node &node) noexcept
{
    if (!node["mappings"]) {
        qInfo() << "YAML don't contain mappings sequence!";
        return false;
    }

    auto mappingsNode = node["mappings"];
    if (!mappingsNode.IsSequence()) {
        qInfo() << "YAML contains mapping tags but it not sequence!";
        return false;
    }

    auto it = mappingsNode.begin();
    while (it != mappingsNode.end())
    {
        auto scalarNode = *it;
        auto mappingStdString = scalarNode.as<std::string>("");
        auto mappingString = QString::fromStdString(mappingStdString);
        auto parts = mappingString.split(" ");
        if (parts.length() != 3) continue;

        auto routeMapping = new RouteMapping();

        if (parts.value(0) == "http") routeMapping->setBindingType(1);
        if (parts.value(0) == "https") routeMapping->setBindingType(2);
        //TODO: make support WebSocket
        //if (parts.value(0) == "ws") routeMapping->setBindingType(3);
        //if (parts.value(0) == "wss") routeMapping->setBindingType(4);

        routeMapping->setLocalRoute(parts.value(1));

        routeMapping->setExternalRoute(processExternalRoute(parts.value(2)));

        m_mappings->append(routeMapping);

        ++it;
    }

    return !m_mappings->isEmpty();
}

QString ConfigurationViewModel::processExternalRoute(QString&& externalRoute) const noexcept
{
    QRegularExpression regexp("\\{[a-zA-Z0-9\\_\\-]{0,}\\}");
    auto iterator = regexp.globalMatch(externalRoute);
    while (iterator.hasNext()) {
        auto match = iterator.next();
        QString word = match.captured(0);
        auto address = "" + word;
        address.replace("{", "").replace("}", "");
        if (m_addresses->contains(address)) {
            externalRoute.replace(word, m_addresses->value(address));
        }
    }
    QString result = std::move(externalRoute);
    return result;
}

void ConfigurationViewModel::setupRootMapping() noexcept
{
    auto iterator = std::find_if(
        m_mappings->cbegin(),
        m_mappings->cend(),
        [] (const RouteMapping* routeMapping) {
            return routeMapping->localRoute() == "/";
        }
    );

    if (iterator == m_mappings->cend()) return;

    m_rootMapping = *iterator;
}

void ConfigurationViewModel::setupYamlPath(const QString& path) noexcept
{
    m_isConfigReaded = true;
    auto currentDir = QDir::current();
    m_pathToYaml = currentDir.absoluteFilePath(path);
}

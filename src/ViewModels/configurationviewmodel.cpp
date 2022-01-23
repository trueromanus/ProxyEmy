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
#include <QMapIterator>

ConfigurationViewModel::ConfigurationViewModel(QObject *parent) : QObject(parent)
{
    auto arguments = QCoreApplication::arguments();
    if (arguments.length() > 1) {
        auto yamlPath = arguments.value(1);
        qInfo() << "YAML passed as parameter: " << yamlPath;
        readYaml(yamlPath);
        setupYamlPath(yamlPath);
    } else if (QFile::exists(m_folderYamlPath)) {
        qInfo() << "YAML finded in current folder: " << QDir::currentPath();
        readYaml(m_folderYamlPath);
        setupYamlPath(m_folderYamlPath);
    }

    setupRootMapping();
    m_configurationMappingListModel->setup(m_mappings);
    m_aliasesListModel->setup(m_aliases);
}

void ConfigurationViewModel::setIsLogRequests(const bool isLogRequests) noexcept
{
    if (m_isLogRequests == isLogRequests) return;

    m_isLogRequests = isLogRequests;
    emit isLogRequestsChanged();
    markChanges();
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

void ConfigurationViewModel::markChanges() noexcept
{
    m_isHasChanges = true;
    emit isHasChangesChanged();
}

void ConfigurationViewModel::unmarkChanges() noexcept
{
    m_isHasChanges = false;
    emit isHasChangesChanged();
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

void ConfigurationViewModel::editMapping(const int id) noexcept
{
    auto iterator = std::find_if(
        m_mappings->cbegin(),
        m_mappings->cend(),
        [id] (const RouteMapping* routeMapping) {
            return routeMapping->id() == id;
        }
    );
    if (iterator == m_mappings->cend()) return;

    auto mapping = *iterator;
    auto values = m_configurationMappingListModel->getEditingValues(id);
    auto localRoute = std::get<0>(values);
    auto externalRoute = std::get<1>(values);

    if (!checkDuplicateRoute(mapping->id(), localRoute)) {
        emit errorMessage("This route already used in another mapping!", "Edit route mapping");
        return;
    }

    mapping->setLocalRoute(localRoute);
    mapping->setExternalRouteOriginal(externalRoute);
    auto originalRoute = externalRoute;
    mapping->setExternalRoute(processExternalRoute(std::move(originalRoute)));

    auto index = m_mappings->indexOf(mapping);
    m_configurationMappingListModel->disableEditing(index);
    markChanges();
    setupRootMapping();
}

void ConfigurationViewModel::addMapping(const QString &localRoute, const QString &externalRoute) noexcept
{
    if (!checkDuplicateRoute(-1, localRoute)) {
        emit errorMessage("This route already used in another mapping!", "Add route mapping");
        return;
    }

    m_lastIdentifier++;
    auto routeMapping = new RouteMapping();
    routeMapping->setId(m_lastIdentifier);
    routeMapping->setLocalRoute(localRoute);
    routeMapping->setExternalRouteOriginal(externalRoute);
    auto route = externalRoute;
    routeMapping->setExternalRoute(processExternalRoute(std::move(route)));

    m_mappings->append(routeMapping);

    m_configurationMappingListModel->refresh();
    markChanges();
    setupRootMapping();
}

void ConfigurationViewModel::deleteMapping(const int index) noexcept
{
    if (index >= m_mappings->count()) return;

    m_mappings->removeAt(index);

    m_configurationMappingListModel->refresh();
    markChanges();
    setupRootMapping();
}

void ConfigurationViewModel::editAlias(const QString &key) noexcept
{
    auto values = m_aliasesListModel->getEditingValues(key);
    auto alias = std::get<0>(values);
    auto aliasValue = std::get<1>(values);

    if (alias != key && m_aliases->contains(alias)) {
        emit errorMessage("This alias with new name already created!", "Edit alias");
        return;
    }

    if (alias != key) {

    } else {
        (*m_aliases)[key] = aliasValue;
    }

    m_aliasesListModel->disableEditing(key);
    refreshAllAfterAlias();
    markChanges();
}

void ConfigurationViewModel::addAlias(const QString &alias, const QString &value) noexcept
{
    if (m_aliases->contains(alias)) {
        emit errorMessage("This alias already created!", "Add alias");
        return;
    }

    m_aliases->insert(alias, value);
    m_aliasesListModel->refresh();
    refreshAllAfterAlias();
    markChanges();
}

void ConfigurationViewModel::deleteAlias(const QString &key) noexcept
{
    if (!m_aliases->contains(key)) return;

    m_aliases->remove(key);
    m_aliasesListModel->refresh();
    refreshAllAfterAlias();
    markChanges();
}

void ConfigurationViewModel::saveConfiguration(const bool saveOpened, const QString& path) noexcept
{
    auto savedPath = saveOpened ? m_pathToYaml : path;

    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "port";
    out << YAML::Value << m_port;
    out << YAML::Key << "secure";
    out << YAML::Value << m_isSecure;
    out << YAML::Key << "logrequests";
    out << YAML::Value << m_isLogRequests;

    out << YAML::Key << "aliases";
    out << YAML::Value << YAML::BeginSeq;
    QMapIterator aliasesIterator(*m_aliases);
    while(aliasesIterator.hasNext()) {
        aliasesIterator.next();
        out << (aliasesIterator.key() + " " + aliasesIterator.value()).toStdString();
    }
    out << YAML::EndSeq;

    out << YAML::Key << "mappings";
    out << YAML::Value << YAML::BeginSeq;
    foreach (auto mapping, *m_mappings) {
        out << (mapping->localRoute() + " " + mapping->externalRouteOriginal()).toStdString();
    }
    out << YAML::EndSeq;

    out << YAML::EndMap;

#ifdef Q_OS_WIN
    auto configurationFilePath = savedPath.replace("file:///", "");
#else
    auto configurationFilePath = savedPath.replace("file://", "");
#endif

    QFile configurationFile(configurationFilePath);
    if (!configurationFile.open(QIODevice::WriteOnly)) {
        auto error = configurationFile.errorString();
        qInfo() << "Can't write YAML file by path: " << configurationFilePath << "\n" << error;
        return;
    }
    configurationFile.write(out.c_str());
    configurationFile.close();

    unmarkChanges();
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
    configurationFile.close();
    try {
        auto yamlRoot = YAML::Load(content.toStdString());

        if (!readPort(yamlRoot)) return;
        if (!readSecurePort(yamlRoot)) return;
        if (!readIsLogRequests(yamlRoot)) return;
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

bool ConfigurationViewModel::readSecurePort(const YAML::Node &node) noexcept
{
    m_isSecure = node["secure"].as<bool>(false);

    return true;
}

bool ConfigurationViewModel::readIsLogRequests(const YAML::Node &node) noexcept
{
    m_isLogRequests = node["logrequests"].as<bool>(false);

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

        m_aliases->insert(parts.first(), parts.last());
        ++it;
    }

    return !m_aliases->isEmpty();
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
        if (parts.length() != 2) continue;

        m_lastIdentifier++;

        auto routeMapping = new RouteMapping();
        routeMapping->setId(m_lastIdentifier);
        routeMapping->setLocalRoute(parts.value(0));
        routeMapping->setExternalRouteOriginal(parts.value(1));
        routeMapping->setExternalRoute(processExternalRoute(parts.value(1)));

        m_mappings->append(routeMapping);

        ++it;
    }

    return !m_mappings->isEmpty();
}

QString ConfigurationViewModel::processExternalRoute(QString &&externalRoute) const noexcept
{
    QRegularExpression regexp("\\{[a-zA-Z0-9\\_\\-]{0,}\\}");
    auto iterator = regexp.globalMatch(externalRoute);
    while (iterator.hasNext()) {
        auto match = iterator.next();
        QString word = match.captured(0);
        auto address = "" + word;
        address.replace("{", "").replace("}", "");
        if (m_aliases->contains(address)) {
            externalRoute.replace(word, m_aliases->value(address));
        }
    }
    QString result = std::move(externalRoute);
    return result;
}

void ConfigurationViewModel::setupRootMapping() noexcept
{
    m_rootMapping = nullptr;

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

bool ConfigurationViewModel::checkDuplicateRoute(const int id, const QString route) const noexcept
{
    auto iterator = std::find_if(
        m_mappings->cbegin(),
        m_mappings->cend(),
        [id, route] (const RouteMapping* mapping) {
            return mapping->id() != id && mapping->localRoute() == route;
        }
    );
    return iterator == m_mappings->end();
}

void ConfigurationViewModel::refreshAllAfterAlias() noexcept
{
    foreach (auto mapping, *m_mappings) {
        auto externalRoute = mapping->externalRoute();
        mapping->setExternalRoute(processExternalRoute(std::move(externalRoute)));
    }
}

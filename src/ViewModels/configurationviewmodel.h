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


#ifndef CONFIGURATIONVIEWMODEL_H
#define CONFIGURATIONVIEWMODEL_H

#include <QObject>
#include <QMap>
#include <QScopedPointer>
#include <QSharedPointer>
#include <QList>
#include "../ListModels/configurationmappinglistmodel.h"
#include "../ListModels/configurationaliaseslistmodel.h"
#include <yaml-cpp/yaml.h>
#include "../Models/routemapping.h"

class ConfigurationViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int port READ port NOTIFY portChanged)
    Q_PROPERTY(ConfigurationMappingListModel* mappingListModel READ mappingListModel NOTIFY mappingListModelChanged)
    Q_PROPERTY(ConfigurationAliasesListModel* aliasesListModel READ aliasesListModel NOTIFY aliasesListModelChanged)
    Q_PROPERTY(QString pathToYaml READ pathToYaml NOTIFY pathToYamlChanged)
    Q_PROPERTY(bool isConfigReaded READ isConfigReaded NOTIFY isConfigReadedChanged)
    Q_PROPERTY(bool isSecure READ isSecure NOTIFY isSecureChanged)
    Q_PROPERTY(QString serverProtocol READ serverProtocol NOTIFY serverProtocolChanged)
    Q_PROPERTY(bool isHasChanges READ isHasChanges NOTIFY isHasChangesChanged)

private:
    int m_port { 8080 };
    bool m_isSecure { false };
    const QString m_folderYamlPath { "proxyemy.yml" };
    QSharedPointer<QMap<QString, QString>> m_aliases { new QMap<QString, QString>() };
    QSharedPointer<QList<RouteMapping*>> m_mappings { new QList<RouteMapping*>() };
    RouteMapping* m_rootMapping { nullptr };
    QScopedPointer<ConfigurationMappingListModel> m_configurationMappingListModel { new ConfigurationMappingListModel(this) };
    QScopedPointer<ConfigurationAliasesListModel> m_aliasesListModel { new ConfigurationAliasesListModel(this)};
    QString m_pathToYaml { "" };
    bool m_isConfigReaded { false };
    int m_lastIdentifier { -1 };
    bool m_isHasChanges { false };

public:
    explicit ConfigurationViewModel(QObject *parent = nullptr);

    int port() const noexcept { return m_port; }
    ConfigurationMappingListModel* mappingListModel() const noexcept { return m_configurationMappingListModel.get(); }
    ConfigurationAliasesListModel* aliasesListModel() const noexcept { return m_aliasesListModel.get(); }

    QString pathToYaml() const noexcept { return m_pathToYaml; }

    bool isConfigReaded() const noexcept { return m_isConfigReaded; }

    bool isSecure() const noexcept { return m_isSecure; }

    QString serverProtocol() const noexcept { return m_isSecure ? "https" : "http"; }

    RouteMapping* getMappingByRoute(const QString& route);

    bool isHasChanges() const noexcept { return m_isHasChanges; }
    void markChanges() noexcept;
    void unmarkChanges() noexcept;

    Q_INVOKABLE void openConfigFolder() const noexcept;
    Q_INVOKABLE void editMapping(const int id) noexcept;
    Q_INVOKABLE void addMapping(const QString& localRoute, const QString& externalRoute) noexcept;
    Q_INVOKABLE void deleteMapping(const int index) noexcept;
    Q_INVOKABLE void editAlias(const QString& key) noexcept;
    Q_INVOKABLE void addAlias(const QString& alias, const QString& value) noexcept;
    Q_INVOKABLE void deleteAlias(const QString& key) noexcept;
    Q_INVOKABLE void saveConfiguration(const bool saveOpened, const QString& path) noexcept;

private:
    void readYaml(const QString& path) noexcept;
    bool readPort(const YAML::Node& node) noexcept;
    bool readSecurePort(const YAML::Node& node) noexcept;
    bool readAddresses(const YAML::Node& node) noexcept;
    bool readMappings(const YAML::Node& node) noexcept;
    QString processExternalRoute(QString&& externalRoute) const noexcept;
    void setupRootMapping() noexcept;
    void setupYamlPath(const QString& path) noexcept;
    bool checkDuplicateRoute(const int id, const QString route) const noexcept;
    void refreshAllAfterAlias() noexcept;

signals:
    void portChanged();
    void mappingListModelChanged();
    void pathToYamlChanged();
    void isConfigReadedChanged();
    void isSecureChanged();
    void serverProtocolChanged();
    void informationMessage(const QString& message);
    void errorMessage(const QString& message, const QString title);
    void aliasesListModelChanged();
    void isHasChangesChanged();

};

#endif // CONFIGURATIONVIEWMODEL_H

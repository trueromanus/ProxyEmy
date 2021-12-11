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
#include <yaml-cpp/yaml.h>
#include "../Models/routemapping.h"

class ConfigurationViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int port READ port NOTIFY portChanged)
    Q_PROPERTY(ConfigurationMappingListModel* mappingListModel READ mappingListModel NOTIFY mappingListModelChanged)
    Q_PROPERTY(QString pathToYaml READ pathToYaml NOTIFY pathToYamlChanged)
    Q_PROPERTY(bool isConfigReaded READ isConfigReaded NOTIFY isConfigReadedChanged)

private:
    int m_port { 8080 };
    QScopedPointer<QMap<QString, QString>> m_addresses { new QMap<QString, QString>() };
    QSharedPointer<QList<RouteMapping*>> m_mappings { new QList<RouteMapping*>() };
    RouteMapping* m_rootMapping { nullptr };
    QScopedPointer<ConfigurationMappingListModel> m_configurationMappingListModel { new ConfigurationMappingListModel() };
    QString m_pathToYaml { "" };
    bool m_isConfigReaded { false };

public:
    explicit ConfigurationViewModel(QObject *parent = nullptr);

    int port() const noexcept { return m_port; }
    ConfigurationMappingListModel* mappingListModel() const noexcept { return m_configurationMappingListModel.get(); }

    QString pathToYaml() const noexcept { return m_pathToYaml; }

    bool isConfigReaded() const noexcept { return m_isConfigReaded; }

    RouteMapping* getMappingByRoute(const QString& route);

    Q_INVOKABLE void openConfigFolder() const noexcept;

private:
    void readYaml(const QString& path) noexcept;
    bool readPort(const YAML::Node& node) noexcept;
    bool readAddresses(const YAML::Node& node) noexcept;
    bool readMappings(const YAML::Node& node) noexcept;
    QString processExternalRoute(QString&& externalRoute) const noexcept;
    void setupRootMapping() noexcept;
    void setupYamlPath(const QString& path) noexcept;

signals:
    void portChanged();
    void mappingListModelChanged();
    void pathToYamlChanged();
    void isConfigReadedChanged();

};

#endif // CONFIGURATIONVIEWMODEL_H

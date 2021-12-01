#ifndef CONFIGURATIONVIEWMODEL_H
#define CONFIGURATIONVIEWMODEL_H

#include <QObject>
#include <QMap>
#include <QScopedPointer>
#include <yaml-cpp/yaml.h>
#include "../Models/routemapping.h"

class ConfigurationViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int port READ port NOTIFY portChanged)

private:
    int m_port { 8080 };
    QScopedPointer<QMap<QString, QString>> m_addresses { new QMap<QString, QString>() };
    QScopedPointer<QList<RouteMapping*>> m_mappings { new QList<RouteMapping*>() };

public:
    explicit ConfigurationViewModel(QObject *parent = nullptr);

    int port() const noexcept { return m_port; }

private:
    void readYaml(const QString& path) noexcept;
    bool readPort(const YAML::Node& node) noexcept;
    bool readAddresses(const YAML::Node& node) noexcept;
    bool readMappings(const YAML::Node& node) noexcept;
    QString processExternalRoute(QString&& externalRoute) const noexcept;

signals:
    void portChanged();

};

#endif // CONFIGURATIONVIEWMODEL_H

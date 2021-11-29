#ifndef CONFIGURATIONVIEWMODEL_H
#define CONFIGURATIONVIEWMODEL_H

#include <QObject>
#include <yaml-cpp/yaml.h>

class ConfigurationViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int port READ port NOTIFY portChanged)

private:
    int m_port { 8080 };

public:
    explicit ConfigurationViewModel(QObject *parent = nullptr);

    int port() const noexcept { return m_port; }

private:
    void readYaml(const QString& path) noexcept;

signals:
    void portChanged();

};

#endif // CONFIGURATIONVIEWMODEL_H

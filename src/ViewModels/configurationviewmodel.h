#ifndef CONFIGURATIONVIEWMODEL_H
#define CONFIGURATIONVIEWMODEL_H

#include <QObject>
#include <yaml-cpp/yaml.h>

class ConfigurationViewModel : public QObject
{
    Q_OBJECT
public:
    explicit ConfigurationViewModel(QObject *parent = nullptr);

private:
    void readYaml(const QString& path);

signals:

};

#endif // CONFIGURATIONVIEWMODEL_H

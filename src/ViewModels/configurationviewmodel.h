#ifndef CONFIGURATIONVIEWMODEL_H
#define CONFIGURATIONVIEWMODEL_H

#include <QObject>

class ConfigurationViewModel : public QObject
{
    Q_OBJECT
public:
    explicit ConfigurationViewModel(QObject *parent = nullptr);

signals:

};

#endif // CONFIGURATIONVIEWMODEL_H

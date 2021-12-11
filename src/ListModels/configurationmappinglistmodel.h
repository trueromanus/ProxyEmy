#ifndef CONFIGURATIONMAPPINGLISTMODEL_H
#define CONFIGURATIONMAPPINGLISTMODEL_H

#include <QObject>
#include <QList>
#include <QSharedPointer>
#include "../Models/routemapping.h"

class ConfigurationMappingListModel : public QObject
{
    Q_OBJECT
private:
    QSharedPointer<QList<RouteMapping*>> m_mappings { nullptr };

public:
    explicit ConfigurationMappingListModel(QObject *parent = nullptr);

    void setup(QSharedPointer<QList<RouteMapping*>> mappings);

signals:

};

#endif // CONFIGURATIONMAPPINGLISTMODEL_H

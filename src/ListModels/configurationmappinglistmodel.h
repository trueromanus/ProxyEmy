#ifndef CONFIGURATIONMAPPINGLISTMODEL_H
#define CONFIGURATIONMAPPINGLISTMODEL_H

#include <QObject>
#include <QList>
#include <QSharedPointer>
#include <QAbstractTableModel>
#include <QVariant>
#include <QByteArray>
#include <QHash>
#include "../Models/routemapping.h"

class ConfigurationMappingListModel : public QAbstractTableModel
{
    Q_OBJECT
private:
    QSharedPointer<QList<RouteMapping*>> m_mappings { nullptr };

    enum CommonItemRoles {
        IdRole = Qt::UserRole + 1,
        LocalRouteRole,
        ExternalRouteRole
    };

public:
    explicit ConfigurationMappingListModel(QObject *parent = nullptr);

    void setup(QSharedPointer<QList<RouteMapping*>> mappings);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

signals:

};

#endif // CONFIGURATIONMAPPINGLISTMODEL_H

#include "configurationmappinglistmodel.h"

ConfigurationMappingListModel::ConfigurationMappingListModel(QObject *parent)
    : QAbstractTableModel{parent}
{

}

void ConfigurationMappingListModel::setup(QSharedPointer<QList<RouteMapping*>> mappings)
{
    m_mappings = mappings;
}

int ConfigurationMappingListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;
    if (m_mappings == nullptr) return 0;
    if (m_mappings->size() < 0) return 0;

    return m_mappings->size();
}

int ConfigurationMappingListModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return 2;
}

QVariant ConfigurationMappingListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto mapping = m_mappings->at(index.row());
    if (mapping == nullptr) return QVariant();

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
            case 0: {
                return QVariant(mapping->localRoute());
            }
            case 1: {
                return QVariant(mapping->externalRoute());
            }
        }
    } else {
        switch (role) {
            case IdRole: {
                return QVariant(1);
            }
            case LocalRouteRole: {
                return QVariant(mapping->localRoute());
            }
            case ExternalRouteRole: {
                return QVariant(mapping->externalRoute());
            }
        }
    }

    return QVariant();
}

QHash<int, QByteArray> ConfigurationMappingListModel::roleNames() const
{
    return {
        {
            Qt::DisplayRole,
            "display"
        },
        {
            IdRole,
            "indentifier"
        },
        {
            LocalRouteRole,
            "localRoute"
        },
        {
            ExternalRouteRole,
            "externalRoute"
        }
    };
}

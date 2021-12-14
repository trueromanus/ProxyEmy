#include "configurationmappinglistmodel.h"

ConfigurationMappingListModel::ConfigurationMappingListModel(QObject *parent)
    : QAbstractTableModel{parent}
{
    m_columnWidth->insert(0, 40);
    m_columnWidth->insert(1, 40);
    m_columnWidth->insert(2, 20);
}

void ConfigurationMappingListModel::setup(QSharedPointer<QList<RouteMapping*>> mappings)
{
    m_mappings = mappings;

    emit isHasMappingsChanged();
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

    return 3;
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
            case 2: {
                return QVariant("");
            }
        }
    } else {
        switch (role) {
            case IdRole: {
                return QVariant(mapping->id());
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

int ConfigurationMappingListModel::getColumnWidth(const int index, const int fullWidth) const
{
    int width = 0;
    if (m_columnWidth->contains(index)) width = m_columnWidth->value(index);

    return width > 0 ? width * fullWidth / 100 : 0;
}

void ConfigurationMappingListModel::deleteMapping(const int index) noexcept
{
    if (index >= m_mappings->size()) return;

    beginResetModel();
    m_mappings->removeAt(index);
    endResetModel();

    emit isHasMappingsChanged();
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

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

void ConfigurationMappingListModel::refresh() noexcept
{
    beginResetModel();
    //TODO: add filtering
    endResetModel();

    emit isHasMappingsChanged();
}

void ConfigurationMappingListModel::refreshItem(const int itemIndex) noexcept
{
    emit dataChanged(index(itemIndex, 0), index(itemIndex, 2));
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

    switch (role) {
        case IdRole: {
            return QVariant(mapping->id());
        }
        case TextValueRole: {
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
        }
        case LocalRouteRole: {
            return QVariant(mapping->localRoute());
        }
        case ExternalRouteRole: {
            return QVariant(mapping->externalRoute());
        }
        case IsEditingRole: {
            return QVariant(m_editing->contains(mapping->id()));
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
            "identifier"
        },
        {
            LocalRouteRole,
            "localRoute"
        },
        {
            ExternalRouteRole,
            "externalRoute"
        },
        {
            IsEditingRole,
            "isEditing"
        },
        {
            TextValueRole,
            "textValue"
        }
    };
}

std::tuple<QString, QString> ConfigurationMappingListModel::getEditingValues(const int id)
{
    if (!m_editing->contains(id)) return std::make_tuple<QString, QString>("", "");

    return m_editing->value(id);
}

int ConfigurationMappingListModel::getColumnWidth(const int index, const int fullWidth) const
{
    int width = 0;
    if (m_columnWidth->contains(index)) width = m_columnWidth->value(index);

    return width > 0 ? width * fullWidth / 100 : 0;
}

void ConfigurationMappingListModel::setEditingValue(const int id, const int column, const QString &value)
{
    if (!m_editing->contains(id)) return;

    auto editingFields = m_editing->value(id);
    switch (column) {
        case LocalRouteColumn:
            std::get<LocalRouteColumn>(editingFields) = value;
            break;
        case ExternalRouteColumn:
            std::get<ExternalRouteColumn>(editingFields) = value;
            break;
        default: return;
    }
    (*m_editing)[id] = editingFields;
}

void ConfigurationMappingListModel::enableEditing(const int itemIndex)
{
    if (itemIndex >= m_mappings->size()) return;

    auto mapping = m_mappings->at(itemIndex);
    if (m_editing->contains(mapping->id())) return;

    auto tuple = std::make_tuple<QString, QString>(mapping->localRoute(), mapping->externalRouteOriginal());
    m_editing->insert(mapping->id(), tuple);

    emit dataChanged(index(itemIndex, 0), index(itemIndex, 2));
}

void ConfigurationMappingListModel::disableEditing(const int itemIndex)
{
    if (itemIndex >= m_mappings->size()) return;

    auto mapping = m_mappings->at(itemIndex);
    if (!m_editing->contains(mapping->id())) return;

    m_editing->remove(mapping->id());
    emit dataChanged(index(itemIndex, 0), index(itemIndex, 2));
}

#include "configurationaliaseslistmodel.h"

ConfigurationAliasesListModel::ConfigurationAliasesListModel(QObject *parent)
    : QAbstractTableModel{parent}
{
    m_columnWidth->insert(0, 50);
    m_columnWidth->insert(1, 50);
}

void ConfigurationAliasesListModel::setup(QSharedPointer<QMap<QString, QString> > aliases)
{
    m_aliases = aliases;
    m_innerAliases = aliases->keys();
}

void ConfigurationAliasesListModel::refresh() noexcept
{
    beginResetModel();
    endResetModel();

    m_innerAliases = m_aliases->keys();
    emit isHasAliasesChanged();
}

void ConfigurationAliasesListModel::refreshItem(const int itemIndex) noexcept
{
    emit dataChanged(index(itemIndex, 0), index(itemIndex, 2));
}

int ConfigurationAliasesListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;
    if (m_aliases == nullptr) return 0;
    if (m_aliases->size() < 0) return 0;

    return m_aliases->size();
}

int ConfigurationAliasesListModel::columnCount(const QModelIndex &parent) const
{
     if (parent.isValid()) return 0;

    return 2;
}

QVariant ConfigurationAliasesListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto aliasKey = m_innerAliases.value(index.row());
    if (aliasKey == nullptr) return QVariant();
    auto aliasValue = m_aliases->value(aliasKey);

    switch (role) {
        case TextValueRole: {
            switch (index.column()) {
                case 0: {
                    return QVariant(aliasKey);
                }
                case 1: {
                    return QVariant(aliasValue);
                }
            }
        }
        case AliasValueRole: {
            return QVariant(aliasValue);
        }
        case AliasRole: {
            return QVariant(aliasKey);
        }
        case IsEditingRole: {
            return QVariant(m_editing->contains(aliasKey));
        }
    }

    return QVariant();
}

QHash<int, QByteArray> ConfigurationAliasesListModel::roleNames() const
{
    return {
        {
            AliasRole,
            "alias"
        },
        {
            AliasValueRole,
            "aliasValue"
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

std::tuple<QString, QString> ConfigurationAliasesListModel::getEditingValues(const QString& key)
{
    if (!m_editing->contains(key)) return std::make_tuple<QString, QString>("", "");

    return m_editing->value(key);
}

int ConfigurationAliasesListModel::getColumnWidth(const int index, const int fullWidth) const
{
    int width = 0;
    if (m_columnWidth->contains(index)) width = m_columnWidth->value(index);

    return width > 0 ? width * fullWidth / 100 : 0;
}

void ConfigurationAliasesListModel::setEditingValue(const QString& key, const int column, const QString &value)
{
    if (!m_editing->contains(key)) return;

    auto editingFields = m_editing->value(key);
    switch (column) {
        case AliasColumn:
            std::get<AliasColumn>(editingFields) = value;
            break;
        case AliasValueColumn:
            std::get<AliasValueColumn>(editingFields) = value;
            break;
        default: return;
    }
    (*m_editing)[key] = editingFields;
}

void ConfigurationAliasesListModel::enableEditing(const QString &key)
{
    if (m_editing->contains(key)) return;
    auto aliasKey = key;
    auto tuple = std::make_tuple<QString, QString>(std::move(aliasKey), m_aliases->value(aliasKey));
    m_editing->insert(key, tuple);

    auto itemIndex = m_innerAliases.indexOf(key);
    emit dataChanged(index(itemIndex, 0), index(itemIndex, 2));
}

void ConfigurationAliasesListModel::disableEditing(const QString &key)
{
    if (!m_editing->contains(key)) return;

    m_editing->remove(key);
    auto itemIndex = m_innerAliases.indexOf(key);
    emit dataChanged(index(itemIndex, 0), index(itemIndex, 2));
}

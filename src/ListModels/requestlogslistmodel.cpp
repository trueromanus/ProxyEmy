#include "requestlogslistmodel.h"

RequestLogsListModel::RequestLogsListModel(QObject *parent)
    : QAbstractTableModel{parent}
{
    m_columnWidth->insert(0, 6);
    m_columnWidth->insert(1, 50);
    m_columnWidth->insert(2, 25);
    m_columnWidth->insert(3, 19);
}

void RequestLogsListModel::setup(QSharedPointer<QList<RequestLogItem *> > items)
{
    m_items = items;
}

void RequestLogsListModel::refresh()
{
    beginResetModel();
    //TODO: add filtering
    endResetModel();
}

int RequestLogsListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return m_items->size();
}

int RequestLogsListModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return 4;
}

QVariant RequestLogsListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto item = m_items->at(index.row());
    if (item == nullptr) return QVariant();

    switch (role) {
        case TextValueRole: {
            switch (index.column()) {
                case 0: {
                    return QVariant(item->method());
                }
                case 1: {
                    return QVariant(item->path());
                }
                case 2: {
                    return QVariant(item->raised());
                }
                case 3: {
                    return QVariant(item->route());
                }
            }
        }
    }

    return QVariant();
}

QHash<int, QByteArray> RequestLogsListModel::roleNames() const
{
    return {
        {
            TextValueRole,
            "textValue"
        }
    };
}

int RequestLogsListModel::getColumnWidth(const int index, const int fullWidth) const
{
    int width = 0;
    if (m_columnWidth->contains(index)) width = m_columnWidth->value(index);

    return width > 0 ? width * fullWidth / 100 : 0;
}

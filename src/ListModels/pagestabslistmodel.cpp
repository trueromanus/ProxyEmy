#include "pagestabslistmodel.h"

PagesTabsListModel::PagesTabsListModel(QObject *parent)
    : QAbstractListModel{parent}
{
    m_tabs->append("Server Info");
    m_tabs->append("Request Log");
}

int PagesTabsListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return m_tabs->size();
}

QVariant PagesTabsListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto tabIndex = index.row();
    auto tab = m_tabs->at(tabIndex);

    switch (role) {
        case TitleRole: {
            return QVariant(tab);
        }
        case IsActiveRole: {
            return QVariant(m_activatedTab == tabIndex);
        }
        case IdRole: {
            return QVariant(tabIndex);
        }
    }

    return QVariant();
}

QHash<int, QByteArray> PagesTabsListModel::roleNames() const
{
    return {
        {
            TitleRole,
            "title"
        },
        {
            IsActiveRole,
            "isActived"
        },
        {
            IdRole,
            "identifier"
        }
    };
}

void PagesTabsListModel::activateTab(const int newIndex)
{
    if (newIndex >= m_tabs->count()) return;

    auto oldIndex = m_activatedTab;
    m_activatedTab = newIndex;

    emit dataChanged(index(oldIndex), index(oldIndex));
    emit dataChanged(index(m_activatedTab), index(m_activatedTab));
}

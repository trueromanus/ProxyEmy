/*
    ProxyEmy local proxy server
    Copyright (C) 2022 Roman Vladimirov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "pagestabslistmodel.h"

PagesTabsListModel::PagesTabsListModel(QObject *parent)
    : QAbstractListModel{parent}
{
    m_tabs->append("Server Info");
    m_tabs->append("Requests Log");
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
    emit isServerInfoVisibleChanged();
    emit isRequestLogVisibleChanged();
}

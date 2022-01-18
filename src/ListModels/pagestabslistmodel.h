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

#ifndef PAGESTABSLISTMODEL_H
#define PAGESTABSLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>

class PagesTabsListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(bool isServerInfoVisible READ isServerInfoVisible NOTIFY isServerInfoVisibleChanged)
    Q_PROPERTY(bool isRequestLogVisible READ isRequestLogVisible NOTIFY isRequestLogVisibleChanged)

private:
    const int ServerInfoTab = 0;
    const int RequestsLogTab = 1;
    QScopedPointer<QStringList> m_tabs { new QStringList() };
    int m_activatedTab = { 0 };

    enum PagesTabsRoles {
        TitleRole = Qt::UserRole + 1,
        IdRole,
        IsActiveRole
    };

public:
    explicit PagesTabsListModel(QObject *parent = nullptr);

    bool isServerInfoVisible() const noexcept { return m_activatedTab == ServerInfoTab; }
    bool isRequestLogVisible() const noexcept { return m_activatedTab == RequestsLogTab; }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void activateTab(const int newIndex);

signals:
    void isServerInfoVisibleChanged();
    void isRequestLogVisibleChanged();

};

#endif // PAGESTABSLISTMODEL_H

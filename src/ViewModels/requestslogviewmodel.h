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

#ifndef REQUESTSLOGVIEWMODEL_H
#define REQUESTSLOGVIEWMODEL_H

#include <QObject>
#include <QList>
#include "../Models/requestlogitem.h"
#include "../ListModels/requestlogslistmodel.h"

class RequestsLogViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(RequestLogsListModel* listModel READ listModel NOTIFY listModelChanged)
    Q_PROPERTY(bool isHasItems READ isHasItems NOTIFY isHasItemsChanged)

private:
    QSharedPointer<QList<RequestLogItem*>> m_items { new QList<RequestLogItem*>() };
    QScopedPointer<RequestLogsListModel> m_listModel { new RequestLogsListModel() };

public:
    explicit RequestsLogViewModel(QObject *parent = nullptr);

    RequestLogsListModel* listModel() const noexcept { return m_listModel.get(); }

    bool isHasItems() const noexcept { return !m_items->isEmpty(); }

    void addRecord(const QString& path, const QString& protocol, const QString& method, const QString& route) noexcept;

signals:
    void listModelChanged();
    void isHasItemsChanged();

};

#endif // REQUESTSLOGVIEWMODEL_H

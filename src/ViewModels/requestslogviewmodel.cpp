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

#include "requestslogviewmodel.h"

RequestsLogViewModel::RequestsLogViewModel(QObject *parent)
    : QObject{parent}
{
    m_listModel->setup(m_items);
}

void RequestsLogViewModel::addRecord(const QString &path, const QString &protocol, const QString &method, const QString &route) noexcept
{
    auto record = new RequestLogItem();
    record->setPath(path);
    record->setProtocol(protocol);
    record->setMethod(method);
    record->setRoute(route);

    auto isEmpty = m_items->isEmpty();

    m_items->append(record);

    if (isEmpty) emit isHasItemsChanged();

    m_listModel->refresh();
}

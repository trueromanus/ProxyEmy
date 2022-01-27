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

#include "notificationhubviewmodel.h"

NotificationHubViewModel::NotificationHubViewModel(QObject *parent)
    : QObject{parent}
{
    m_pool->setup(m_messages);
}

void NotificationHubViewModel::pushMessage(const QString &title, const QString &message, const QString &topic)
{
    m_messages->append(std::make_tuple(title, message, topic));
    m_pool->pushMessage(m_messages->count() - 1);
}

void NotificationHubViewModel::pushErrorMessage(const QString &title, const QString &message)
{
    pushMessage(title, message, m_errorTopic);
}

void NotificationHubViewModel::pushInfoMessage(const QString &title, const QString &message)
{
    pushMessage(title, message, m_infoTopic);
}


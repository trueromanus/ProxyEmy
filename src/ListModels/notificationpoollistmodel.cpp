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

#include "notificationpoollistmodel.h"

NotificationPoolListModel::NotificationPoolListModel(QObject *parent)
    : QAbstractListModel{parent}
{
    m_timer->setInterval(500);
    connect(m_timer.get(), &QTimer::timeout, this, &NotificationPoolListModel::timerTriggered);
}

void NotificationPoolListModel::setup(QSharedPointer<QList<std::tuple<QString, QString, QString>>> messages)
{
    m_messages = messages;

}

void NotificationPoolListModel::pushMessage(const int index) noexcept
{
    auto expired = QDateTime::currentDateTime().addSecs(2);
    m_currentEvents->enqueue(std::make_tuple(index, expired));
    if (!m_timer->isActive()) m_timer->start();
    refresh();
}

void NotificationPoolListModel::refresh() noexcept
{
    beginResetModel();
    endResetModel();
}

int NotificationPoolListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;
    if (m_messages == nullptr) return 0;

    return m_currentEvents->size();
}

QVariant NotificationPoolListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto currentEvent = m_currentEvents->at(index.row());
    auto indexOfItem = std::get<IndexField>(currentEvent);
    auto item = m_messages->at(indexOfItem);
    auto topic = std::get<NotificationTopicField>(item);

    switch (role) {
        case TitleRole: {
            return QVariant(std::get<NotificationTitleField>(item));
        }
        case MessageRole: {
            return QVariant(std::get<NotificationMessageField>(item));
        }
        case IsErrorRole: {
            return QVariant(topic == "error");
        }
        case IsInformationRole: {
            return QVariant(topic == "info");
        }
        case IsAnimationRole: {
            return m_currentAnimating && index.row() == 0;
        }
    }

    return QVariant();
}

QHash<int, QByteArray> NotificationPoolListModel::roleNames() const
{
    return {
        {
            TitleRole,
            "title"
        },
        {
            MessageRole,
            "message"
        },
        {
            IsErrorRole,
            "isError"
        },
        {
            IsInformationRole,
            "isInformation"
        },
        {
            IsAnimationRole,
            "isActiveAnimation"
        }
    };
}

void NotificationPoolListModel::itemAnimationEnded() noexcept
{
    m_currentAnimating = false;

    beginResetModel();
    m_currentEvents->dequeue();
    endResetModel();

    if (m_currentEvents->isEmpty()) m_timer->stop();
}

void NotificationPoolListModel::timerTriggered()
{
    if (m_currentAnimating) return;
    if (m_currentEvents->isEmpty()) return;

    auto expired = std::get<ExpiredField>(m_currentEvents->head());
    if (QDateTime::currentDateTime() >= expired) {
        m_currentAnimating = true;
        emit dataChanged(index(0), index(0));
    }
}

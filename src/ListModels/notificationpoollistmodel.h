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

#ifndef NOTIFICATIONPOOLLISTMODEL_H
#define NOTIFICATIONPOOLLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QQueue>
#include <QDateTime>
#include <QTimer>
#include <QSharedPointer>
#include "../globalenums.h"

class NotificationPoolListModel : public QAbstractListModel
{
    Q_OBJECT
private:
    bool m_currentAnimating { false };
    QScopedPointer<QQueue<std::tuple<int, QDateTime>>> m_currentEvents { new QQueue<std::tuple<int, QDateTime>>() };
    QScopedPointer<QTimer> m_timer { new QTimer() };
    QSharedPointer<QList<std::tuple<QString, QString, QString>>> m_messages { nullptr };

    enum NotificationPoolField {
        IndexField = 0,
        ExpiredField
    };

    enum NotificationPoolRoles {
        TitleRole = Qt::UserRole + 1,
        MessageRole,
        IsErrorRole,
        IsInformationRole,
        IsAnimationRole
    };

public:
    explicit NotificationPoolListModel(QObject *parent = nullptr);
    void setup(QSharedPointer<QList<std::tuple<QString, QString, QString>>> messages);
    void pushMessage(const int index) noexcept;
    void refresh() noexcept;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    Q_INVOKABLE void itemAnimationEnded() noexcept;

signals:
    void startItemAnimation(const int index);

private slots:
    void timerTriggered();

};

#endif // NOTIFICATIONPOOLLISTMODEL_H

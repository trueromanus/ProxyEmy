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

#ifndef NOTIFICATIONHUBVIEWMODEL_H
#define NOTIFICATIONHUBVIEWMODEL_H

#include <QObject>
#include <QList>
#include <QSharedPointer>
#include "../ListModels/notificationpoollistmodel.h"
#include "../globalenums.h"

class NotificationHubViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(NotificationPoolListModel* pool READ pool NOTIFY poolChanged)
private:
    QSharedPointer<QList<std::tuple<QString, QString, QString>>> m_messages { new QList<std::tuple<QString, QString, QString>>() };
    QScopedPointer<NotificationPoolListModel> m_pool { new NotificationPoolListModel(this) };
    QString m_errorTopic { "error" };
    QString m_infoTopic { "info" };

public:
    explicit NotificationHubViewModel(QObject *parent = nullptr);

    NotificationPoolListModel* pool() const noexcept { return m_pool.get(); };

    Q_INVOKABLE void pushMessage(const QString& title, const QString& message, const QString& topic);
    Q_INVOKABLE void pushErrorMessage(const QString& title, const QString& message);
    Q_INVOKABLE void pushInfoMessage(const QString& title, const QString& message);

signals:
    void poolChanged();

};

#endif // NOTIFICATIONHUBVIEWMODEL_H

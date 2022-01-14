#ifndef NOTIFICATIONHUBVIEWMODEL_H
#define NOTIFICATIONHUBVIEWMODEL_H

#include <QObject>
#include <QList>
#include <QSharedPointer>
#include "../ListModels/notificationpoollistmodel.h"

class NotificationHubViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(NotificationPoolListModel* pool READ pool NOTIFY poolChanged)
private:
    QSharedPointer<QList<std::tuple<QString, QString, QString>>> m_messages { new QList<std::tuple<QString, QString, QString>>() };
    QScopedPointer<NotificationPoolListModel> m_pool { new NotificationPoolListModel(this) };
    QString m_errorTopic { "error" };
    QString m_infoTopic { "info" };

    enum NotificationField {
        NotificationTitleField = 0,
        NotificationMessageField,
        NotificationTopicField
    };

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

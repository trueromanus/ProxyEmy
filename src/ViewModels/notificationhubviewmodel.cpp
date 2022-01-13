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


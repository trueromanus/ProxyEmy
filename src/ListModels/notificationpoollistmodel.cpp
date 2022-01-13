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
    auto expired = QDateTime::currentDateTime().addSecs(4);
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

    qDebug() << "LALALLALALA";

    auto currentEvent = m_currentEvents->at(index.row());
    auto indexOfItem = std::get<IndexField>(currentEvent);
    auto item = m_messages->at(indexOfItem);
    auto topic = std::get<2>(item);

    switch (role) {
        case TitleRole: {
            return QVariant(std::get<0>(item));
        }
        case MessageRole: {
            return QVariant(std::get<1>(item));
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
            "isAnimation"
        }
    };
}

void NotificationPoolListModel::timerTriggered()
{
    if (m_currentAnimating) return;

    auto expired = std::get<ExpiredField>(m_currentEvents->head());
    if (QDateTime::currentDateTime() >= expired) {
        m_currentAnimating = true;
        auto indexItem = std::get<IndexField>(m_currentEvents->head());
        emit startItemAnimation(indexItem);
        emit dataChanged(index(indexItem), index(indexItem));
    }
}

void NotificationPoolListModel::itemAnimationEnded()
{
    m_currentAnimating = false;

    beginResetModel();
    m_currentEvents->dequeue();
    endResetModel();
}

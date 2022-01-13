#ifndef NOTIFICATIONPOOLLISTMODEL_H
#define NOTIFICATIONPOOLLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QQueue>
#include <QDateTime>
#include <QTimer>
#include <QSharedPointer>

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

signals:
    void startItemAnimation(const int index);

private slots:
    void timerTriggered();
    void itemAnimationEnded();

};

#endif // NOTIFICATIONPOOLLISTMODEL_H

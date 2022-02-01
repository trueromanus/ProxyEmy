#ifndef REQUESTLOGSLISTMODEL_H
#define REQUESTLOGSLISTMODEL_H

#include <QObject>
#include <QList>
#include <QAbstractTableModel>
#include "../Models/requestlogitem.h"

class RequestLogsListModel : public QAbstractTableModel
{
    Q_OBJECT

private:
    QSharedPointer<QList<RequestLogItem*>> m_items { nullptr };
    QScopedPointer<QList<RequestLogItem*>> m_filtertedItems { new QList<RequestLogItem*>() };
    QScopedPointer<QMap<int, int>> m_columnWidth { new QMap<int, int>() };

    enum RequestItemRoles {
        TextValueRole = Qt::UserRole + 1,
    };

public:
    explicit RequestLogsListModel(QObject *parent = nullptr);

    void setup(QSharedPointer<QList<RequestLogItem*>> items);

    void refresh();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE int getColumnWidth(const int index, const int fullWidth) const;

private:
    void sortFilteringItems() noexcept;

signals:

};

#endif // REQUESTLOGSLISTMODEL_H

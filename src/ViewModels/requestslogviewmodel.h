#ifndef REQUESTSLOGVIEWMODEL_H
#define REQUESTSLOGVIEWMODEL_H

#include <QObject>
#include <QList>
#include "../Models/requestlogitem.h"
#include "../ListModels/requestlogslistmodel.h"

class RequestsLogViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(RequestLogsListModel* listModel READ listModel NOTIFY listModelChanged)
    Q_PROPERTY(bool isHasItems READ isHasItems NOTIFY isHasItemsChanged)

private:
    QSharedPointer<QList<RequestLogItem*>> m_items { new QList<RequestLogItem*>() };
    QScopedPointer<RequestLogsListModel> m_listModel { new RequestLogsListModel() };

public:
    explicit RequestsLogViewModel(QObject *parent = nullptr);

    RequestLogsListModel* listModel() const noexcept { return m_listModel.get(); }

    bool isHasItems() const noexcept { return !m_items->isEmpty(); }

    void addRecord(const QString& path, const QString& protocol, const QString& method, const QString& route) noexcept;

signals:
    void listModelChanged();
    void isHasItemsChanged();

};

#endif // REQUESTSLOGVIEWMODEL_H

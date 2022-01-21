#include "requestslogviewmodel.h"

RequestsLogViewModel::RequestsLogViewModel(QObject *parent)
    : QObject{parent}
{
    m_listModel->setup(m_items);
}

void RequestsLogViewModel::addRecord(const QString &path, const QString &protocol, const QString &method, const QString &route) noexcept
{
    auto record = new RequestLogItem();
    record->setPath(path);
    record->setProtocol(protocol);
    record->setMethod(method);
    record->setRoute(route);

    auto isEmpty = m_items->isEmpty();

    m_items->append(record);

    if (isEmpty) emit isHasItemsChanged();

    m_listModel->refresh();
}

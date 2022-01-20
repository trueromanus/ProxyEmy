#include "requestlogitem.h"

RequestLogItem::RequestLogItem()
{

}

void RequestLogItem::setPath(const QString &path) noexcept
{
    m_path = path;
}

void RequestLogItem::setProtocol(const QString &protocol) noexcept
{
    m_protocol = protocol;
}

void RequestLogItem::setMethod(const QString &method) noexcept
{
    m_method = method;
}

void RequestLogItem::setRoute(const QString &route) noexcept
{
    m_route = route;
}

void RequestLogItem::setRaised(const QDateTime &raised) noexcept
{
    m_raised = raised;
}

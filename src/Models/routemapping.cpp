#include "routemapping.h"

RouteMapping::RouteMapping()
{

}

void RouteMapping::setExternalRoute(const QString &externalRoute) noexcept
{
    m_externalRoute = externalRoute;
    m_externalRouteUrl = QUrl(externalRoute);
}

QString RouteMapping::getExternalHost() noexcept
{
    return m_externalRouteUrl.host();
}

int RouteMapping::getExternalPort() noexcept
{
    return m_externalRouteUrl.port();
}

QString RouteMapping::mapLocalToExternal(const QString& currentRoute) noexcept
{
    if (m_localRoute == "/") return currentRoute;

    return currentRoute.mid(m_localRoute.length());
}

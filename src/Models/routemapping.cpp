#include "routemapping.h"

RouteMapping::RouteMapping()
{

}

void RouteMapping::setExternalRoute(const QString &externalRoute) noexcept
{
    m_externalRoute = externalRoute;
    m_externalRouteUrl = QUrl(externalRoute);
}

QString RouteMapping::getExternalHost() const noexcept
{
    return m_externalRouteUrl.host();
}

int RouteMapping::getExternalPort() const noexcept
{
    auto port = m_externalRouteUrl.port();

    if (port == -1) return isExternalSecure() ? 443 : 80;

    return port;
}

QString RouteMapping::mapLocalToExternal(const QString& currentRoute) const noexcept
{
    if (m_localRoute == "/") return currentRoute;

    auto path = currentRoute.mid(m_localRoute.length());
    if (path.isEmpty()) return "/";

    return path;
}

bool RouteMapping::isExternalSecure() const noexcept
{
    return m_externalRouteUrl.scheme().toLower() == m_secureScheme;
}

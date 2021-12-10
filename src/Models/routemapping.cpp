/*
    ProxyEmy local proxy server
    Copyright (C) 2022 Roman Vladimirov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/


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

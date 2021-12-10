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


#ifndef ROUTEMAPPING_H
#define ROUTEMAPPING_H

#include <QObject>
#include <QUrl>

class RouteMapping
{

private:
    int m_bindingType { 0 };
    QString m_localRoute { "" };
    QString m_externalRoute { "" };
    QUrl m_externalRouteUrl { QUrl("http://localhost") };
    QString m_secureScheme { "https" };

public:
    explicit RouteMapping();

    int bindingType() const noexcept { return m_bindingType; }
    void setBindingType(const int bindingType) noexcept { m_bindingType = bindingType; };

    QString localRoute() const noexcept { return m_localRoute; }
    void setLocalRoute(const QString& localRoute) noexcept { m_localRoute = localRoute; };

    QString externalRoute() const noexcept { return m_externalRoute; }
    void setExternalRoute(const QString& externalRoute) noexcept;

    QString getExternalHost() const noexcept;
    int getExternalPort() const noexcept;
    QString mapLocalToExternal(const QString& currentRoute) const noexcept;
    bool isExternalSecure() const noexcept;

};

#endif // ROUTEMAPPING_H

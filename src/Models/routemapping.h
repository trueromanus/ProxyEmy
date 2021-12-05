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

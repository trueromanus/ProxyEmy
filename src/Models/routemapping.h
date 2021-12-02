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

public:
    explicit RouteMapping();

    int bindingType() const noexcept { return m_bindingType; }
    void setBindingType(const int bindingType) noexcept { m_bindingType = bindingType; };

    QString localRoute() const noexcept { return m_localRoute; }
    void setLocalRoute(const QString& localRoute) noexcept { m_localRoute = localRoute; };

    QString externalRoute() const noexcept { return m_externalRoute; }
    void setExternalRoute(const QString& externalRoute) noexcept;

    QString getExternalHost() noexcept;
    int getExternalPort() noexcept;
    QString mapLocalToExternal(const QString& currentRoute) noexcept;

};

#endif // ROUTEMAPPING_H

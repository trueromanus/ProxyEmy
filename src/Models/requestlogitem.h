#ifndef REQUESTLOGITEM_H
#define REQUESTLOGITEM_H

#include <QString>
#include <QDateTime>

class RequestLogItem
{
private:
    QString m_path { "" };
    QString m_protocol { "" };
    QString m_method { "" };
    QDateTime m_raised { QDateTime::currentDateTime() };
    QString m_route { "" };

public:
    RequestLogItem();

    QString path() const noexcept { return m_path; }
    QString protocol() const noexcept { return m_protocol; }
    QString method() const noexcept { return m_method; }
    QString route() const noexcept { return m_route; }
    QDateTime raised() const noexcept { return m_raised; }

    void setPath(const QString& path) noexcept;
    void setProtocol(const QString& protocol) noexcept;
    void setMethod(const QString& method) noexcept;
    void setRoute(const QString& route) noexcept;
    void setRaised(const QDateTime& raised) noexcept;
};

#endif // REQUESTLOGITEM_H

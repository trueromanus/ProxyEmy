#ifndef PROXYEMYBACKEND_H
#define PROXYEMYBACKEND_H

#include <QObject>
#include <qqml.h>

class ProxyEmyBackend : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString windowTitle READ windowTitle NOTIFY windowTitleChanged)

private:
    QString m_windowTitle { "ProxyEmy" };

public:
    explicit ProxyEmyBackend(QObject *parent = nullptr);

    QString windowTitle() const noexcept { return m_windowTitle; };
    void setWindowTitle(const QString& windowTitle) noexcept;

signals:
    void windowTitleChanged();

};

#endif // PROXYEMYBACKEND_H

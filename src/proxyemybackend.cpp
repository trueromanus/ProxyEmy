#include "proxyemybackend.h"

ProxyEmyBackend::ProxyEmyBackend(QObject *parent) : QObject(parent)
{

}

void ProxyEmyBackend::setWindowTitle(const QString &windowTitle) noexcept
{
    if (m_windowTitle == windowTitle) return;

    m_windowTitle = windowTitle;
    emit windowTitleChanged();
}

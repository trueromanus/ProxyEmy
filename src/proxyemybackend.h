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


#ifndef PROXYEMYBACKEND_H
#define PROXYEMYBACKEND_H

#include <QObject>
#include <qqml.h>
#include "ListModels/pagestabslistmodel.h"

class ProxyEmyBackend : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString windowTitle READ windowTitle NOTIFY windowTitleChanged)
    Q_PROPERTY(PagesTabsListModel* tabs READ tabs NOTIFY tabsChanged)

private:
    QString m_windowTitle { "ProxyEmy" };
    QScopedPointer<PagesTabsListModel> m_tabs { new PagesTabsListModel() };

public:
    explicit ProxyEmyBackend(QObject *parent = nullptr);

    QString windowTitle() const noexcept { return m_windowTitle; };
    void setWindowTitle(const QString& windowTitle) noexcept;

    PagesTabsListModel* tabs() const noexcept { return m_tabs.get(); };

signals:
    void windowTitleChanged();
    void tabsChanged();

};

#endif // PROXYEMYBACKEND_H

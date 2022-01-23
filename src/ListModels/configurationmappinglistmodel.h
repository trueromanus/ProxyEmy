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

#ifndef CONFIGURATIONMAPPINGLISTMODEL_H
#define CONFIGURATIONMAPPINGLISTMODEL_H

#include <QObject>
#include <QList>
#include <QSharedPointer>
#include <QAbstractTableModel>
#include <QVariant>
#include <QByteArray>
#include <QHash>
#include "../Models/routemapping.h"

class ConfigurationMappingListModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(bool isHasMappings READ isHasMappings NOTIFY isHasMappingsChanged)

private:
    QSharedPointer<QList<RouteMapping*>> m_mappings { nullptr };
    QScopedPointer<QMap<int, std::tuple<QString, QString>>> m_editing { new QMap<int, std::tuple<QString, QString>>() };
    QScopedPointer<QMap<int, int>> m_columnWidth { new QMap<int, int>() };

    enum CommonItemRoles {
        IdRole = Qt::UserRole + 1,
        LocalRouteRole,
        ExternalRouteRole,
        IsEditingRole,
        TextValueRole,
        ExternalRouteOriginRole,
    };

    enum UserEditingColumns {
        LocalRouteColumn = 0,
        ExternalRouteColumn
    };

public:
    explicit ConfigurationMappingListModel(QObject *parent = nullptr);

    void setup(QSharedPointer<QList<RouteMapping*>> mappings);

    void refresh() noexcept;
    void refreshItem(const int itemIndex) noexcept;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    bool isHasMappings() const noexcept { return m_mappings != nullptr && !m_mappings->isEmpty(); }

    std::tuple<QString, QString> getEditingValues(const int id);

    Q_INVOKABLE int getColumnWidth(const int index, const int fullWidth) const;
    Q_INVOKABLE void setEditingValue(const int id, const int column, const QString& value);
    Q_INVOKABLE void enableEditing(const int itemIndex);
    Q_INVOKABLE void disableEditing(const int itemIndex);

signals:
    void isHasMappingsChanged();

};

#endif // CONFIGURATIONMAPPINGLISTMODEL_H

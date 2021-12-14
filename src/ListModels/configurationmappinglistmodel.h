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
    QScopedPointer<QMap<int, int>> m_columnWidth { new QMap<int, int>() };

    enum CommonItemRoles {
        IdRole = Qt::UserRole + 1,
        LocalRouteRole,
        ExternalRouteRole
    };

    enum CommonItemRoles {
        IdRole = Qt::UserRole + 1,
        LocalRouteRole,
        ExternalRouteRole
    };

public:
    explicit ConfigurationMappingListModel(QObject *parent = nullptr);

    void setup(QSharedPointer<QList<RouteMapping*>> mappings);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    bool isHasMappings() const noexcept { return m_mappings != nullptr && !m_mappings->isEmpty(); }

    Q_INVOKABLE int getColumnWidth(const int index, const int fullWidth) const;
    Q_INVOKABLE void deleteMapping(const int index) noexcept;

signals:
    void isHasMappingsChanged();

};

#endif // CONFIGURATIONMAPPINGLISTMODEL_H

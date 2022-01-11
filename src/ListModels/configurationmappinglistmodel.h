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

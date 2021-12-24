#ifndef CONFIGURATIONALIASESLISTMODEL_H
#define CONFIGURATIONALIASESLISTMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include <QSharedPointer>
#include <QMap>

class ConfigurationAliasesListModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(bool isHasAliases READ isHasAliases NOTIFY isHasAliasesChanged)

private:
    QSharedPointer<QMap<QString, QString>> m_aliases { nullptr };
    QStringList m_innerAliases { QStringList() };
    QScopedPointer<QMap<QString, std::tuple<QString, QString>>> m_editing { new QMap<QString, std::tuple<QString, QString>>() };
    QScopedPointer<QMap<int, int>> m_columnWidth { new QMap<int, int>() };

    enum AliasesRoles {
        AliasRole = Qt::UserRole + 1,
        AliasValueRole,
        IsEditingRole,
        TextValueRole
    };

    enum UserEditingColumns {
        AliasColumn = 0,
        AliasValueColumn
    };

public:
    explicit ConfigurationAliasesListModel(QObject *parent = nullptr);

    void setup(QSharedPointer<QMap<QString, QString>> aliases);

    bool isHasAliases() const noexcept { return m_aliases->isEmpty(); }

    void refresh() noexcept;
    void refreshItem(const int itemIndex) noexcept;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    std::tuple<QString, QString> getEditingValues(const QString& key);

    Q_INVOKABLE int getColumnWidth(const int index, const int fullWidth) const;
    Q_INVOKABLE void setEditingValue(const QString& key, const int column, const QString& value);
    Q_INVOKABLE void enableEditing(const QString &key);
    Q_INVOKABLE void disableEditing(const QString& key);

signals:
    void isHasAliasesChanged();

};

#endif // CONFIGURATIONALIASESLISTMODEL_H

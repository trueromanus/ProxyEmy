#ifndef PAGESTABSLISTMODEL_H
#define PAGESTABSLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>

class PagesTabsListModel : public QAbstractListModel
{
    Q_OBJECT

private:
    QScopedPointer<QStringList> m_tabs { new QStringList() };
    int m_activatedTab = { 0 };

    enum PagesTabsRoles {
        TitleRole = Qt::UserRole + 1,
        IdRole,
        IsActiveRole
    };

public:
    explicit PagesTabsListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void activateTab(const int newIndex);

signals:

};

#endif // PAGESTABSLISTMODEL_H

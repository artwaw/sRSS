#ifndef ITEMPROXYMODEL_H
#define ITEMPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QObject>

class ItemProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    ItemProxyModel(QObject *parent=nullptr);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
};

#endif // ITEMPROXYMODEL_H

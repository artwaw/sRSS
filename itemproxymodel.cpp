#include "itemproxymodel.h"

ItemProxyModel::ItemProxyModel(QObject *parent) : QSortFilterProxyModel(parent) {}

bool ItemProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
    if (filterKeyColumn()==14) {
        return !sourceModel()->data(sourceModel()->index(sourceRow,14,sourceParent)).toBool();
    }
    if (filterKeyColumn()==13) {
        return sourceModel()->data(sourceModel()->index(sourceRow,13,sourceParent)).toBool();
    }
    return QSortFilterProxyModel::filterAcceptsRow(sourceRow,sourceParent);
}

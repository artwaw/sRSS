#include "itemsmodel.h"

ItemsModel::ItemsModel(QObject *parent, QSqlDatabase db) : QSqlTableModel(parent,db) {}

bool ItemsModel::select() {
    bool status = QSqlTableModel::select();
    if (!status) {
      return status;
    }
    while (canFetchMore()) {
      fetchMore();
    }
    return status;
}

QVariant ItemsModel::data(const QModelIndex &idx, int role) const {
    return QSqlTableModel::data(idx,role);
}

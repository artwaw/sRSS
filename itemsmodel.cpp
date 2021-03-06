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
    if (!idx.isValid()) { return QVariant(); }
    if ((idx.column()==4)&&(role==Qt::DisplayRole)) {
        QTextDocument doc;
        doc.setHtml(QSqlTableModel::data(idx,role).toString());
        return doc.toPlainText().simplified();
    }
    if (role==Qt::FontRole) {
        return QSqlTableModel::data(idx.siblingAtColumn(14),Qt::DisplayRole).toBool();
    }
    return QSqlTableModel::data(idx,role);
}

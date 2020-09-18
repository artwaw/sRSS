#include "itemdelegate.h"

ItemDelegate::ItemDelegate(QObject *parent) : QStyledItemDelegate(parent) {}

void ItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QStyleOptionViewItem opt=option;
    opt.font.setBold(!index.data(Qt::FontRole).toBool());
    QStyledItemDelegate::paint(painter,opt,index);
}

#include "channeldelegate.h"

ChannelDelegate::ChannelDelegate(QObject *parent) : QStyledItemDelegate(parent) {}

void ChannelDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    if (index.data(Qt::DecorationRole).isValid()) {
        painter->save();
        painter->drawImage(option.rect.x(),option.rect.y(),QImage::fromData(index.data(Qt::DecorationRole).toByteArray()).scaledToHeight(option.rect.height()));
        painter->restore();
        return;
    }
    QStyledItemDelegate::paint(painter,option,index);
}

#include "itemdelegate.h"

ItemDelegate::ItemDelegate(QObject *parent) : QStyledItemDelegate(parent) {}

void ItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QStyleOptionViewItem opt=option;
    opt.font.setBold(!index.data(Qt::FontRole).toBool());
    if (index.column()==13) {
        if (index.data().toBool()) {
            painter->save();
            painter->setRenderHint(QPainter::Antialiasing,true);
            //painter->setPen(Qt::NoPen);
            QPolygon poly;
            int yjump = (option.rect.height()/10);
            int xjump = (option.rect.width()/10);
            poly << QPoint(option.rect.x()+(xjump*5),option.rect.y()+yjump)
                 << QPoint(option.rect.x()+(xjump*5)+yjump,option.rect.y()+(yjump*4))
                 << QPoint(option.rect.x()+(xjump*5)+(yjump*4),option.rect.y()+(yjump*4))
                 << QPoint(option.rect.x()+(xjump*5)+(yjump*2),option.rect.y()+(yjump*6))
                 << QPoint(option.rect.x()+(xjump*5)+(yjump*3),option.rect.y()+(yjump*9))
                 << QPoint(option.rect.x()+(xjump*5),option.rect.y()+(yjump*7))
                 << QPoint(option.rect.x()+(xjump*5)-(yjump*3),option.rect.y()+(yjump*9))
                 << QPoint(option.rect.x()+(xjump*5)-(yjump*2),option.rect.y()+(yjump*6))
                 << QPoint(option.rect.x()+(xjump*5)-(yjump*4),option.rect.y()+(yjump*4))
                 << QPoint(option.rect.x()+(xjump*5)-yjump,option.rect.y()+(yjump*4));
            painter->drawPolygon(poly,Qt::WindingFill);
            painter->restore();
        }
        return;
    }
    QStyledItemDelegate::paint(painter,opt,index);
}

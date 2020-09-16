#include "channelmodel.h"

ChannelModel::ChannelModel(QObject *parent, QSqlDatabase db) : QSqlTableModel(parent,db) {}

bool ChannelModel::select() {
    bool status = QSqlTableModel::select();
    if (!status) {
      return status;
    }
    while (canFetchMore()) {
      fetchMore();
    }
    return status;
}

QVariant ChannelModel::data(const QModelIndex &idx, int role) const {
    if (!idx.isValid()) { return QVariant(); }
    if (idx.column()==2) {
        if (role==Qt::DecorationRole) {
            QString path=settings.value("cache").toString()+"/channels/";
            if (QFile::exists(path+data(idx.siblingAtColumn(0)).toString()+".xcf")) {
                return QPixmap(path+data(idx.siblingAtColumn(0)).toString()+".xcf");
            } else {
                return QPixmap(":/gfx/rss-feed.png").scaled(16,16,Qt::KeepAspectRatio,Qt::SmoothTransformation);
            }
        }
        if (role==Qt::ToolTipRole) {
            return data(idx.siblingAtColumn(4));
        }
    } //title column
    return QSqlTableModel::data(idx,role);
}

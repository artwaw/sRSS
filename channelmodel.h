#ifndef CHANNELMODEL_H
#define CHANNELMODEL_H

#include <QSqlTableModel>
#include <QObject>
#include <QSettings>
#include <QFile>
#include <QPixmap>

class ChannelModel : public QSqlTableModel
{
    Q_OBJECT
public:
    ChannelModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());
    bool select() override;
    QVariant data(const QModelIndex &idx, int role = Qt::DisplayRole) const override;

private:
    QSettings settings;
};

#endif // CHANNELMODEL_H

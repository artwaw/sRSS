#ifndef ITEMSMODEL_H
#define ITEMSMODEL_H

#include <QSqlTableModel>
#include <QObject>

class ItemsModel : public QSqlTableModel
{
    Q_OBJECT
public:
    ItemsModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());
    bool select() override;
    QVariant data(const QModelIndex &idx, int role = Qt::DisplayRole) const override;
};

#endif // ITEMSMODEL_H
#ifndef ITEMSMODEL_H
#define ITEMSMODEL_H

#include <QSqlTableModel>
#include <QObject>
#include <QImage>
#include <QTextDocument>
#include <QDebug>

/*! \class ItemsModel
 * \brief Simple QSqlTableModel descendant providing data for the items table view. */
class ItemsModel : public QSqlTableModel
{
    Q_OBJECT
public:
    /*! \brief Generic constructor.
     * \param parent Optional parent object or nullptr
     * \param db Database instance */
    ItemsModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());

    /*! \brief Simple override of generic select() with lazy model population off
     * \return true if model is successfuly populated */
    bool select() override;

    /*! \brief QSqlTableModel::data() override. Removes HTML from descriptions.
     * \param idx Index of the data from model
     * \param role Role of the data that index is pointing to
     * \return QVariant with data or empty QVariant() if data not available */
    QVariant data(const QModelIndex &idx, int role = Qt::DisplayRole) const override;
};

#endif // ITEMSMODEL_H

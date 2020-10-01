#ifndef ITEMPROXYMODEL_H
#define ITEMPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QObject>

/*! \class ItemProxyModel
 * \brief Simple extension of the QSortFilterProxyModel providing missing flexible filter option. */
class ItemProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    /*! \brief Generic constructor. Calls parent's ctor directly.
     * \param  parent Parent objects pointer or nullptr */
    ItemProxyModel(QObject *parent=nullptr);

protected:
    /*! \brief Method juggling rows on which the model is sorted in the view.
     *
     * By override we provide needed option to display unread only or bookmarked only.
     * The method switches coverage based on the column (inverted read bool or bookmark bool).
     * Acceptance of the data for those two column is set on the criteria above. For any other
     * column parent method is called.
     * \param sourceRow Row in the source model
     * \param sourceParent Index to the parent of the source data in source model
     * \return True if the data indicated by row and index should be included in the model, false otherwise */
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
};

#endif // ITEMPROXYMODEL_H

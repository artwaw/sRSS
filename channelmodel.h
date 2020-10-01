#ifndef CHANNELMODEL_H
#define CHANNELMODEL_H

#include <QSqlTableModel>
#include <QObject>
#include <QSettings>
#include <QFile>
#include <QPixmap>

/*! \class ChannelModel
 * \brief Simple QSqlTableModel descendant providing data for the list of channels. */
class ChannelModel : public QSqlTableModel
{
    Q_OBJECT
public:
    /*! \brief Generic constructor.
     * \param parent Optional parent object or nullptr
     * \param db Database instance */
    ChannelModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());

    /*! \brief Simple override of generic select() with lazy model population off
     * \return true if model is successfuly populated */
    bool select() override;

    /*! \brief QSqlTableModel::data() override. Returns proper tooltips and icons
     * \param idx Index of the data from model
     * \param role Role of the data that index is pointing to
     * \return QVariant with data or empty QVariant() if data not available */
    QVariant data(const QModelIndex &idx, int role = Qt::DisplayRole) const override;

private:
    QSettings settings; //!< Local access to program wide settings
};

#endif // CHANNELMODEL_H

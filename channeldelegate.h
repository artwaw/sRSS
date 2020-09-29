#ifndef CHANNELDELEGATE_H
#define CHANNELDELEGATE_H

#include <QStyledItemDelegate>
#include <QObject>
#include <QPainter>

/*! \class ChannelDelegate
 * \brief Class is responsible for drawing a star when the given item is marked as bookmarked */
class ChannelDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    /*! \brief Generic constructor.
     * \param parent Optional parent object pointer or nullptr */
    ChannelDelegate(QObject *parent=nullptr);

    /*! \brief Method painting actual star depending on the value of the flag
     * \param painter Painter devices on which painting will be done
     * \param option Style option reference for painting process
     * \param index Index of the item that should be painted */
    void paint(QPainter *painter,const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // CHANNELDELEGATE_H

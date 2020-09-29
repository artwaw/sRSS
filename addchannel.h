#ifndef ADDCHANNEL_H
#define ADDCHANNEL_H

#include <QDialog>
#include <QtNetwork>
#include <QAction>
#include <QIcon>

#include <channelparser.h>

namespace Ui {
class addChannel;
}

/*! \class addChannel
 * \brief QDialog offering the user an option to add new RSS channel */
class addChannel : public QDialog
{
    Q_OBJECT

public:
    /*! \brief generic constructor. Inits the view.
     * \param parent Parent widget, defaults to nullptr */
    explicit addChannel(QWidget *parent = nullptr);

    /*! \brief generic destructor */
    ~addChannel();

    /*! \brief Method to check for the last HTTP status code
     * \return Last HTTP status code */
    int lastStatus();

    /*! \brief Getter method for items from the freshly added channel.
     * \return Vector with ChannelParser::itemInfo data or empty QVector
     * \see ChannelParser::itemInfo */
    QVector<ChannelParser::itemInfo> getChannelItems() const;

    /*! \brief Getter method for details of freshly added channel.
     * \return ChannelParser::channelInfo data set
     * \see ChannelParser::channelInfo */
    ChannelParser::channelInfo getChannelInfo() const;

private:
    Ui::addChannel *ui; //!< user interface pointer
    QNetworkAccessManager *manager; //!< NetworkAccessManager instance used to get the data of the channel
    int status; //!< Holds last received HTTP status code
    ChannelParser parser; //!< Instance of the parser to extract data from XML
    QVector<ChannelParser::itemInfo> items; //!< Vector holding last retrieved items from the channel. \see ChannelParser::itemInfo
    ChannelParser::channelInfo chinfo; //!< Structure with channel info data. \see ChannelParser::channelInfo

private slots:
    /*! \brief Method processing network replies when adding the channel.
     * \param reply Pointer to QNetworkReply object holding the reply data */
    void finished(QNetworkReply *reply);

    /*! \brief Method sending GET request asking for RSS channel info and data */
    void add();

    /*! \brief Helper method making sure that appropriate buttons are disabled until input has data in more or less correct format */
    void verifyBtns();
};

#endif // ADDCHANNEL_H

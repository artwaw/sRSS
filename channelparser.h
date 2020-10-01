#ifndef CHANNELPARSER_H
#define CHANNELPARSER_H

#include <QDebug>
#include <QObject>
#include <QtXml>
#include <QUrl>
#include <QLocale>
#include <QDateTime>

/*! \class ChannelParser
 * \brief Class parsing the XML RSS info into ready to use structures */
class ChannelParser : public QObject
{
    Q_OBJECT

public:

    /*! \struct img
     * \brief Substructure of ChannelParser::channelInfo with details of image (if supplied)
     * \see ChannelParser::channelInfo */
    struct img {
        QUrl url; //!< URL of the image
        QString title; //!< Title of the image (as in HTML <title=>
        QUrl link; //!< Target link of an image
        int width; //!< Width of the image
        int height; //!< Hight of the image
    };

    /*! \struct ChannelParser::channelInfo
     * \brief General info about the channel being added as parsed from XML (fields as per RFC) */
    struct channelInfo {
        QString title; //!< Title of the channel
        QUrl link; //!< Link to the channel XML source
        QString description; //!< Description of the channel
        QLocale language; //!< Language code of channel
        QString copyright; //!< Copyright mark
        QString managingDirector; //!< Contact info of the managing director of the channel
        QString webMaster; //!< Webmaster info
        QDateTime pubDate; //!< Publication date
        QDateTime lastBuildDate; //!< Timestamp of the last build of data info (from the server)
        QString category; //!< Category info
        QString generator; //!< Generator info
        QUrl docs; //!< Link to the documentation
        int ttl; //!< Time to live mark
        img image; //!< Structure describing the image associated with the channel
        QVector<bool> skipHours; //!< Hours of a day when channel will not be rebuild
        QVector<bool> skipDays; //!< Days of the week when channel will not be rebuld
    };

    /*! \struct encl
     * \brief Enclosure structure as a possible part of item returned for the given RSS channel
     * \see ChannelParser::itemInfo */
    struct encl {
        QUrl url; //!< URL of the items enclosure
        int length; //!< Length in bytes
        QString type; //!< Mime type
    };

    /*! \struct ChannelParser::itemInfo
     * \brief Representaion of a single item from a list returned for given RSS channel */
    struct itemInfo {
        QString title; //!< Display title
        QUrl link; //!< Link to the webpage with the entry
        QString description; //!< Description as per RFC
        QUrl author; //!< Contanct info for entry author
        QUrl comments; //!< Link to the comments for the item given
        QString guid; //!< Unique guid of an item
        encl enclosure; //!< Enclosure structure, if present
        QDateTime pubDate; //!< Publication timestamp
    };

    /*! \brief Generic constructor. Calls init() to set initial values
     * \param parent Optional parent object or nullptr */
    explicit ChannelParser(QObject *parent = nullptr);

    /*! \brief Method parsing data raw XML data returned from the server
     * \param data Raw byte array with XML data
     * \return true on success, false on parsing error
     * \see lastError() */
    bool parseData(QByteArray &data);

    /*! \brief Clears internal data structures from any data */
    void clear();

    /*! \brief Inits internal data structures with default (empty or predefined) data */
    void init();

    /*! \brief Getter method for parsed info about RSS channel
     * \return Strucutre filled with data
     * \see ChannelParser::channelInfo() */
    channelInfo getChannelInfo() const;

    /*! \brief Getter method returning list of entries in the RSS channel
     * \return QVector of entries or empty QVector() if no items are published
     * \see ChannelParser::itemInfo() */
    QVector<itemInfo> getItemsInfo() const;

    /*! \brief Human readable info about possible errors
     * \return Unicode error string or empty QString() */
    QString lastError() const;

signals:
    /*! \brief Signal emitted when parsing of the server reply is complete
     * \param result True upon success, false if parsing failed */
    void channelParsed(bool result);

private:
    channelInfo channel; //!< Internal strucutre holding parsed channel info. \see ChannelParser::channelInfo() \see ChannelParser::getChannelInfo()
    QVector<itemInfo> items; //!< Internal strucutre holding published items for the channel. \see ChannelParser::itemInfo \see ChannelParser::getItemsInfo()
    QString errorStr; //!< Internal member holding last error string.
};

#endif // CHANNELPARSER_H

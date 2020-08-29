#ifndef CHANNELPARSER_H
#define CHANNELPARSER_H

#include <QDebug>
#include <QObject>
#include <QtXml>
#include <QUrl>
#include <QLocale>
#include <QDateTime>

class ChannelParser : public QObject
{
    Q_OBJECT

public:

    struct img {
        QUrl url;
        QString title;
        QUrl link;
        int width;
        int height;
    };

    struct channelInfo {
        QString title;
        QUrl link;
        QString description;
        QLocale language;
        QString copyright;
        QString managingDirector;
        QString webMaster;
        QDateTime pubDate;
        QDateTime lastBuildDate;
        QString category;
        QString generator;
        QUrl docs;
        int ttl;
        img image;
        QVector<bool> skipHours;
        QVector<bool> skipDays;
    };

    struct encl {
        QUrl url;
        int length;
        QString type;
    };

    struct itemInfo {
        QString title;
        QUrl link;
        QString description;
        QUrl author;
        QUrl comments;
        QString guid;
        encl enclosure;
        QDateTime pubDate;
    };

    explicit ChannelParser(QObject *parent = nullptr);
    bool parseData(QByteArray &data);
    void clear();
    void init();
    channelInfo getChannelInfo() const;
    QVector<itemInfo> getItemsInfo() const;
    QString lastError() const;

signals:
    void channelParsed(bool result);

private:
    channelInfo channel;
    QVector<itemInfo> items;
    QString errorStr;
};

#endif // CHANNELPARSER_H

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

class addChannel : public QDialog
{
    Q_OBJECT

public:
    explicit addChannel(QWidget *parent = nullptr);
    ~addChannel();
    int lastStatus();
    QVector<ChannelParser::itemInfo> getChannelItems() const;
    ChannelParser::channelInfo getChannelInfo() const;

private:
    Ui::addChannel *ui;
    QNetworkAccessManager *manager;
    int status;
    ChannelParser parser;
    QVector<ChannelParser::itemInfo> items;
    ChannelParser::channelInfo chinfo;

private slots:
    void finished(QNetworkReply *reply);
    void add();
    void verifyBtns();
};

#endif // ADDCHANNEL_H

#include "channelparser.h"

ChannelParser::ChannelParser(QObject *parent) : QObject(parent)
{
    init();
}

void ChannelParser::clear() {
    init();
}

void ChannelParser::init() {
    channel.title.clear();
    channel.link.clear();
    channel.description.clear();
    channel.language=QLocale();
    channel.copyright.clear();
    channel.managingDirector.clear();
    channel.webMaster.clear();
    channel.pubDate = QDateTime();
    channel.lastBuildDate = QDateTime();
    channel.category.clear();
    channel.generator.clear();
    channel.docs.clear();
    channel.ttl=-1;
    channel.image.url.clear();
    channel.image.title.clear();
    channel.image.link.clear();
    channel.image.width=88;
    channel.image.height=31;
    channel.skipHours.clear();
    channel.skipHours.reserve(24);
    channel.skipHours.fill(false);
    channel.skipDays.clear();
    channel.skipDays.reserve(7);
    channel.skipDays.fill(false);
    items.clear();
    errorStr=tr("No error");
}

ChannelParser::channelInfo ChannelParser::getChannelInfo() const {
    return channel;
}

QVector<ChannelParser::itemInfo> ChannelParser::getItemsInfo() const {
    return items;
}

bool ChannelParser::parseData(QByteArray &data) {
    clear();
    QDomDocument xml;
    xml.setContent(data);
    QDomElement ele=xml.documentElement();
    QDomElement hook=ele.firstChildElement("channel");
    channel.title = hook.firstChildElement("title").toElement().text();
    channel.link.setUrl(hook.firstChildElement("atom:link").attribute("href"));
    channel.description = hook.firstChildElement("description").toElement().text();
    if (channel.title.isEmpty()||channel.link.isEmpty()||channel.description.isEmpty()) {
        errorStr = "Missing title, link or description in returned data. Response from server not valid.";
        channelParsed(false);
        return false;
    }
    channel.language = QLocale(hook.firstChildElement("language").toElement().text());
    channel.copyright = hook.firstChildElement("copyright").toElement().text();
    channel.managingDirector = hook.firstChildElement("managingDirector").toElement().text();
    channel.webMaster = hook.firstChildElement("webMaster").toElement().text();
    channel.pubDate = QDateTime::fromString(hook.firstChildElement("pubDate").toElement().text());
    channel.lastBuildDate = QDateTime::fromString(hook.firstChildElement("lastBuildDate").toElement().text());
    channel.generator = hook.firstChildElement("generator").toElement().text();
    channel.docs.setUrl(hook.firstChildElement("docs").toElement().text());
    channel.ttl = hook.firstChildElement("ttl").toElement().text().toInt();
    QDomNode item;
    if (!hook.firstChildElement("skipHours").toElement().text().isEmpty()) {
        item = hook.firstChildElement("hour");
        while (!item.nextSibling().isNull()) {
            channel.skipHours[item.toElement().text().toInt()]=true;
        }
    }
    if (!hook.firstChildElement("skipDays").toElement().text().isEmpty()) {
        item = hook.firstChildElement("day");
        QMap<QString,int> days; days.insert("Monday",1); days.insert("Tuesday",2); days.insert("Wednesday",3); days.insert("Thursday",4); days.insert("Friday",5); days.insert("Saturday",6); days.insert("Sunday",7);
        while (!item.nextSibling().isNull()) {
            channel.skipDays[days.value(item.toElement().text())]=true;
        }
    }
    item = hook.firstChildElement("item");
    itemInfo info;
    while (!item.nextSibling().isNull()) {
        info.title = item.firstChildElement("title").toElement().text();
        info.link.setUrl(item.firstChildElement("link").toElement().text());
        info.description = item.firstChildElement("description").toElement().text();
        info.pubDate = QDateTime::fromString(item.firstChildElement("pubDate").toElement().text());
        items.append(info);
        item = item.nextSibling();
    }
    channelParsed(true);
    return true;
}

QString ChannelParser::lastError() const {
    return errorStr;
}

/*
This file is part of SmallRSS (shortened to sRSS).
sRSS is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
sRSS is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with sRSS.  If not, see <https://www.gnu.org/licenses/>.
SPDX: GPL-3.0-or-later
*/
#include "sfeedupdater.h"

sFeedUpdater::sFeedUpdater(QObject *parent) : QObject(parent)
{
    QSettings settings;
    jar = new QNetworkCookieJar(this);
    manager = new QNetworkAccessManager(this);
    timerID = 0;
    startTimer(settings.value("autointerval").toInt());
}

void sFeedUpdater::startTimer(const int interval) {
    if (interval==0) { return; }
    timerID = QObject::startTimer(interval,Qt::CoarseTimer);
    connect(manager,&QNetworkAccessManager::finished,this,&sFeedUpdater::finished);
}

void sFeedUpdater::stopTimer() {
    QObject::killTimer(timerID);
}

void sFeedUpdater::timeout() {
    if (timerID==0) { return; }
    auto db = QSqlDatabase::database();
    QSqlQuery query;
    query.exec("select 'link' from sIndex;");
    while (query.next()) {
        qDebug() << query.value(0).toString();
        pollOne(query.value(0).toString());
    }
}

void sFeedUpdater::finished(QNetworkReply *reply) {
    int ecode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << reply->url().toString() << ecode;
    if (ecode==200) {
        processXML(reply->readAll());
        newData();
    }
    reply->deleteLater();
}

void sFeedUpdater::processXML(QByteArray data) {
    qDebug() << data;
}

void sFeedUpdater::pollOne(QString url) {
    QUrl uurl(url);
    manager->get(QNetworkRequest(uurl));
}

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
#ifndef SFEEDUPDATER_H
#define SFEEDUPDATER_H

#include <QtCore/QObject>
#include <QtCore/qglobal.h>
#include <QtNetwork>
#include <QtSql>

class sFeedUpdater : public QObject
{
    Q_OBJECT
public:
    explicit sFeedUpdater(QObject *parent = nullptr);

public slots:
    void startTimer(const int interval);
    void stopTimer();
    void pollOne(QString url);


private slots:
    void finished(QNetworkReply *reply);
    void timeout();

private:
   QNetworkAccessManager *manager;
   int timerID;
   QNetworkCookieJar *jar;

   void processXML(QByteArray data);

signals:
   void newData();

};

#endif // SFEEDUPDATER_H

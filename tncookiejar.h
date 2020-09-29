#ifndef COOKIEJAR_H
#define COOKIEJAR_H

#include <QNetworkCookieJar>
#include <QNetworkCookie>
#include <QtCore>
#include <QCborValue>
#include <QDateTime>

class TNCookieJar : public QNetworkCookieJar
{
    Q_OBJECT
public:
    TNCookieJar(QObject *parent=nullptr, const QString path=QString());
    bool insertCookie(const QNetworkCookie &cookie);
    bool deleteCookie(const QNetworkCookie &cookie);
    bool updateCookie(const QNetworkCookie &cookie);
    void deleteCookies();

private:
    QString apath;

    bool saveCookie(const QNetworkCookie &cookie) const;
    QNetworkCookie loadCookie(const QString fname) const;
    QFileInfoList listCookies() const;
};

#endif // COOKIEJAR_H

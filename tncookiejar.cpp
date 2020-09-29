#include "tncookiejar.h"

TNCookieJar::TNCookieJar(QObject *parent, const QString path) : QNetworkCookieJar(parent) {
    apath = path;
    QDir dir(apath);
    if (!dir.exists()) {
        dir.mkpath(dir.absolutePath());
    }
    QFileInfoList files=listCookies();
    for (auto x=0;x<files.size();++x) {
        QNetworkCookieJar::insertCookie(loadCookie(files.at(x).absoluteFilePath()));
        qDebug() << files.at(x).absoluteFilePath();
    }
}

QNetworkCookie TNCookieJar::loadCookie(const QString fname) const {
    QFile file(fname);
    if (!file.open(QIODevice::ReadOnly)) {
        return QNetworkCookie();
    }
    QJsonObject doc(QCborValue::fromCbor(file.readAll()).toMap().toJsonObject());
    file.close();
    QNetworkCookie cookie;
    cookie.setDomain(doc.value("domain").toString());
    cookie.setExpirationDate(QDateTime::fromString(doc.value("expiration").toString()));
    cookie.setHttpOnly(doc.value("http").toBool());
    cookie.setName(doc.value("name").toVariant().toByteArray());
    cookie.setPath(doc.value("path").toString());
    cookie.setSecure(doc.value("secure").toBool());
    cookie.setValue(doc.value("value").toVariant().toByteArray());
    return cookie;
}

bool TNCookieJar::saveCookie(const QNetworkCookie &cookie) const {
    QJsonObject doc;
    doc.insert("domain",cookie.domain());
    doc.insert("expiration",cookie.expirationDate().toString(Qt::ISODate));
    doc.insert("http",cookie.isHttpOnly());
    doc.insert("name",QJsonValue::fromVariant(QVariant(cookie.name())));
    doc.insert("path",cookie.path());
    doc.insert("secure",cookie.isSecure());
    doc.insert("value",QJsonValue::fromVariant(QVariant(cookie.value())));
    QDir dir;
    if (!cookie.domain().isEmpty()) {
        dir.setPath(apath+"/"+cookie.domain().replace('.','_'));
        if (!dir.exists()) {
            bool res = dir.mkpath(dir.absolutePath());
            qDebug() << res << dir.absolutePath();
        }
    } else {
        dir.setPath(apath);
    }
    QFile file(dir.absolutePath()+"/"+cookie.name()+".ck");
    qDebug() << file.fileName() << dir.absolutePath();
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Open file to save cookie failed:" << cookie.name();
        return false;
    }
    bool ret=file.write(QCborValue::fromJsonValue(doc).toCbor());
    qDebug() << ret << file.errorString() << file.error();
    file.close();
    return ret;
}

bool TNCookieJar::insertCookie(const QNetworkCookie &cookie) {
    qDebug() << "attempting to write cookie:" << cookie.name() << cookie.domain();
    saveCookie(cookie);
    return QNetworkCookieJar::insertCookie(cookie);
}

bool TNCookieJar::updateCookie(const QNetworkCookie &cookie) {
    qDebug() << "updating the cookie:" << cookie.name() << cookie.domain();
    saveCookie(cookie);
    return QNetworkCookieJar::updateCookie(cookie);
}

bool TNCookieJar::deleteCookie(const QNetworkCookie &cookie) {

    return QNetworkCookieJar::deleteCookie(cookie);
}

void TNCookieJar::deleteCookies() {
    QDir dir(apath);
    dir.removeRecursively();
    dir.mkpath(dir.absolutePath());
    QList<QNetworkCookie> list = QNetworkCookieJar::allCookies();
    for (auto x=0;x<list.size();++x) {
        QNetworkCookieJar::deleteCookie(list.at(x));
    }
}

QFileInfoList TNCookieJar::listCookies() const {
    QDir dir(apath);
    QFileInfoList domains = dir.entryInfoList(QDir::Dirs);
    qDebug() << domains.size();
    QFileInfoList res = dir.entryInfoList(QStringList("*.ck"),QDir::Files);
    for (auto x=0;x<domains.size();++x) {
        dir.setPath(domains.at(x).absolutePath());
        res.append(dir.entryInfoList(QStringList("*.ck"),QDir::Files));
    }
    qDebug() << res.size();
    return res;
}

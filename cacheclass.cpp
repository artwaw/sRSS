#include "cacheclass.h"

CacheClass::CacheClass(QObject *parent) : QObject(parent)
{
    QSettings settings;
    jar = new TNCookieJar(this,QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/cookies");
    cache = new QNetworkDiskCache(this);
    cache->setCacheDirectory(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
}

bool CacheClass::isJarValid() const {
    return jar!=nullptr;
}

bool CacheClass::isCacheValid() const {
    return !cache->cacheDirectory().isEmpty();
}

TNCookieJar* CacheClass::getCookieJar() const {
    return jar;
}

QNetworkDiskCache* CacheClass::getCache() const {
    return cache;
}

void CacheClass::clearCache() {
    cache->clear();
}

void CacheClass::clearCookies() {
    jar->deleteCookies();
}


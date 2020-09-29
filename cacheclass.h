#ifndef CACHECLASS_H
#define CACHECLASS_H

#include <QObject>
#include <QtNetwork>
#include <tncookiejar.h>

/*! \brief Class holding custom cookie jar and disk cache instance that can be shared across the application. */
class CacheClass : public QObject
{
    Q_OBJECT
public:
    /*! \brief Generic constructor. Also, loads existing cookies from disk if there are any.
     * \param parent Parent object or nullptr */
    explicit CacheClass(QObject *parent = nullptr);

    /*! \brief Method returning pointer to internal cookie jar object.
     * \return Cookie jar pointer
     * \see TMCookieJar() */
    TNCookieJar* getCookieJar() const;

    /*! \brief Method returning pointer to internal disk cache object.
     * \return Pointer to generic disk cache */
    QNetworkDiskCache* getCache() const;

    /*! \brief Checks if cookie jar is valid (i.e. not nullptr)
     * \return Bool as the outcome of a check */
    bool isJarValid() const;

    /*! \brief Checks if caches is valid and set.
     * \return Bool - true if the object is initiated and has working folder assigned */
    bool isCacheValid() const;

private:
    TNCookieJar *jar; //!< Internal, shared cookie jar object
    QNetworkDiskCache *cache; //!< Internal, shared network disk cache

public slots:
    /*! \brief Slot clearing the cache from disk. */
    void clearCache();

    /*! \brief Slot removing all saved cookies both from the disk and from the memory */
    void clearCookies();
};

#endif // CACHECLASS_H

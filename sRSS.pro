QT       += core gui xml network sql printsupport webkitwidgets webkit

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addchannel.cpp \
    cacheclass.cpp \
    channeldelegate.cpp \
    channeleditdialog.cpp \
    channelmodel.cpp \
    channelparser.cpp \
    channelremovedialog.cpp \
    itemdelegate.cpp \
    itemproxymodel.cpp \
    itemsmodel.cpp \
    main.cpp \
    mainwindow.cpp \
    optiondlg.cpp \
    sfeedupdater.cpp \
    tncookiejar.cpp

HEADERS += \
    addchannel.h \
    cacheclass.h \
    channeldelegate.h \
    channeleditdialog.h \
    channelmodel.h \
    channelparser.h \
    channelremovedialog.h \
    itemdelegate.h \
    itemproxymodel.h \
    itemsmodel.h \
    mainwindow.h \
    optiondlg.h \
    sfeedupdater.h \
    tncookiejar.h

FORMS += \
    addchannel.ui \
    channeleditdialog.ui \
    channelremovedialog.ui \
    mainwindow.ui \
    optiondlg.ui

TRANSLATIONS += \
    sRSS_en_GB.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    COPYING \
    GNU GENERAL PUBLIC LICENSE \
    README.md \
    channelschema.sql \
    itemschema.sql \
    lic_template

RESOURCES += \
    res.qrc

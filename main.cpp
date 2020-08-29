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

#include "mainwindow.h"

#include <QtCore/qglobal.h>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QApplication>
#else
#include <QtGui/QApplication>
#endif

#include <QtPlugin>
#include <QTranslator>
#include <QLibraryInfo>

int main(int argc, char *argv[])
{
    qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "1");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setApplicationVersion("0.1");
    QCoreApplication::setOrganizationName("TrollNet");
    QCoreApplication::setOrganizationDomain("trollnet.com.pl");
    QCoreApplication::setApplicationName("sRSS");
    QApplication a(argc, argv);
    QCoreApplication::addLibraryPath("/libs");
    MainWindow w;
    w.setAttribute(Qt::WA_AlwaysShowToolTips);
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),":/lang");
    a.installTranslator(&qtTranslator);
    QTranslator myappTranslator;
    myappTranslator.load("sRSS_" + QLocale::system().name(),":/lang");
    a.installTranslator(&myappTranslator);
    w.show();
    return a.exec();
}

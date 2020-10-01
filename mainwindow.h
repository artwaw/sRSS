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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <qglobal.h>
#include <QtWidgets>
#include <QtGui>
#include <QtSql>
#include <QWebView>

#include "optiondlg.h"
#include "addchannel.h"
#include "channelmodel.h"
#include "itemsmodel.h"
#include "itemdelegate.h"
#include "itemproxymodel.h"
#include "channeldelegate.h"
#include "cacheclass.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/*! \class MainWindow
 * \brief Main thread class holding UI and main logic of the program */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /*! \brief Generic constructor that inits the program.
     *
     * This ctor handles database initialisation, first run autodetection, inits models and views.
     * This ctor is also resposible for network cache and cookie jar initialisation.
     * \param parent Parent widget pointer or nullptr
     * \see zeroconf()
     * \see firstrun
     * \see initDB()
     * \see loadSettings()
     * \see initItems()
     * \see initChannels()
     * \see cache
     * \see manager */
    MainWindow(QWidget *parent = nullptr);

    /*! \brief Generic destructor.
     *
     * The only thing destructor does is to destroy the UI. All closing logic is handled in closeEvent()
     * \see closeEvent() */
    ~MainWindow() override;

protected:
    /*! \brief Event processed upon closing the main thread.
     *
     * In this event the state of the main window is being saved.
     * \param event Generic QCloseEvent */
    void closeEvent(QCloseEvent *event) override;

private slots:
//    void updateItems();
    void addChannel();
//    void editChannel();
//    void removeChannel();
    void starItem(const QModelIndex &idx);
    void clearCache();
//    void moveCache(QString target);
//    void clearDB();
//    void moveDB(QString target);
    void itemAct(QModelIndex idx);
    void restoreHeaders();
    void onItemLoad(bool ecode);
#ifndef QT_NO_CONTEXT_MENU
    void setupItemHeaderContextMenu();
    void setupChannelContextMenu();
    void itemHeaderMenuRequested(QPoint pos);
    void channelContextMenuRequested(QPoint pos);
#endif
    [[ noreturn ]] void closeApp();
//    void newDB();
//    void openDB();

private:
    Ui::MainWindow *ui;
    QSettings settings;
    ChannelModel *channels;
    ItemProxyModel *itemproxy;
    ItemsModel *items;
    QMenu *itemHeaderContextMenu;
    QMenu *channelContextMenu;
    bool firstrun;
    CacheClass *cache;
    QNetworkAccessManager *manager;

    void zeroconf();
    void loadSettings();
    void saveSettings();
    bool initDB();
    void initChannels();
    void initItems();
    void criticalMsg(const QString reason, const QString msg);
    [[ noreturn ]] void fail(const qint32 excode, const QString reason, const QString msg);
};
#endif // MAINWINDOW_H

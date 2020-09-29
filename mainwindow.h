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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
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

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
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    if (!settings.allKeys().contains("dbfile")||(!QFile::exists(settings.value("dbfile").toString()))) {
        zeroconf();
        firstrun = true;
    } else {
        initDB();
        firstrun = false;
    }
    loadSettings();
    connect(ui->action_Quit,&QAction::triggered,this,&MainWindow::closeApp);
    connect(ui->action_Add,&QAction::triggered,this,&MainWindow::addChannel);
    connect(ui->action_Edit_channel,&QAction::triggered,this,&MainWindow::editChannel);
    initItems();
    initChannels();
    connect(ui->channelListView->selectionModel(),&QItemSelectionModel::selectionChanged,this,&MainWindow::updateChannelActions);
    connect(ui->channelListView->selectionModel(),&QItemSelectionModel::modelChanged,this,&MainWindow::updateChannelActions);
    connect(ui->channelListView,&QListView::clicked,this,&MainWindow::updateChannelActions);
    QProgressBar *statusProgress = new QProgressBar(this);
    statusProgress->setMaximum(100);
    statusProgress->setMinimum(0);
    connect(ui->webview,&QWebView::loadProgress,statusProgress,&QProgressBar::setValue);
    connect(ui->webview,&QWebView::loadFinished,statusProgress,&QProgressBar::reset);
    ui->statusbar->addPermanentWidget(statusProgress);
    cache = new CacheClass(this);
    manager = new QNetworkAccessManager(this);
    manager->setCookieJar(cache->getCookieJar());
    manager->setCache(cache->getCache());
    ui->webview->page()->setNetworkAccessManager(manager);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::fail(const qint32 excode, const QString reason, const QString msg) {
    QMessageBox::critical(this,reason,msg);
    exit(excode);
}

void MainWindow::criticalMsg(const QString reason, const QString msg) {
    QMessageBox::critical(this,reason,msg);
}

bool MainWindow::initDB() {
    auto db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(settings.value("dbfile").toString());
    return db.open();
}

void MainWindow::zeroconf() {
    QDir path(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    if (!path.exists()) {
        path.mkpath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    }
    settings.setValue("dbfile",QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/sRSS.srss");
    settings.setValue("cache",QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
    settings.setValue("envelopes",true);
    settings.setValue("downloads",QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));
    settings.setValue("askdownload",false);
    settings.setValue("autocheck",true);
    settings.setValue("autointerval",15);
    settings.setValue("honor",true);
    saveSettings();
    if (!initDB()) {
        criticalMsg(tr("Error opening file"),tr("There was an error creating data file using default filename and path. Please go to settings and verify the data file location."));
        return;
    }
    QSqlQuery query;
    query.exec("create table sIndex(id integer primary key, rgroup integer, title text not null, link text not null unique, description text not null, lang text, copyright text, mandir text, webmaster text, pubdate text, lastbuild text, category text, generator text, docs text, ttl integer, imgurl text, imgwidth integer, imgheight integer, skiphrs text, skipdays text);");
    if (query.lastError().type()!=QSqlError::NoError) {
        fail(1,tr("Error while data file init"),tr("Something went wrong while preparing the data file. Please forward the below info to srss@trollnet.com.pl: sIndex - ")+query.lastError().text());
    }
    query.exec("insert into sIndex(title,link,description) values('Unread','unread','All unread items');");
    query.exec("insert into sIndex(title,link,description) values('Bookmarked','bookmarked','All bookmarked items');");
    query.exec("create table items(id integer primary key, origin integer not null, title text, link text, description text, author text, category text, comments text, guid text, enclink text, enclen integer, enctype text, pubdate text, star bool, read bool);");
    if (query.lastError().type()!=QSqlError::NoError) {
        fail(1,tr("Error while data file init"),tr("Something went wrong while preparing the data file. Please forward the below info to srss@trollnet.com.pl: Items - ")+query.lastError().text());
    }
}

void MainWindow::loadSettings() {
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("state").toByteArray());
}

void MainWindow::saveSettings() {
    settings.setValue("geometry",saveGeometry());
    settings.setValue("state",saveState());
    settings.beginWriteArray("itemsView",ui->itemsView->horizontalHeader()->count());
    for (auto x=0;x<ui->itemsView->horizontalHeader()->count();++x) {
        settings.setArrayIndex(x);
        settings.setValue("hidden",ui->itemsView->isColumnHidden(x));
        settings.setValue("width",ui->itemsView->columnWidth(x));
    }
    settings.endArray();
    settings.sync();
}

void MainWindow::initChannels() {
    channels = new ChannelModel(this,QSqlDatabase::database(QSqlDatabase::defaultConnection,true));
    channels->setTable("sIndex");
    ui->channelListView->setModel(channels);
    ui->channelListView->setModelColumn(2);
    ui->channelListView->setUniformItemSizes(true);
    ui->channelListView->setMovement(QListView::Static);
    ui->channelListView->setIconSize(QSize(16,16));
    channels->select();
    ChannelDelegate *chdelegate = new ChannelDelegate(this);
    ui->channelListView->setItemDelegate(chdelegate);
#ifndef QT_NO_CONTEXT_MENU
    ui->channelListView->setContextMenuPolicy(Qt::CustomContextMenu);
    setupChannelContextMenu();
#endif
}

void MainWindow::closeEvent(QCloseEvent *event) {
    saveSettings();
    event->accept();
}

void MainWindow::closeApp() {
    saveSettings();
    exit(0);
}

void MainWindow::clearCache() {
    QDir root(settings.value("cache").toString());
    if (QMessageBox::question(this,tr("Doublecheck needed"),tr("Are you sure you want to remove all cached items?"),QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes) {
        root.removeRecursively();
    }
}

void MainWindow::addChannel() {
    class addChannel addDialog;
    if (addDialog.exec()==QDialog::Rejected) { return;}
    ChannelParser::channelInfo info=addDialog.getChannelInfo();
    QVector<ChannelParser::itemInfo> chinfo=addDialog.getChannelItems();
    QSqlRecord rec = channels->record();
    rec.setGenerated("id",false);
    rec.setValue("title",info.title);
    rec.setValue("link",info.link.toString());
    rec.setValue("description",info.description);
    rec.setValue("lang",info.language.name());
    rec.setValue("copyright",info.copyright);
    rec.setValue("mandir",info.managingDirector);
    rec.setValue("webmaster",info.webMaster);
    rec.setValue("pubdate",info.pubDate.toString(Qt::ISODate));
    rec.setValue("lastbuild",info.lastBuildDate.toString(Qt::ISODate));
    rec.setValue("category",info.category);
    rec.setValue("generator",info.generator);
    rec.setValue("docs",info.docs.toString());
    rec.setValue("ttl",info.ttl);
    channels->insertRecord(-1,rec);
    int id = channels->record(channels->rowCount()-1).value(0).toInt();
    channels->select();
    rec = items->record();
    if (chinfo.isEmpty()) { return; }
    for (auto x=0;x<chinfo.size();++x) {
        rec.setValue("origin",id);
        rec.setValue("title",chinfo.at(x).title);
        rec.setValue("link",chinfo.at(x).link.toString());
        rec.setValue("description",chinfo.at(x).description);
        rec.setValue("author",chinfo.at(x).author.toString());
        rec.setValue("comments",chinfo.at(x).comments.toString());
        rec.setValue("guid",chinfo.at(x).guid);
        rec.setValue("encurl",chinfo.at(x).enclosure.url.toString());
        rec.setValue("enclen",chinfo.at(x).enclosure.length);
        rec.setValue("enctype",chinfo.at(x).enclosure.type);
        rec.setValue("pundate",chinfo.at(x).pubDate.toString(Qt::ISODate));
        rec.setValue("star",false);
        rec.setValue("read",false);
        items->insertRecord(-1,rec);
        rec.clearValues();
    }
    items->select();
}

void MainWindow::initItems() {
    items = new ItemsModel(this,QSqlDatabase::database(QSqlDatabase::defaultConnection,true));
    items->setTable("items");
    items->setHeaderData(2,Qt::Horizontal,QVariant(tr("Title")));
    items->setHeaderData(4,Qt::Horizontal,QVariant(tr("Description")));
    items->setHeaderData(5,Qt::Horizontal,QVariant(tr("Author")));
    items->setHeaderData(12,Qt::Horizontal,QVariant(tr("Publication date")));
    items->setHeaderData(13,Qt::Horizontal,QVariant(tr("Bookmark")));
    items->select();
    itemproxy = new ItemProxyModel(this);
    itemproxy->setSourceModel(items);
    ui->itemsView->setModel(itemproxy);
    connect(ui->itemsView->horizontalHeader(),&QHeaderView::customContextMenuRequested,this,&MainWindow::itemHeaderMenuRequested);
    connect(ui->channelListView,&QListView::clicked,this,&MainWindow::itemAct);
    items->select();
    restoreHeaders();
    if (!firstrun) {
        int size = settings.beginReadArray("itemsView");
        for (auto x=0;x<size;++x) {
            settings.setArrayIndex(x);
            ui->itemsView->setColumnHidden(x,settings.value("hidden").toBool());
            ui->itemsView->setColumnWidth(x,settings.value("width").toInt());
        }
        settings.endArray();
    }
    connect(ui->itemsView,&QTableView::clicked,this,&MainWindow::starItem);
    connect(ui->webview,&QWebView::loadFinished,this,&MainWindow::onItemLoad);
    ItemDelegate *itemDelegate = new ItemDelegate(this);
    ui->itemsView->setItemDelegate(itemDelegate);
#ifndef QT_NO_CONTEXT_MENU
    ui->itemsView->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    setupItemHeaderContextMenu();
#endif
}

#ifndef QT_NO_CONTEXT_MENU
void MainWindow::setupItemHeaderContextMenu() {
    itemHeaderContextMenu = new QMenu(this);
    QAction *titleBox = new QAction(tr("Title"));
    titleBox->setCheckable(true);
    titleBox->setChecked(!ui->itemsView->isColumnHidden(2));
    itemHeaderContextMenu->addAction(titleBox);
    QAction *descBox = new QAction(tr("Description"));
    descBox->setCheckable(true);
    descBox->setChecked(!ui->itemsView->isColumnHidden(4));
    itemHeaderContextMenu->addAction(descBox);
    QAction *authorBox = new QAction(tr("Author"));
    authorBox->setCheckable(true);
    authorBox->setChecked(!ui->itemsView->isColumnHidden(5));
    itemHeaderContextMenu->addAction(authorBox);
    QAction *dateBox = new QAction(tr("Publication date"));
    dateBox->setCheckable(true);
    dateBox->setChecked(!ui->itemsView->isColumnHidden(12));
    itemHeaderContextMenu->addAction(dateBox);
    QAction *bookmarkBox = new QAction(tr("Bookmarks"));
    bookmarkBox->setCheckable(true);
    bookmarkBox->setChecked(!ui->itemsView->isColumnHidden(13));
    itemHeaderContextMenu->addAction(bookmarkBox);
    itemHeaderContextMenu->addSeparator();
    QAction *restoreHeader = new QAction(tr("Restore columns"));
    connect(restoreHeader,&QAction::triggered,this,&MainWindow::restoreHeaders);
    itemHeaderContextMenu->addAction(restoreHeader);
    connect(ui->itemsView,&QTableView::customContextMenuRequested,this,&MainWindow::itemHeaderMenuRequested);
}

void MainWindow::setupChannelContextMenu() {
    channelContextMenu = new QMenu(this);
    channelContextMenu->addAction(ui->action_Add);
    channelContextMenu->addAction(ui->action_Edit_channel);
    channelContextMenu->addAction(ui->action_Remove);
    channelContextMenu->addSeparator();
    channelContextMenu->addAction(ui->actionRefresh_selected);
    channelContextMenu->addAction(ui->actionR_efresh_all);
    connect(ui->channelListView,&QListView::customContextMenuRequested,this,&MainWindow::channelContextMenuRequested);

}

void MainWindow::itemHeaderMenuRequested(QPoint pos) {
    itemHeaderContextMenu->popup(ui->itemsView->mapToGlobal(pos));
}

void MainWindow::channelContextMenuRequested(QPoint pos) {
    channelContextMenu->popup(ui->channelListView->mapToGlobal(pos));
}
#endif

void MainWindow::restoreHeaders() {
    ui->itemsView->setColumnHidden(2,false);
    ui->itemsView->setColumnHidden(5,false);
    ui->itemsView->setColumnHidden(12,false);
    ui->itemsView->setColumnHidden(13,false);
    ui->itemsView->setColumnHidden(0,true);
    ui->itemsView->setColumnHidden(1,true);
    ui->itemsView->setColumnHidden(3,true);
    //ui->itemsView->setColumnHidden(4,true);
    ui->itemsView->setColumnHidden(6,true);
    ui->itemsView->setColumnHidden(7,true);
    ui->itemsView->setColumnHidden(8,true);
    ui->itemsView->setColumnHidden(9,true);
    ui->itemsView->setColumnHidden(10,true);
    ui->itemsView->setColumnHidden(11,true);
    ui->itemsView->setColumnHidden(14,true);
    ui->itemsView->resizeColumnsToContents();
    ui->itemsView->resizeRowsToContents();
}

void MainWindow::itemAct(QModelIndex idx) {
    itemproxy->invalidate();
    switch (idx.row()) {
        case 0: itemproxy->setFilterKeyColumn(14);
                break;
        case 1: itemproxy->setFilterKeyColumn(13);
                break;
        default: itemproxy->setFilterKeyColumn(1);
                 itemproxy->setFilterFixedString(idx.sibling(idx.row(),0).data().toString());
    }
}

void MainWindow::starItem(const QModelIndex &idx) {
    if (idx.column()!=13) {
        ui->webview->load(itemproxy->data(idx.sibling(idx.row(),3)).toUrl());
        qDebug() << itemproxy->data(idx.sibling(idx.row(),3)).toString();
        return;
    }
    QSqlRecord rec = items->record(idx.row());
    rec.setValue("star",!rec.value("star").toBool());
    items->setRecord(idx.row(),rec);
    items->select();
}

void MainWindow::onItemLoad(bool ecode) {
    if (!ecode) { return; }
    QSqlRecord rec = items->record(itemproxy->mapToSource(ui->itemsView->selectionModel()->currentIndex()).row());
    rec.setValue("read",true);
    items->setRecord(itemproxy->mapToSource(ui->itemsView->selectionModel()->currentIndex()).row(),rec);
    items->select();
}

void MainWindow::editChannel() {
    if (!ui->channelListView->selectionModel()->hasSelection()) {
        return;
    }
    channelEditDialog editDlg;
    int row = ui->channelListView->selectionModel()->currentIndex().row();
    editDlg.setData(channels->record(row));
    if (editDlg.exec()==QDialog::Accepted) {
        channels->setRecord(row,editDlg.getData());
    }
}

void MainWindow::updateChannelActions() {
    bool res = (ui->channelListView->selectionModel()->hasSelection()&&(itemproxy->filterKeyColumn()==1));
    ui->action_Edit_channel->setEnabled(res);
    ui->action_Remove->setEnabled(res);
}

void MainWindow::removeChannel() {
    channelRemoveDialog dlg;
    if (dlg.exec()==QDialog::Accepted) {
        //remove entries
    }
}

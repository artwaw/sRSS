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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if (!settings.allKeys().contains("dbfile")||(!QFile::exists(settings.value("dbfile").toString()))) {
        zeroconf();
    }
    loadSettings();
    connect(ui->action_Quit,&QAction::triggered,this,&MainWindow::closeApp);
    connect(ui->action_Add,&QAction::triggered,this,&MainWindow::addChannel);
}

MainWindow::~MainWindow()
{
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
    settings.setValue("cache",QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
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
    QSqlQuery query("create table sIndex(id integer primary key, rgroup integer, rtable text, title text, link text, description text, lang text, copyright text, manedit text, webmaster text, pubdate text, lastbuild text, category text, generator text, docs text, ttl integer, imgurl text, imgwidth integer, imgheight integer, skiphrs text, skipdays text);");
    query.exec();
    if (query.lastError().type()!=QSqlError::NoError) {
        fail(1,tr("Error while data file init"),tr("Something went wrong while preparing the data file. Please forward the below info to srss@trollnet.com.pl:")+query.lastError().text());
    }
    query.exec("create table sGroups(id integer primary key, rgroup text);");
    if (query.lastError().type()!=QSqlError::NoError) {
        fail(1,tr("Error while data file init"),tr("Something went wrong while preparing the data file. Please forward the below info to srss@trollnet.com.pl:")+query.lastError().text());
    }
    query.exec("insert into sGroups(rgroup) values('-');");
    if (query.lastError().type()!=QSqlError::NoError) {
        fail(1,tr("Error while data file init"),tr("Something went wrong while preparing the data file. Please forward the below info to srss@trollnet.com.pl:")+query.lastError().text());
    }
}

void MainWindow::loadSettings() {
    move(settings.value("posX").toInt(),settings.value("posY").toInt());
    resize(settings.value("width").toInt(),settings.value("height").toInt());
}

void MainWindow::saveSettings() {
    settings.setValue("width",width());
    settings.setValue("height",height());
    settings.setValue("posX",x());
    settings.setValue("posY",y());
    settings.sync();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    auto db = QSqlDatabase::database();
    db.close();
    saveSettings();
    event->accept();
}

void MainWindow::closeApp() {
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

}

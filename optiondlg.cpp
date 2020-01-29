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
#include "optiondlg.h"
#include "ui_optiondlg.h"

optiondlg::optiondlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::optiondlg)
{
    ui->setupUi(this);
    reloadSettings();
    recalculateSizes();
    findFile = new QAction(QIcon(":/gfx/file-code-setting.png"),"");
    findFile->setToolTip(tr("Browse for file..."));
    ui->dbFolderEdit->addAction(findFile,QLineEdit::TrailingPosition);
    findFolder = new QAction(QIcon("/gfx/folder-2.png"),"");
    findFolder->setToolTip("Brwose for folder...");
    ui->cacheEdit->addAction(findFolder,QLineEdit::TrailingPosition);
    connect(findFile,&QAction::triggered,this,&optiondlg::selectFile);
    connect(findFolder,&QAction::triggered,this,&optiondlg::selectFolder);
    connect(ui->okBtn,&QPushButton::clicked,this,&optiondlg::ok);
    connect(ui->helpBtn,&QPushButton::clicked,this,&optiondlg::help);
    connect(ui->cancelBtn,&QPushButton::clicked,this,&optiondlg::reject);
    connect(ui->clearDBBtn,&QPushButton::clicked,this,&optiondlg::clearDBClicked);
    connect(ui->openDBBtn,&QPushButton::clicked,this,&optiondlg::openDBClicked);
    connect(ui->moveDBBtn,&QPushButton::clicked,this,&optiondlg::moveDB);
    connect(ui->cacheClearBtn,&QPushButton::clicked,this,&optiondlg::clearCacheClicked);
    connect(ui->cacheMoveBtn,&QPushButton::clicked,this,&optiondlg::moveCache);
    connect(ui->newDBBtn,&QPushButton::clicked,this,&optiondlg::newDBclicked);
}

optiondlg::~optiondlg()
{
    delete ui;
}

void optiondlg::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void optiondlg::reloadSettings() {
    ui->cacheEdit->setText(settings.value("cache").toString());
    ui->cacheAttached->setChecked(settings.value("envelopes").toBool());
    ui->downloadEdit->setText(settings.value("downloads").toString());
    ui->downloadAskBox->setChecked(settings.value("askdownload").toBool());
    ui->dbFolderEdit->setText(settings.value("dbfile").toString());
    ui->honorSkipBox->setChecked(settings.value("honor").toBool());
    ui->checkContentBox->setChecked(settings.value("autocheck").toBool());
    ui->spinBox->setValue(settings.value("autointerval").toInt());
}

void optiondlg::recalculateSizes() {
    ui->cacheSize->setText(QString::number((getDirSize(settings.value("cache").toString())/1024)/1024));
    QFileInfo finfo;
    finfo.setFile(QFile(settings.value("dbdir").toString()));
    ui->dbFileSize->setText(QString::number((finfo.size()/1024)/1024));
}

qint64 optiondlg::getDirSize(QString dir) {
    QDir folder(dir);
    qint64 sum=0;
    QFileInfoList flist=folder.entryInfoList(QDir::NoDot|QDir::NoDotDot|QDir::NoSymLinks);
    for (auto x=0;x<flist.size();++x) {
        if (flist.at(x).isDir()) {
            sum=getDirSize(flist.at(x).dir().path());
        } else {
            sum=sum+flist.at(x).size();
        }
    }
    return sum;
}

void optiondlg::moveDB() {
    QString target=QFileDialog::getExistingDirectory(this,tr("Select target folder for data file"),ui->dbFolderEdit->text());
    if (target.isEmpty()) {
        return;
    }
    emit moveDBClicked(target);
    reloadSettings();
}

void optiondlg::moveCache() {
    QString target=QFileDialog::getExistingDirectory(this,tr("Select destination folder for cache"),ui->cacheEdit->text());
    if (target.isEmpty()) {
        return;
    }
    emit moveCacheClicked(target);
    reloadSettings();
    recalculateSizes();
}

void optiondlg::help() {
    QMessageBox::information(this,tr("No such method"),tr("This functionality has not been implemented yet."));
}

void optiondlg::ok() {
    settings.setValue("dbfile",ui->dbFolderEdit->text());
    settings.setValue("cache",ui->cacheEdit->text());
    settings.setValue("envelope",ui->cacheAttached->isChecked());
    settings.setValue("downloads",ui->downloadEdit->text());
    settings.setValue("askdownload",ui->downloadAskBox->isChecked());
    settings.setValue("autocheck",ui->checkContentBox->isChecked());
    settings.setValue("autointerval",ui->spinBox->value());
    settings.setValue("honor",ui->honorSkipBox->isChecked());
    settings.sync();
    accept();
}

void optiondlg::selectFile() {
    QString target=QFileDialog::getOpenFileName(this,tr("Select data file"),ui->dbFolderEdit->text());
    if (target.isEmpty()) {
        return;
    }
    ui->dbFolderEdit->setText(target);
}

void optiondlg::selectFolder() {
    QString target=QFileDialog::getExistingDirectory(this,tr("Select folder"),ui->cacheEdit->text());
    if (target.isEmpty()) {
        return;
    }
    ui->cacheEdit->setText(target);
}

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
#ifndef OPTIONDLG_H
#define OPTIONDLG_H

#include <QDialog>
#include <QSettings>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>

namespace Ui {
class optiondlg;
}

class optiondlg : public QDialog
{
    Q_OBJECT

public:
    explicit optiondlg(QWidget *parent = nullptr);
    ~optiondlg();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::optiondlg *ui;
    QSettings settings;
    QAction *findFile;
    QAction *findFolder;

    void recalculateSizes();
    qint64 getDirSize(QString dir);

signals:
    void newDBclicked();
    void moveDBClicked(QString target);
    void clearCacheClicked();
    void moveCacheClicked(QString target);
    void openDBClicked();
    void clearDBClicked();

public slots:
    void reloadSettings();

private slots:
    void moveDB();
    void moveCache();
    void help();
    void ok();
    void selectFile();
    void selectFolder();
};

#endif // OPTIONDLG_H

#include "channeleditdialog.h"
#include "ui_channeleditdialog.h"

channelEditDialog::channelEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::channelEditDialog)
{
    ui->setupUi(this);
    ui->skipdaysWidget->insertItems(0,QStringList({tr("Monday"),tr("Tuesday"),tr("Wednesday"),tr("Thursday"),tr("Friday"),tr("Saturday"),tr("Sunday")}));
    QString tmp;
    for (auto x=0;x<24;++x) {
        tmp = QString::number(x);
        if (x<10) {
            tmp.prepend('0');
        }
        ui->skiphoursWidget->insertItem(x,tmp);
    }
    connect(ui->okBtn,&QPushButton::clicked,this,&channelEditDialog::accept);
    connect(ui->cancelBtn,&QPushButton::clicked,this,&channelEditDialog::reject);
    connect(ui->revertBtn,&QPushButton::clicked,this,&channelEditDialog::fillData);
}

channelEditDialog::~channelEditDialog()
{
    delete ui;
}

void channelEditDialog::setData(QSqlRecord record) {
   ori=record;
   fillData();
}

void channelEditDialog::fillData() {
    ui->addressLine->setText(ori.value("link").toString());
    QStringList days=ori.value("skipDays").toString().split(',',Qt::SkipEmptyParts);
    QVector<bool> vec; vec.reserve(7); vec.fill(false,7);
    for (auto x=0;x<days.size();++x) {
        if (days.at(x).toInt()>=0&&days.at(x).toInt()<7) {
            vec[days.at(x).toInt()] = true;
        }
    }
    for (auto x=0;x<7;++x) {
        ui->skipdaysWidget->item(x)->setSelected(vec.at(x));
    }
    days=ori.value("skipHours").toString().split(',',Qt::SkipEmptyParts);
    vec.clear(); vec.reserve(24); vec.fill(false,24);
    for (auto x=0;x<days.size();++x) {
        if (days.at(x).toInt()>=0&&days.at(x).toInt()<24) {
            vec[days.at(x).toInt()] = true;
        }
    }
    for (auto x=0;x<24;++x) {
        ui->skiphoursWidget->item(x)->setSelected(vec.at(x));
    }
}

QSqlRecord channelEditDialog::getData() const {
    QSqlRecord rec = ori;
    rec.setValue("link",ui->addressLine->text());
    QString list;
    for (auto x=0;x<7;++x) {
        if (ui->skipdaysWidget->item(x)->isSelected()) {
            list.append(QString::number(x)+",");
        }
    }
    list.chop(1);
    rec.setValue("skipDays",list);
    list.clear();
    for (auto x=0;x<24;++x) {
        if (ui->skiphoursWidget->item(x)->isSelected()) {
            list.append(QString::number(x)+",");
        }
    }
    list.chop(1);
    rec.setValue("skipHours",list);
    return rec;
}

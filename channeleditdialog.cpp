#include "channeleditdialog.h"
#include "ui_channeleditdialog.h"

channelEditDialog::channelEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::channelEditDialog)
{
    ui->setupUi(this);
}

channelEditDialog::~channelEditDialog()
{
    delete ui;
}

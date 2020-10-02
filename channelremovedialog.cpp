#include "channelremovedialog.h"
#include "ui_channelremovedialog.h"

channelRemoveDialog::channelRemoveDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::channelRemoveDialog)
{
    ui->setupUi(this);
    connect(ui->pushButton_2,&QPushButton::clicked,this,&channelRemoveDialog::accept);
    connect(ui->pushButton,&QPushButton::clicked,this,&channelRemoveDialog::reject);
}

channelRemoveDialog::~channelRemoveDialog()
{
    delete ui;
}

bool channelRemoveDialog::withBookmarks() const {
    return ui->checkBox->isChecked();
}

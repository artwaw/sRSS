#include "addchannel.h"
#include "ui_addchannel.h"

addChannel::addChannel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addChannel)
{
    ui->setupUi(this);
    ui->addBtn->setEnabled(false);
    QAction *clearAct = new QAction(QIcon(":/gfx/skull-2.png"),QString(),this);
    clearAct->setToolTip(tr("Clears input line"));
    ui->urlLine->addAction(clearAct);
    manager = new QNetworkAccessManager(this);
    connect(clearAct,&QAction::triggered,ui->urlLine,&QLineEdit::clear);
    connect(ui->addBtn,&QPushButton::clicked,this,&addChannel::add);
    connect(ui->cancelBtn,&QPushButton::clicked,this,&QDialog::reject);
    connect(ui->urlLine,&QLineEdit::textChanged,this,&addChannel::verifyBtns);
    connect(manager,&QNetworkAccessManager::finished,this,&addChannel::finished);
    status = -1;
    parser.init();
}

addChannel::~addChannel()
{
    delete ui;
}

void addChannel::verifyBtns() {
    ui->addBtn->setEnabled(ui->urlLine->text().size()>5);
}

void addChannel::add() {
    manager->get(QNetworkRequest(QUrl(ui->urlLine->text())));
}

void addChannel::finished(QNetworkReply *reply) {
    status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (status==200) {
        QByteArray data=reply->readAll();
        if (parser.parseData(data)) {
            chinfo=parser.getChannelInfo();
            items=parser.getItemsInfo();
            reply->deleteLater();
            accept();
            return;
        }
    }
    reply->deleteLater();
    reject();
}

int addChannel::lastStatus() {
    return status;
}

QVector<ChannelParser::itemInfo> addChannel::getChannelItems() const {
    return items;
}

ChannelParser::channelInfo addChannel::getChannelInfo() const {
    return chinfo;
}

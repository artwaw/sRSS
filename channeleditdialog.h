#ifndef CHANNELEDITDIALOG_H
#define CHANNELEDITDIALOG_H

#include <QDialog>

namespace Ui {
class channelEditDialog;
}

class channelEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit channelEditDialog(QWidget *parent = nullptr);
    ~channelEditDialog();

private:
    Ui::channelEditDialog *ui;
};

#endif // CHANNELEDITDIALOG_H

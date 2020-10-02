#ifndef CHANNELEDITDIALOG_H
#define CHANNELEDITDIALOG_H

#include <QDialog>
#include <QSqlRecord>
#include <QDebug>

namespace Ui {
class channelEditDialog;
}

/*! \class channelEditDialog
 * \brief Simple editor of properties of RSS channel as a modal dialog. */
class channelEditDialog : public QDialog
{
    Q_OBJECT

public:
    /*! \brief Generic constructor. Inits the view.
     * \param parent Optional parent object pointer or nullptr */
    explicit channelEditDialog(QWidget *parent = nullptr);

    /*! \brief Generic destructor */
    ~channelEditDialog();

    /*! \brief Setter method for the view items
     * \param record Database record with channel parameters */
    void setData(QSqlRecord record);

    /*! \brief Getter method for the altered channel.
     *
     * Please note that it only holds valid data if dialog has been closed with "accept" button.
     * \return Complete record with all the parameters (not only altered ones). */
    QSqlRecord getData() const;

private:
    Ui::channelEditDialog *ui; //!< User interface pointer
    QSqlRecord ori; //!< Holder var for source record

private slots:
    /*! \brief Method fills the form with ori record data
     * \see setData()
     * \see ori */
    void fillData();
};

#endif // CHANNELEDITDIALOG_H

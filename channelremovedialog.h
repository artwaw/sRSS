#ifndef CHANNELREMOVEDIALOG_H
#define CHANNELREMOVEDIALOG_H

#include <QDialog>

namespace Ui {
class channelRemoveDialog;
}

/*! \class channelRemoveDialog
 * \brief Simple QDialog asking for confirmation.
 *
 * Also offers an option to remove/leave bookmarked items. */
class channelRemoveDialog : public QDialog
{
    Q_OBJECT

public:
    /*! \brief Generic ctor
     * \param parent Pointer to parent object or nullptr */
    explicit channelRemoveDialog(QWidget *parent = nullptr);

    /*! \brief Generic dctor */
    ~channelRemoveDialog();

    /*! \brief Getter method for checkbox confirming bookmarked item deletion
     * \return true if user agrees to remove bookmarked items */
    bool withBookmarks() const;

private:
    Ui::channelRemoveDialog *ui; //!< Internal user interface pointer
};

#endif // CHANNELREMOVEDIALOG_H

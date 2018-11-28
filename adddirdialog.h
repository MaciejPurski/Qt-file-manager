#ifndef ADDDIRDIALOG_H
#define ADDDIRDIALOG_H

#include <QDialog>

namespace Ui {
class AddDirDialog;
}

class AddDirDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddDirDialog(QWidget *parent = 0);
    ~AddDirDialog();

private:
    Ui::AddDirDialog *ui;
};

#endif // ADDDIRDIALOG_H

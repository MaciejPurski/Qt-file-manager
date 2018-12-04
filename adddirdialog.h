#ifndef ADDDIRDIALOG_H
#define ADDDIRDIALOG_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class AddDirDialog;
}

class AddDirDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddDirDialog(QWidget *parent = 0, QString name = "");
    ~AddDirDialog();
    QString getDirName();

private slots:
    void on_textEdit_textChanged();

    void on_okButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::AddDirDialog *ui;
    MainWindow *mainWindow;
    QString dirName;
    bool edit;
};

#endif // ADDDIRDIALOG_H

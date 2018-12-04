#include "adddirdialog.h"
#include "ui_adddirdialog.h"
#include <iostream>

AddDirDialog::AddDirDialog(QWidget *parent, QString name) :
    QDialog(parent),
    ui(new Ui::AddDirDialog)
{
    this->mainWindow = static_cast<MainWindow *>(parent);
    ui->setupUi(this);

    ui->okButton->setEnabled(false);
    ui->errorLabel->setText("");
    ui->textEdit->setStyleSheet("");
    ui->textEdit->setText(name);
    this->dirName = name;
    edit = (name != "");
}

AddDirDialog::~AddDirDialog()
{
    delete ui;
}

QString AddDirDialog::getDirName()
{
    return dirName;
}

void AddDirDialog::on_textEdit_textChanged()
{
    QString text = ui->textEdit->toPlainText();
    if ((edit && (text == dirName)) || text == "") {
        ui->okButton->setEnabled(false);
        ui->errorLabel->setText("");
        ui->textEdit->setStyleSheet("");
    } else if (mainWindow->dirExists(text)) {
        ui->errorLabel->setText("Directory already exists!");
        ui->errorLabel->setStyleSheet("#errorLabel { color : red; }");
        ui->okButton->setEnabled(false);
        ui->textEdit->setStyleSheet("#textEdit { border: 3px solid red; }");
    } else {
        ui->okButton->setEnabled(true);
        ui->errorLabel->setText("");
        ui->textEdit->setStyleSheet("");
    }
}

void AddDirDialog::on_okButton_clicked()
{
    dirName = ui->textEdit->toPlainText();
    std::cout << "dir name; " << dirName.toStdString() << std::endl;
    this->accept();

}

void AddDirDialog::on_cancelButton_clicked()
{
    this->reject();
}

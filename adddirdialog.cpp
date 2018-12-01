#include "adddirdialog.h"
#include "ui_adddirdialog.h"

AddDirDialog::AddDirDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDirDialog)
{
    ui->setupUi(this);
    ui->okButton->setEnabled(false);
    ui->errorLabel->setText("");
    ui->textEdit->setStyleSheet("");
}

AddDirDialog::~AddDirDialog()
{
    delete ui;
}

void AddDirDialog::on_textEdit_textChanged()
{
    if (ui->textEdit->toPlainText() == "") {
        ui->okButton->setEnabled(false);
        ui->errorLabel->setText("");
        ui->textEdit->setStyleSheet("");
    } else if (ui->textEdit->toPlainText() == "dupa") {
        ui->errorLabel->setText("Directory already exists!");
        ui->okButton->setEnabled(false);
        //frame->setObjectName("myObject");
        ui->textEdit->setStyleSheet("#textEdit { border: 3px solid red; }");
    } else {
        ui->okButton->setEnabled(true);
        ui->errorLabel->setText("");
        ui->textEdit->setStyleSheet("");
    }
}

void AddDirDialog::on_okButton_clicked()
{
    this->accept();

}

void AddDirDialog::on_cancelButton_clicked()
{
    this->reject();
}

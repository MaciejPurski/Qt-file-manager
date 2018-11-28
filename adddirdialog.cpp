#include "adddirdialog.h"
#include "ui_adddirdialog.h"

AddDirDialog::AddDirDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDirDialog)
{
    ui->setupUi(this);
}

AddDirDialog::~AddDirDialog()
{
    delete ui;
}

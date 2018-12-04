#include "copydialog.h"
#include "ui_copydialog.h"

CopyDialog::CopyDialog(QWidget *parent, QModelIndexList &list, QFileSystemModel *nmodel, QString dest) :
    copyList(list),
    model(nmodel),
    QDialog(parent),
    ui(new Ui::CopyDialog)
{
    ui->setupUi(this);
    nBlocks = model->fileInfo(list.front()).size() / 1024;
    if (model->fileInfo(list.front()).size() % 1024 != 0)
        nBlocks++;

    blocksCopied = 0;
    copyFrom = model->filePath(list.front());
    copyTo = dest;
    ui->progressBar->setValue(0);
}

CopyDialog::~CopyDialog()
{
    delete ui;
}


void CopyDialog::startCopy()
{
    src = new QFile(copyFrom);
    src->open(QIODevice::ReadOnly);

    dst = new QFile(copyTo);
    src->open(QIODevice::ReadWrite);
}

void CopyDialog::copyBlock()
{
    char buf[1024];

    if (nBlocks == blocksCopied) {
        ui->closeButton->setEnabled(true);
        return;
    }

    src->read(buf, 1024);
    dst->write(buf, 1024);
    nBlocks++;

    ui->progressBar->setValue(((double) blocksCopied / (double) nBlocks) * 100.0);

    QTimer::singleShot(0, this, SLOT(copyBlock());
}

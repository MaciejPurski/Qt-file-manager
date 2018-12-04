#ifndef COPYDIALOG_H
#define COPYDIALOG_H

#include <QDialog>
#include <QFileSystemModel>
#include <QItemSelectionModel>

namespace Ui {
class CopyDialog;
}

class CopyDialog : public QDialog
{
    Q_OBJECT

public:
    CopyDialog(QWidget *parent = 0, QModelIndexList &list, QFileSystemModel *nmodel, QString dest);
    ~CopyDialog();

public slots:
    void startCopy();

private:
    Ui::CopyDialog *ui;
    QFileSystemModel *model;
    QModelIndexList &copyList;
    QString copyFrom;
    QString copyTo;
    uint nBlocks;
    uint blocksCopied;
    QFile *src, *dst;

private slots:
    void copyBlock();
};

#endif // COPYDIALOG_H

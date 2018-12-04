#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "filesystemmodel.h"
#include "adddirdialog.h"

#include <QFileSystemModel>
#include <QDir>
#include <QDebug>
#include <iostream>
#include <QInputDialog>
#include <QErrorMessage>
#include <QMessageBox>

/*
 * wymagania:
 * - nazwa okna inna niz main
 * - wszystkie komunikaty w jednym jezyku
 * - pasek narzedziowy dobrze zrobiony
 * - toolbar nie moze byc pusty
 * - menu musi sie rozwijac
 * - dla Model - View wlasny model
 * - nie wolno blokowac zmiany rozmiaru okienka
 */

// artykul - keeping the gui responsive qt
// sprawdzic czy sie odswieza QFileSystemModel
// skroty podlaczyc do przyciskow

// TODO: enter nie powinien powodowac przejscia do kolejnej linii tylko potwierdzenie
/* pytania:
 * - czy model-view
 * - czy QTableView ok i QFileSystemModel ok
 * - czy QFileSystemModel sie odswieza
 * - ktory widget do przyciskow
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    model = new FileSystemModel(this);
    model->setRootPath(QDir::homePath());
    ui->tableView->setModel(model);

    copyPending = false;
    cutPending = false;
    selectionModel = ui->tableView->selectionModel();
    enterDirectory(model->index(QDir::homePath()));
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    connect(selectionModel, SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            this, SLOT(updateSelection(const QItemSelection &, const QItemSelection &)));

    connect(ui->tableView, SIGNAL(doubleClicked(const QModelIndex)), this, SLOT(enterDirectory(const QModelIndex)));
    connect(ui->actionParent_Directory, SIGNAL(triggered(bool)), this, SLOT(goToParentDir()));
}

QModelIndex MainWindow::currentDir()
{
    return ui->tableView->rootIndex();
}

bool MainWindow::isCurrentDirWritable()
{
    return model->fileInfo(currentDir()).isWritable();
}

bool MainWindow::dirExists(const QString &name)
{
    QTableView *tableView = ui->tableView;

    QModelIndex index = tableView->rootIndex();
    QString path = model->filePath(index);
    path.append("/" + name);
    return QDir(path).exists();
}

void MainWindow::updateSelection(const QItemSelection &selected,
    const QItemSelection &deselected)
{
    return;

    QModelIndex index;
    QModelIndexList items = selected.indexes();

    foreach (index, items) {
        QString text = model->fileName(index);

        ui->statusBar->showMessage(model->fileInfo(index).fileName());
    }
}

void MainWindow::enterDirectory(const QModelIndex &index)
{
    QModelIndex fileIndex = index.sibling(index.row(), 0);


    if (!model->fileInfo(fileIndex).isReadable() ||
        !model->fileInfo(fileIndex).isExecutable()) {
        QMessageBox::critical(this, "Permission denied", "You are not permitted to enter this directory!");
        return;
    }


    if (model->isDir(fileIndex)) {
        if (model->fileName(fileIndex) == "..") {
            const QModelIndex &parent = model->parent(currentDir());

            ui->tableView->setRootIndex(parent);
            ui->statusBar->showMessage(model->filePath(parent));
        } else if (model->fileName(fileIndex) == ".") {
            return;
        } else {
            ui->tableView->setRootIndex(fileIndex);
            ui->statusBar->showMessage(model->filePath(fileIndex));
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::goToParentDir()
{
    enterDirectory(model->parent(currentDir()));
}

void MainWindow::on_actionEnter_Directory_triggered()
{
    QModelIndexList items = selectionModel->selectedRows(0);

    if (items.size() != 1) {
        QMessageBox::warning(this, "Error", "You must select exactly one item!");
        return;
    }

    enterDirectory(items.front());
}

void MainWindow::on_actionAddDirectory_triggered()
{
    if (!isCurrentDirWritable()) {
        QMessageBox::critical(this, "Permission denied", "You don't have write permission in this directory!");
        return;
    }

    AddDirDialog *dialogBox = new AddDirDialog(this);

    if (dialogBox->exec() == QDialog::Accepted) {
        model->mkdir(currentDir(), dialogBox->getDirName());
    }
}

void MainWindow::on_actionDelete_triggered()
{
    if (!isCurrentDirWritable()) {
        QMessageBox::critical(this, "Permission denied", "You don't have write permission in this directory!");
        return;
    }

    QModelIndexList items = selectionModel->selectedRows(0);

    if (items.size() == 0) {
        QMessageBox::warning(this, "Error", "You must select at least one item!");
        return;
    }


    QMessageBox::StandardButton ret = QMessageBox::question(this, "Warning", "Are you sure, you want to remove " +
                                      QString::number(items.size()) +
                                      " elements?", QMessageBox::Cancel | QMessageBox::Yes);

    if (ret == QMessageBox::Cancel)
        return;

     QModelIndex index;

    foreach (index, items) {
        model->remove(index);
    }

}

void MainWindow::on_actionEdit_item_triggered()
{

    if (!isCurrentDirWritable()) {
        QMessageBox::critical(this, "Permission denied", "You don't have write permission in this directory!");
        return;
    }

    QModelIndexList items = selectionModel->selectedRows(0);

    if (items.size() != 1) {
        QMessageBox::warning(this, "Error", "You must select exactly one item!");
        return;
    }

    QModelIndex index = items.front();

    AddDirDialog *dialog = new AddDirDialog(this, model->fileName(index));

    if (dialog->exec() == QDialog::Accepted) {
        QFile::rename(model->filePath(index), model->filePath(currentDir()) + "/" + dialog->getDirName());
    }
}

void MainWindow::on_actionCopy_triggered()
{
    QModelIndexList nList = selectionModel->selectedRows(0);

    if (nList.empty())
        return;

    if (cutPending)
        cutPending = false;

    selectedList = std::move(nList);
    copyPending = true;
}

void MainWindow::on_actionCut_triggered()
{
    QModelIndexList nList = selectionModel->selectedRows(0);

    if (nList.empty())
        return;

    if (copyPending) {
        copyPending = false;
    }

    selectedList = std::move(nList);
    cutPending = true;
}

void MainWindow::on_actionPaste_triggered()
{
    if (selectedList.empty())
        return;

    /* Cancel pending copy or paste if the dest directory is the same as the source */
    if (model->parent(selectedList.front()) == currentDir()) {
        copyPending = false;
        cutPending = false;
        selectedList.clear();
        std::cout << "CANCEL\n";
        return;
    }

    if (copyPending) {
        QModelIndex index;

        foreach (index, selectedList) {
            model->copy(index, currentDir());
        }
    } else if (cutPending) {

    }
}

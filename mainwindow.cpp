#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "filesystemmodel.h"
#include "adddirdialog.h"

#include <QFileSystemModel>
#include <QDir>
#include <QDebug>
#include <iostream>

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

    selectionModel = ui->tableView->selectionModel();
    enterDirectory(model->index(QDir::homePath()));
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    connect(selectionModel, SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            this, SLOT(updateSelection(const QItemSelection &, const QItemSelection &)));

    connect(ui->tableView, SIGNAL(doubleClicked(const QModelIndex)), this, SLOT(enterDirectory(const QModelIndex)));
    connect(ui->actionParent_Directory, SIGNAL(triggered(bool)), this, SLOT(goToParentDir()));
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

    if (model->isDir(fileIndex)) {
        if (model->fileName(fileIndex) == "..") {
            const QModelIndex &parent = model->parent(ui->tableView->rootIndex());

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
    enterDirectory(model->parent(ui->tableView->rootIndex()));
}

void MainWindow::on_actionEnter_Directory_triggered()
{
    QModelIndexList items = selectionModel->selectedRows(0);

    if (items.size() != 1)
        return;

    enterDirectory(items.front());
}

void MainWindow::on_actionAddDirectory_triggered()
{
    AddDirDialog *dialogBox = new AddDirDialog(this);

    dialogBox->exec();
}

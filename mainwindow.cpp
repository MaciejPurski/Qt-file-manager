#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "filesystemmodel.h"
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

    enterDirectory(model->index(QDir::homePath()));

    selectionModel = ui->tableView->selectionModel();


    connect(selectionModel, SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            this, SLOT(updateSelection(const QItemSelection &, const QItemSelection &)));

    connect(ui->tableView, SIGNAL(doubleClicked(const QModelIndex)), this, SLOT(enterDirectory(const QModelIndex)));
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
    if (model->isDir(index)) {
        if (model->fileName(index) == "..") {
            const QModelIndex &parent = model->parent(ui->tableView->rootIndex());

            ui->tableView->setRootIndex(parent);
            ui->statusBar->showMessage(model->filePath(parent));
        } else if (model->fileName(index) == ".") {
            return;
        } else {
            ui->tableView->setRootIndex(index);
            ui->statusBar->showMessage(model->filePath(index));
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_toolButton_2_clicked()
{
    enterDirectory(model->parent(ui->tableView->rootIndex()));
}

void MainWindow::on_tableView_entered(const QModelIndex &index)
{
    enterDirectory(model->parent(ui->tableView->rootIndex()));
}

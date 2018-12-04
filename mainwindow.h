#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QItemSelectionModel>
#include <QItemSelection>
#include <QFileSystemModel>
#include "filesystemmodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    bool dirExists(const QString &name);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    FileSystemModel *model;
    QItemSelectionModel *selectionModel;
    QModelIndexList selectedList;
    bool copyPending;
    bool cutPending;

    QModelIndex currentDir();

public slots:
    void updateSelection(const QItemSelection &selected,
                         const QItemSelection &deselected);
    void enterDirectory(const QModelIndex &index);
private slots:
    void goToParentDir();
    void on_actionEnter_Directory_triggered();
    void on_actionAddDirectory_triggered();
    bool isCurrentDirWritable();
    void on_actionDelete_triggered();
    void on_actionEdit_item_triggered();
    void on_actionCopy_triggered();
    void on_actionCut_triggered();
    void on_actionPaste_triggered();
};

#endif // MAINWINDOW_H

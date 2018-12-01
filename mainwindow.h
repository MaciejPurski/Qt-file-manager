#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QItemSelectionModel>
#include <QItemSelection>
#include <QFileSystemModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QFileSystemModel *model;
    QItemSelectionModel *selectionModel;

public slots:
    void updateSelection(const QItemSelection &selected,
                         const QItemSelection &deselected);
    void enterDirectory(const QModelIndex &index);
private slots:
    void goToParentDir();
    void on_actionEnter_Directory_triggered();
    void on_actionAddDirectory_triggered();
    bool dirExists(QString name);
    bool isPermitedToCreate();
};

#endif // MAINWINDOW_H

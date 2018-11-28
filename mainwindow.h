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
    void on_toolButton_2_clicked();
    void on_tableView_entered(const QModelIndex &index);
};

#endif // MAINWINDOW_H

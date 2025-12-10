#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>

namespace Ui {
class MainWindow;
}

class FileManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // UI slots connected in setupConnections()
    void onTreeViewClicked(const QModelIndex &index);
    void onListViewClicked(const QModelIndex &index);
    void onListViewDoubleClicked(const QModelIndex &index);
    void onGoButtonClicked();
    void onPathEditReturnPressed();

    // Action slots (auto-connected from .ui)
    void on_actionNewFolder_triggered();
    void on_actionNewFile_triggered();
    void on_actionDelete_triggered();
    void on_actionRefresh_triggered();
    void on_actionExit_triggered();
    void on_actionHome_triggered();
    void on_actionUp_triggered();
    void on_actionAbout_triggered();

private:
    void setupFileModels();
    void setupConnections();
    void navigateTo(const QString &path);
    void updateStatus(const QString &message);

    Ui::MainWindow *ui;
    QFileSystemModel *treeModel;
    QFileSystemModel *listModel;
    FileManager *fileManager;
};

#endif // MAINWINDOW_H

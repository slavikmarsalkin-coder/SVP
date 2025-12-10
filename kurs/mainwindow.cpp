#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "filemanager.h"

#include <QFileSystemModel>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialize file manager
    fileManager = new FileManager(this);

    // Setup file models
    setupFileModels();

    // Setup connections
    setupConnections();

    // Go to home directory
    on_actionHome_triggered();

    // Setup list view
    ui->listView->setViewMode(QListView::IconMode);
    ui->listView->setIconSize(QSize(48, 48));
    ui->listView->setGridSize(QSize(100, 80));

    // Setup tree view
    ui->treeView->hideColumn(1); // Size
    ui->treeView->hideColumn(2); // Type
    ui->treeView->hideColumn(3); // Date modified

    // Setup splitter
    ui->splitter->setSizes({300, 700});

    // Set window title
    setWindowTitle("File Manager");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupFileModels()
{
    // Tree model for folders only
    treeModel = new QFileSystemModel(this);
    treeModel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
    treeModel->setRootPath("");
    ui->treeView->setModel(treeModel);

    // List model for all files
    listModel = new QFileSystemModel(this);
    listModel->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    ui->listView->setModel(listModel);
}

void MainWindow::setupConnections()
{
    // Connect UI signals
    connect(ui->treeView, &QTreeView::clicked,
            this, &MainWindow::onTreeViewClicked);
    connect(ui->listView, &QListView::clicked,
            this, &MainWindow::onListViewClicked);
    connect(ui->listView, &QListView::doubleClicked,
            this, &MainWindow::onListViewDoubleClicked);
    connect(ui->goButton, &QPushButton::clicked,
            this, &MainWindow::onGoButtonClicked);
    connect(ui->pathEdit, &QLineEdit::returnPressed,
            this, &MainWindow::onPathEditReturnPressed);

    // Connect file manager signals
    connect(fileManager, &FileManager::directoryChanged,
            this, &MainWindow::navigateTo);
    connect(fileManager, &FileManager::errorOccurred,
            [this](const QString &error) {
                ui->statusbar->showMessage("Error: " + error, 3000);
                QMessageBox::warning(this, "Error", error);
            });
}

void MainWindow::onTreeViewClicked(const QModelIndex &index)
{
    QString path = treeModel->filePath(index);
    navigateTo(path);
}

void MainWindow::onListViewClicked(const QModelIndex &index)
{
    QString path = listModel->filePath(index);
    QFileInfo info(path);

    if (info.isFile()) {
        QString status = QString("File: %1 | Size: %2 bytes")
                        .arg(info.fileName())
                        .arg(info.size());
        ui->statusLabel->setText(status);
        ui->statusbar->showMessage(status, 3000);
    }
}

void MainWindow::onListViewDoubleClicked(const QModelIndex &index)
{
    QString path = listModel->filePath(index);
    QFileInfo info(path);

    if (info.isDir()) {
        navigateTo(path);
    }
}

void MainWindow::onGoButtonClicked()
{
    QString path = ui->pathEdit->text();
    navigateTo(path);
}

void MainWindow::onPathEditReturnPressed()
{
    onGoButtonClicked();
}

void MainWindow::on_actionNewFolder_triggered()
{
    QString currentPath = listModel->rootPath();
    bool ok;
    QString folderName = QInputDialog::getText(this, "New Folder",
                                              "Enter folder name:",
                                              QLineEdit::Normal,
                                              "", &ok);

    if (ok && !folderName.isEmpty()) {
        if (fileManager->createDirectory(currentPath, folderName)) {
            updateStatus("Folder created: " + folderName);
        }
    }
}

void MainWindow::on_actionNewFile_triggered()
{
    QString currentPath = listModel->rootPath();
    bool ok;
    QString fileName = QInputDialog::getText(this, "New File",
                                           "Enter file name:",
                                           QLineEdit::Normal,
                                           "new_file.txt", &ok);

    if (ok && !fileName.isEmpty()) {
        if (fileManager->createFile(currentPath, fileName)) {
            updateStatus("File created: " + fileName);
        }
    }
}

void MainWindow::on_actionDelete_triggered()
{
    QModelIndexList selected = ui->listView->selectionModel()->selectedIndexes();
    if (selected.isEmpty()) {
        QMessageBox::information(this, "Information",
                               "Select a file or folder to delete");
        return;
    }

    QModelIndex index = selected.first();
    QString path = listModel->filePath(index);
    QFileInfo info(path);

    int ret = QMessageBox::question(this, "Confirm Delete",
                                  QString("Delete \"%1\"?").arg(info.fileName()),
                                  QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        if (fileManager->deleteItem(path)) {
            updateStatus("Deleted: " + info.fileName());
        }
    }
}

void MainWindow::on_actionRefresh_triggered()
{
    QString currentPath = listModel->rootPath();
    listModel->setRootPath(currentPath);
    ui->listView->setRootIndex(listModel->index(currentPath));
    updateStatus("View refreshed");
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_actionHome_triggered()
{
    navigateTo(QDir::homePath());
}

void MainWindow::on_actionUp_triggered()
{
    QString currentPath = listModel->rootPath();
    QDir dir(currentPath);
    if (dir.cdUp()) {
        navigateTo(dir.absolutePath());
    }
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, "About File Manager",
                      "File Manager v1.0\n\n"
                      "Simple file manager application\n"
                      "Built with Qt 5.11\n\n"
                      "Features:\n"
                      "- Browse file system\n"
                      "- Create files and folders\n"
                      "- Delete items\n"
                      "- Navigate through directories");
}

void MainWindow::navigateTo(const QString &path)
{
    QDir dir(path);
    if (!dir.exists()) {
        QMessageBox::warning(this, "Error",
                           "Directory does not exist:\n" + path);
        return;
    }

    QString absolutePath = dir.absolutePath();

    // Update UI
    ui->pathEdit->setText(absolutePath);

    // Update tree view
    ui->treeView->setRootIndex(treeModel->index(absolutePath));

    // Update list view
    listModel->setRootPath(absolutePath);
    ui->listView->setRootIndex(listModel->index(absolutePath));

    // Expand tree to current path
    QModelIndex index = treeModel->index(absolutePath);
    while (index.isValid()) {
        ui->treeView->expand(index);
        index = index.parent();
    }

    // Update status
    updateStatus("Current directory: " + absolutePath);
}

void MainWindow::updateStatus(const QString &message)
{
    ui->statusLabel->setText(message);
    ui->statusbar->showMessage(message, 3000);
}

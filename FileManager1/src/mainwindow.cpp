#include "mainwindow.h"
#include <QSplitter>
#include <QDir>
#include <QInputDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    central(new QWidget(this)),
    pathEdit(new QLineEdit(this)),
    treeView(new QTreeView(this)),
    listView(new QListView(this)),
    btnUp(new QPushButton("Back", this)),
    manager(new FileManager(this)),
    createFileButton(new QPushButton("Create file", this)),
    createDirButton(new QPushButton("Create dir", this)),
    deletebutton(new QPushButton("Delete", this))
{
    setCentralWidget(central);

    initUI();

    // Подключаем модели
    treeView->setModel(manager->dirModel());
    listView->setModel(manager->fileModel());

    initConnections();

    openPath(QDir::homePath());
}

void MainWindow::initUI()
{
    auto *topButtons = new QHBoxLayout;
    topButtons->addWidget(btnUp);
    topButtons->addWidget(this->createDirButton);
    topButtons->addWidget(this->createFileButton);
    topButtons->addWidget(this->deletebutton);
    topButtons->addStretch();

    auto *splitter = new QSplitter(this);
    splitter->addWidget(treeView);
    splitter->addWidget(listView);
    splitter->setStretchFactor(1, 1);

    auto *mainLayout = new QVBoxLayout(central);
    mainLayout->addLayout(topButtons);
    mainLayout->addWidget(pathEdit);
    mainLayout->addWidget(splitter);

    treeView->setHeaderHidden(true);
}

void MainWindow::initConnections()
{
    connect(treeView, &QTreeView::clicked, this, &MainWindow::onDirClicked);
    connect(listView, &QListView::doubleClicked, this, &MainWindow::onFileClicked);
    connect(btnUp, &QPushButton::clicked, this, &MainWindow::goUp);
    connect(this->deletebutton, &QPushButton::clicked, this, &MainWindow::onDelete);
    connect(this->createFileButton, &QPushButton::clicked, this, &MainWindow::onCreateFile);
    connect(this->createDirButton, &QPushButton::clicked, this, &MainWindow::onCreateDir);
}

void MainWindow::openPath(const QString &path)
{
    QModelIndex idx = manager->setCurrentPath(path);
    pathEdit->setText(path);
    listView->setRootIndex(idx);
}

void MainWindow::onDirClicked(const QModelIndex &index)
{
    QString path = manager->dirModel()->filePath(index);
    openPath(path);
}

void MainWindow::onFileClicked(const QModelIndex &index)
{
    QFileInfo info = manager->fileInfo(index);

    if (info.isDir())
    {
        openPath(info.absoluteFilePath());
    }
    else
    {
        manager->openFile(info.absoluteFilePath());
    }
}

void MainWindow::goUp()
{
    QString parent = manager->parentPath(pathEdit->text());
    openPath(parent);
}

void MainWindow::onDelete()
{
    QModelIndex index = this->listView->currentIndex();
    if (!index.isValid())
        return;

    QString path = manager->fileModel()->filePath(index);

    // Подтверждение удаления
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(
        this,
        "Удаление",
        "Удалить объект?\n" + path,
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
        manager->deleteFile(path);
    }
}

void MainWindow::onCreateFile()
{
    QModelIndex index = this->listView->currentIndex();
    QString dirPath;

    if (index.isValid())
    {
        QFileInfo info(manager->fileModel()->filePath(index));
        dirPath = info.isDir() ? info.absoluteFilePath() : info.absolutePath();
    }
    else
    {
        dirPath = QDir::homePath();
    }

    bool ok;
    QString name = QInputDialog::getText(
        this,
        "Создание файла",
        "Введите имя файла:",
        QLineEdit::Normal,
        "",
        &ok
        );

    if (!ok || name.isEmpty())
        return;

    QString fullPath = dirPath + "/" + name;

    manager->createFile(fullPath);
}

void MainWindow::onCreateDir()
{
    QModelIndex index = this->listView->currentIndex();
    QString dirPath;

    if (index.isValid())
    {
        QFileInfo info(manager->fileModel()->filePath(index));
        dirPath = info.isDir() ? info.absoluteFilePath() : info.absolutePath();
    }
    else
    {
        dirPath = QDir::homePath();
    }

    bool ok;
    QString name = QInputDialog::getText(
        this,
        "Создание папки",
        "Введите имя папки:",
        QLineEdit::Normal,
        "",
        &ok
        );

    if (!ok || name.isEmpty())
        return;

    QString fullPath = dirPath + "/" + name;

    manager->createDir(fullPath);
}

#include "test.h"
#include <QSignalSpy>
#include <QModelIndex>
#include <QLineEdit>
#include <QTreeView>
#include <QTest>
#include "../src/MainWindow.h"

TestMainWindow::TestMainWindow() {}
TestMainWindow::~TestMainWindow() {}

void TestMainWindow::testOpenPath()
{
    MainWindow w;
    w.show();
    QTest::qWaitForWindowExposed(&w);

    QLineEdit* pathEdit = w.findChild<QLineEdit*>();
    QVERIFY2(pathEdit != nullptr, "QLineEdit with objectName 'pathLineEdit' not found!");

    QString path = QDir::homePath();
    w.openPath(path);

    QCOMPARE(pathEdit->text(), path);

    QTreeView* treeView = w.findChild<QTreeView*>();
    QVERIFY(treeView != nullptr);

    QFileSystemModel *model = qobject_cast<QFileSystemModel*>(treeView->model());
    QVERIFY(model != nullptr);

    QModelIndex idx = model->index(path);
    QVERIFY(idx.isValid());
}

void TestMainWindow::testGoUp()
{
    MainWindow w;
    w.show();
    QTest::qWaitForWindowExposed(&w);

    QLineEdit* pathEdit = w.findChild<QLineEdit*>();
    QVERIFY(pathEdit != nullptr);

    QString path = QDir::homePath();
    w.openPath(path);

    QString parent = QFileInfo(path).dir().absolutePath();
    w.goUp();

    QCOMPARE(pathEdit->text(), parent);
}

void TestMainWindow::testFileClick()
{
    MainWindow w;
    w.show();
    QTest::qWaitForWindowExposed(&w);
    QListView* listView = w.findChild<QListView*>();
    QVERIFY(listView != nullptr);

    QFileSystemModel *fileModel = qobject_cast<QFileSystemModel*>(listView->model());
    QVERIFY(fileModel != nullptr);

    QString path = QDir::homePath();
    QModelIndex idx = fileModel->index(path);
    QVERIFY(idx.isValid());

    listView->setCurrentIndex(idx);
    w.onFileClicked(idx);

    QLineEdit* pathEdit = w.findChild<QLineEdit*>();
    QVERIFY(pathEdit != nullptr);
    QCOMPARE(pathEdit->text(), path);

}

void TestMainWindow::testDirClick()
{
    MainWindow w;
    w.show();
    QTest::qWaitForWindowExposed(&w);

    QTreeView* treeView = w.findChild<QTreeView*>();
    QVERIFY(treeView != nullptr);

    QFileSystemModel *dirModel = qobject_cast<QFileSystemModel*>(treeView->model());
    QVERIFY(dirModel != nullptr);

    QString path = QDir::homePath();
    QModelIndex idx = dirModel->index(path);
    QVERIFY(idx.isValid());

    treeView->setCurrentIndex(idx);
    w.onDirClicked(idx);

    QLineEdit* pathEdit = w.findChild<QLineEdit*>();
    QVERIFY(pathEdit != nullptr);
    QCOMPARE(pathEdit->text(), path);
}

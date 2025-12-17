#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include <QFileSystemModel>
#include <QFileInfo>

class FileManager : public QObject {
    Q_OBJECT

public:
    FileManager(QObject *parent = nullptr);

    // Основные операции
    QString normalizePath(const QString &path);
    QString parentPath(const QString &path);

    QModelIndex setCurrentPath(const QString &path);
    QString getCurrentPath() const { return m_currentPath; }

    bool isDir(const QModelIndex &index);
    QFileInfo fileInfo(const QModelIndex &index);

    void openFile(const QString &path);
    void deleteFile(const QString &path);
    void createFile(const QString &path);
    void createDir(const QString &path);

    QFileSystemModel* dirModel() const { return m_dirModel; }
    QFileSystemModel* fileModel() const { return m_fileModel; }

private:
    QFileSystemModel *m_dirModel;
    QFileSystemModel *m_fileModel;

    QString m_currentPath;
};

#endif

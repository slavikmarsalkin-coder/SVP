#include "filemanager.h"
#include <QDebug>

FileManager::FileManager(QObject *parent)
    : QObject(parent)
    , currentPath(QDir::homePath())
{
}

QStringList FileManager::getDirectoryContents(const QString &path)
{
    QDir dir(path);
    if (!dir.exists()) {
        emit errorOccurred("Directory does not exist: " + path);
        return QStringList();
    }

    QStringList contents;
    QFileInfoList entries = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot,
                                              QDir::DirsFirst | QDir::Name);

    for (const QFileInfo &info : entries) {
        contents << info.absoluteFilePath();
    }

    return contents;
}

bool FileManager::createDirectory(const QString &path, const QString &name)
{
    QDir dir(path);
    if (!dir.exists()) {
        emit errorOccurred("Directory does not exist: " + path);
        return false;
    }

    if (dir.mkdir(name)) {
        emit directoryChanged(path);
        return true;
    }

    emit errorOccurred("Cannot create directory: " + name);
    return false;
}

bool FileManager::createFile(const QString &path, const QString &name)
{
    QDir dir(path);
    if (!dir.exists()) {
        emit errorOccurred("Directory does not exist: " + path);
        return false;
    }

    QFile file(dir.filePath(name));
    if (file.open(QIODevice::WriteOnly)) {
        file.close();
        emit directoryChanged(path);
        return true;
    }

    emit errorOccurred("Cannot create file: " + name);
    return false;
}

bool FileManager::deleteItem(const QString &path)
{
    QFileInfo info(path);

    if (!info.exists()) {
        emit errorOccurred("Item does not exist: " + path);
        return false;
    }

    bool success = false;
    if (info.isDir()) {
        QDir dir(path);
        success = dir.removeRecursively();
    } else {
        success = QFile::remove(path);
    }

    if (success) {
        emit directoryChanged(info.absolutePath());
    } else {
        emit errorOccurred("Cannot delete: " + path);
    }

    return success;
}

QString FileManager::getCurrentPath() const
{
    return currentPath;
}

void FileManager::setCurrentPath(const QString &path)
{
    if (QDir(path).exists()) {
        currentPath = path;
        emit directoryChanged(path);
    } else {
        emit errorOccurred("Directory does not exist: " + path);
    }
}

QString FileManager::getHomePath() const
{
    return QDir::homePath();
}

QString FileManager::getParentPath(const QString &path) const
{
    QDir dir(path);
    if (dir.cdUp()) {
        return dir.absolutePath();
    }
    return path;
}

#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include <QDir>
#include <QFileInfo>
#include <QStringList>

class FileManager : public QObject
{
    Q_OBJECT

public:
    explicit FileManager(QObject *parent = nullptr);

    QStringList getDirectoryContents(const QString &path);
    bool createDirectory(const QString &path, const QString &name);
    bool createFile(const QString &path, const QString &name);
    bool deleteItem(const QString &path);
    QString getCurrentPath() const;
    void setCurrentPath(const QString &path);
    QString getHomePath() const;
    QString getParentPath(const QString &path) const;

signals:
    void directoryChanged(const QString &path);
    void errorOccurred(const QString &error);

private:
    QString currentPath;
};

#endif // FILEMANAGER_H

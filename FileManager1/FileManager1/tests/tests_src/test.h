#ifndef TEST_H
#define TEST_H

#include <QObject>

class TestMainWindow : public QObject
{
    Q_OBJECT
public:
    TestMainWindow();
    ~TestMainWindow();

private slots:
    void testOpenPath();
    void testGoUp();
    void testDirClick();
    void testFileClick();
};

#endif

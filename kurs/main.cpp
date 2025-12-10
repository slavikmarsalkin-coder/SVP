#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication app(argc, argv);

    app.setApplicationName("File Manager");
    app.setOrganizationName("MyCompany");
    app.setApplicationVersion("1.0");

    MainWindow window;
    window.show();

    return app.exec();
}

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QTranslator translator;
    translator.load(QString("cpp-table_") + QLocale::system().name());
    app.installTranslator(&translator);
    MainWindow w;
    w.show();
    return app.exec();
}

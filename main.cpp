#include "MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.initFunction();
    w.initToolBar();
    w.initRibbon();
    w.showMaximized();
    return a.exec();
}

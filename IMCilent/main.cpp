#include "mainwindow.h"
#include "ckernel.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();
    ckernel kernel;
    return a.exec();
}

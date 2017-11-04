#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::setStyle("fusion");//убрать стиль для того, чтобы использовать setsylesheet на андроид
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

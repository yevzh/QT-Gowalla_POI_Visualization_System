#include "mainwindow.h"
#include "include.h"
#include <QApplication>
#include<QMetaType>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<range>("range");
    qRegisterMetaType<userComparison>("userComparison");
    qRegisterMetaType<locationComparison>("locationComparison");
    qRegisterMetaType<pred>("pred");
    qRegisterMetaType<QVector<int>>("QVector<int>");
    MainWindow w;
    w.resize(800,550);
    w.show();

    return a.exec();
}

#include "DeltaEMainwindow.h"
#include "global_define.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DeltaEMainWindow w;
    w.setWindowTitle(QString(VERSION));
    w.show();

    return a.exec();
}

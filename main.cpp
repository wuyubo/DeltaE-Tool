#include "DeltaEMainwindow.h"
#include "global.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DeltaEMainWindow w;
    w.setWindowTitle(QString(VERSION));
    w.setWindowIcon(QIcon("cvte.ico"));
    w.show();

    return a.exec();
}

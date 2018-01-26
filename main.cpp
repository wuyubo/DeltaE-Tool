#include "DeltaEMainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DeltaEMainWindow w;
    w.show();

    return a.exec();
}

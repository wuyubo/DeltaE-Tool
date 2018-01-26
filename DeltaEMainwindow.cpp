#include "DeltaEMainwindow.h"
#include "ui_DeltaEMainwindow.h"

DeltaEMainWindow::DeltaEMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DeltaEMainWindow)
{
    ui->setupUi(this);
}

DeltaEMainWindow::~DeltaEMainWindow()
{
    delete ui;
}

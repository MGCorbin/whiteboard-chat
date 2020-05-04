#include "receivewindow.h"
#include "ui_receivewindow.h"

ReceiveWindow::ReceiveWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ReceiveWindow)
{
    ui->setupUi(this);
}

ReceiveWindow::~ReceiveWindow()
{
    delete ui;
}

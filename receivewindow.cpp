#include <QPainter>
#include <QDebug>

#include "receivewindow.h"
#include "ui_receivewindow.h"

ReceiveWindow::ReceiveWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ReceiveWindow), m_ViewArea(new ViewArea(this))
{
    ui->setupUi(this);

    setCentralWidget(m_ViewArea);
    this->setWindowTitle("Receive Window");

}

ReceiveWindow::~ReceiveWindow()
{
    delete ui;
}


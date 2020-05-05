#include <QPainter>

#include "receivewindow.h"
#include "ui_receivewindow.h"

ReceiveWindow::ReceiveWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ReceiveWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Receive Window");
}

ReceiveWindow::~ReceiveWindow()
{
    delete ui;
}

void ReceiveWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);                 // instanitate a QPainter that will draw to the current widget
    painter.drawPixmap(0, 0, m_Pixmap);     // draw the saved pixmap
}

void ReceiveWindow::setDrawData(QByteArray &arr)
{
    m_DrawData = arr;
    m_Pixmap.loadFromData(m_DrawData, "PNG");
    update();
}

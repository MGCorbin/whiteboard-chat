#include <QPainter>

#include "viewarea.h"

ViewArea::ViewArea(QWidget *parent)
    : QWidget(parent), m_Pixmap(800, 600)
{
    m_Pixmap.fill(Qt::white);
}


void ViewArea::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);                 // instanitate a QPainter that will draw to the current widget
    painter.drawPixmap(0, 0, m_Pixmap);     // draw the saved pixmap
}

void ViewArea::drawLine(const draw_data_t &dat)
{
    QPainter painter(&m_Pixmap);

    painter.setPen(QPen(dat.col, 3));
    painter.drawLine(dat.line);

    update();
}

void ViewArea::clearScreen(const QColor &c)
{
    m_Pixmap.fill(c);
    update();
}

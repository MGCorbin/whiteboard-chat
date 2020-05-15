#include <QMouseEvent>
#include <QPainter>

#include "drawarea.h"

DrawArea::DrawArea(QWidget *parent)
    : QWidget(parent), m_PenColour(Qt::black), m_Pixmap(800, 600)
{
    m_Pixmap.fill(Qt::white);
}

DrawArea::~DrawArea()
{

}

void DrawArea::clearDrawArea()
{
    m_Pixmap.fill(Qt::white);
    update();
}

void DrawArea::setPenColour(const QColor &c)
{
    m_PenColour = c;
}


void DrawArea::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);                 // instanitate a QPainter that will draw to the current widget
    painter.drawPixmap(0, 0, m_Pixmap);     // draw the saved pixmap
}

void DrawArea::mousePressEvent(QMouseEvent *event)
{
    m_OldPoint = event->pos();
}

void DrawArea::mouseMoveEvent(QMouseEvent *event)
{
    static int counter = 0;

    QPainter painter(&m_Pixmap);                    // instatiate a QPaitner that will draw to the Pixmap
    painter.setPen(QPen(m_PenColour, 3));           // set what colour the pen is based on m_PenColour
    painter.drawLine(m_OldPoint, event->pos());     // draw a line between the old mouse position and the new one

    QRect rect(m_OldPoint, event->pos());
    rect = rect.normalized();
    update(rect.x() - 1, rect.y() - 1, rect.width() + 2, rect.height() + 2);        // only update the area that has been drawn to to save us re drawing the entire pixmap

    counter ++;
    if(counter == 100)
    {
//        emit pixmapUpdated(m_Pixmap);
        counter = 0;
    }

    m_OldPoint = event->pos();
}

void DrawArea::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    emit pixmapUpdated(m_Pixmap);
}

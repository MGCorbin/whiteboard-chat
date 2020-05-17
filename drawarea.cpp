#include <QMouseEvent>
#include <QPainter>
#include <QDebug>

#include "drawarea.h"

DrawArea::DrawArea(QWidget *parent)
    : QWidget(parent), m_PenColour(Qt::black), m_BackgroundColour(Qt::white), m_Pixmap(800, 600)
{
    m_Pixmap.fill(Qt::white);
}

DrawArea::~DrawArea()
{

}

void DrawArea::clearDrawArea()
{
    m_Pixmap.fill(m_BackgroundColour);
    emit screenCleared(m_BackgroundColour);
    update();
}

void DrawArea::setPenColour(const QColor &c)
{
    m_PenColour = c;
}

void DrawArea::setBackgroundColour(const QColor &c)
{
    m_BackgroundColour = c;
    clearDrawArea();
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
    m_DrawData.col = m_PenColour;
    m_DrawData.line.setPoints(m_OldPoint, event->pos());

//    qDebug() << "x1: " << m_OldPoint.x() << "y1: " << m_OldPoint.y() << "x2: " << event->x() << "y2: " << event->y();

    QPainter painter(&m_Pixmap);                    // instatiate a QPaitner that will draw to the Pixmap
    painter.setPen(QPen(m_PenColour, 3));           // set what colour the pen is based on m_PenColour
    painter.drawLine(m_DrawData.line);     // draw a line between the old mouse position and the new one

    emit lineDrawn(m_DrawData);

    QRect rect(m_OldPoint, event->pos());
    rect = rect.normalized();
    update(rect.x() - 1, rect.y() - 1, rect.width() + 2, rect.height() + 2);        // only update the area that has been drawn to to save us re drawing the entire pixmap

    m_OldPoint = event->pos();

    QLine line(0, 0, 0, 0);
}

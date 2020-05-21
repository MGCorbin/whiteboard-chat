#include <QMouseEvent>
#include <QPainter>
#include <QDebug>

#include "drawarea.h"

DrawArea::DrawArea(QWidget *parent)
    : QWidget(parent), m_PenColour(Qt::black), m_PenWidth(3), m_BackgroundColour(Qt::white), m_Pixmap(800, 600)
{
    m_Pixmap.fill(m_BackgroundColour);                                              // fill the pixmap when the window is created- we start with a blank drawArea
}

DrawArea::~DrawArea()
{

}

void DrawArea::clearDrawArea(const QColor &c)
{
    m_BackgroundColour = c;                                                         // set the background colour to what the signal specified
    m_Pixmap.fill(m_BackgroundColour);                                              // fill the pixmap with the colour
    emit drawAreaCleared(m_BackgroundColour);
    update();                                                                       // update the entire view
}

void DrawArea::setPenColour(const QColor &c)
{
    m_PenColour = c;
    emit penColourChanged(m_PenColour);
}

void DrawArea::setPenWidth(const int w)
{
    m_PenWidth = w;
    emit penWidthChanged(m_PenWidth);
}



void DrawArea::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);                                                         // instanitate a QPainter that will draw to the current widget
    painter.drawPixmap(0, 0, m_Pixmap);                                             // draw the saved pixmap

}

void DrawArea::mousePressEvent(QMouseEvent *event)
{
    m_OldPoint = event->pos();
    emit pressed();                                                                 // tell the sendWindow that we have pressed the mouse... drawing has started
}

void DrawArea::mouseMoveEvent(QMouseEvent *event)
{
    QPainter painter(&m_Pixmap);                                                    // instatiate a QPaitner that will draw to the Pixmap
    painter.setPen(QPen(m_PenColour, m_PenWidth, Qt::SolidLine, Qt::RoundCap));     // set what colour the pen is based on m_PenColour
    painter.drawLine(m_OldPoint, event->pos());                                     // draw a line between the old mouse position and the new one

    emit lineDrawn(QLine(m_OldPoint, event->pos()));

    QRect rect(m_OldPoint, event->pos());                                           // set a rectangle around the points that we have drawn
    rect = rect.normalized();
    int temp = (m_PenWidth / 2) + 2;                                                // temp variable to hold how much to adjust each vertex of our rectangle, accounting for pen width
    update(rect.adjusted(-temp, -temp, +temp, +temp));                              // only update the area that has been drawn to to save us re drawing the entire pixmap

    m_OldPoint = event->pos();
}

void DrawArea::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit released();                                                                // tell the sendWindow that we have released the mouse... no longer drawing
}

void DrawArea::resizeEvent(QResizeEvent *event)
{
    m_Pixmap = m_Pixmap.scaled(event->size());                                      // if the window is resized, scale the pixmap accordingly
}

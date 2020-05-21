#include <QPainter>
#include <QResizeEvent>
#include <QDebug>

#include "viewarea.h"

ViewArea::ViewArea(QWidget *parent)
    : QWidget(parent), m_PenColour(Qt::black), m_PenWidth(3), m_BackgroundColour(Qt::white), m_Pixmap(800, 600)
{
    m_Pixmap.fill(m_BackgroundColour);
}


void ViewArea::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);                                             // instanitate a QPainter that will draw to the current widget
    painter.drawPixmap(0, 0, m_Pixmap);                                 // draw the saved pixmap
}

void ViewArea::resizeEvent(QResizeEvent *event)
{
    m_Pixmap = m_Pixmap.scaled(event->size());                          // if there is a resize event, scale the pixmap accordingly
}

void ViewArea::drawLine(const QLine &l)
{
    QPainter painter(&m_Pixmap);

    painter.setPen(QPen(m_PenColour, m_PenWidth, Qt::SolidLine, Qt::RoundCap));
    painter.drawLine(l);

    QRect rect(l.p1(), l.p2());                                         // set a rectangle around the points that we have drawn
    rect = rect.normalized();
    int temp = (m_PenWidth / 2) + 2;                                    // temp variable to hold how much to adjust each vertex of our rectangle, accounting for pen width
    update(rect.adjusted(-temp, -temp, +temp, +temp));                  // only update the area that has been drawn to to save us re drawing the entire pixmap
}

void ViewArea::clearScreen(const QColor &c)
{
    m_BackgroundColour = c;
    m_Pixmap.fill(m_BackgroundColour);
    update();
}

void ViewArea::setPenColour(const QColor &c)
{
    m_PenColour = c;
}

void ViewArea::setPenWidth(int w)
{
    m_PenWidth = w;
}

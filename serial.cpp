#include <QBuffer>
#include <QPixmap>

#include "serial.h"


Serial::Serial(QObject *parent, DrawArea *drawArea, SafeQueue<char> *queuePtr)
    : QObject(parent), m_OldDrawDataSize(0), m_SafeQueue(queuePtr)
{
    connect(drawArea, SIGNAL(pixmapUpdated(QPixmap &)), this, SLOT(updatePixmap(QPixmap &)));
}

Serial::~Serial()
{

}

void Serial::updatePixmap(QPixmap &p)
{
    m_Pixmap = p;
    serialise();
    for(int i = m_OldDrawDataSize; i < m_DrawData.size(); i++)      // initalise i with the old size of the draw data, this prevents us from pushing the same data again and again
    {
        m_SafeQueue->push(i);
    }
    m_OldDrawDataSize = m_DrawData.size();                          // update the old draw data size so we only draw from here next time
}

void Serial::serialise()
{
    QBuffer buffer(&m_DrawData);
    buffer.open(QIODevice::WriteOnly);
    m_Pixmap.save(&buffer, "PNG");
}

#ifndef SERIAL_H
#define SERIAL_H

#include <QObject>

#include "drawarea.h"
#include "safequeue.h"

class Serial : public QObject
{
    Q_OBJECT
public:
    explicit Serial(QObject *parent = nullptr, DrawArea *drawArea = nullptr, SafeQueue<char> *queuePtr = nullptr);
    ~Serial();

public slots:
    void updatePixmap(QPixmap &p);

signals:
    void serialDataUpdated(QByteArray &arr);

private:
    QPixmap m_Pixmap;
    QByteArray m_DrawData;
    int m_OldDrawDataSize;

    SafeQueue<char> *m_SafeQueue;

    void serialise();

};

#endif // SERIAL_H

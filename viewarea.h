#ifndef VIEWAREA_H
#define VIEWAREA_H

#include <QWidget>

#include "typedefs.h"

class ViewArea : public QWidget
{
    Q_OBJECT
public:
    explicit ViewArea(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

public slots:
    void drawLine(const QLine &l);
    void clearScreen(const QColor &c);
    void setPenColour(const QColor &c);
    void setPenWidth(int w);

private:
    QColor m_PenColour;
    int m_PenWidth;
    QColor m_BackgroundColour;
    QPixmap m_Pixmap;

};

#endif // VIEWAREA_H

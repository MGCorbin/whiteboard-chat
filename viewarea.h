#ifndef VIEWAREA_H
#define VIEWAREA_H

#include <QWidget>

#include "typedefs.h"

class ViewArea : public QWidget
{
    Q_OBJECT
public:
    explicit ViewArea(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *event) override;

public slots:
    void drawLine(const draw_data_t &dat);
    void clearScreen(const QColor &c);

private:
    QPixmap m_Pixmap;

};

#endif // VIEWAREA_H

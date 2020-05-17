#ifndef DRAWAREA_H
#define DRAWAREA_H

#include <QWidget>
#include <vector>

#include "typedefs.h"

class DrawArea : public QWidget
{
    Q_OBJECT
public:
    explicit DrawArea(QWidget *parent = nullptr);
    ~DrawArea();

    QColor penColour() const { return m_PenColour; }
    QColor backgroundColour() const { return m_BackgroundColour; }

public slots:
    void clearDrawArea();
    void setPenColour(const QColor &c);
    void setBackgroundColour(const QColor &c);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

signals:
    void lineDrawn(const draw_data_t &l);
    void screenCleared(const QColor &c);

private:
    QColor m_PenColour;
    QColor m_BackgroundColour;
    QPixmap m_Pixmap;
    QPoint m_OldPoint;
    draw_data_t m_DrawData;
};

#endif // DRAWAREA_H

#ifndef DRAWAREA_H
#define DRAWAREA_H

#include <QWidget>
#include <vector>
#include <stack>

#include "typedefs.h"

/*
 * Class adapted from: https://doc.qt.io/qt-5/qtwidgets-widgets-scribble-example.html
 */

class DrawArea : public QWidget
{
    Q_OBJECT
public:
    explicit DrawArea(QWidget *parent = nullptr);
    ~DrawArea();

    QColor penColour() const { return m_PenColour; }
    QColor backgroundColour() const { return m_BackgroundColour; }
    int penWidth() const { return m_PenWidth; }

public slots:
    void clearDrawArea(const QColor &c);
    void setPenColour(const QColor &c);
    void setPenWidth(const int w);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

signals:
    void lineDrawn(const QLine &l);
    void drawAreaCleared(const QColor &c);
    void penColourChanged(const QColor &c);
    void penWidthChanged(const int w);
    void pressed();
    void released();

private:
    QColor m_PenColour;
    int m_PenWidth;
    QColor m_BackgroundColour;
    QPixmap m_Pixmap;
    QPoint m_OldPoint;
};

#endif // DRAWAREA_H

#ifndef DRAWAREA_H
#define DRAWAREA_H

#include <QWidget>

class DrawArea : public QWidget
{
    Q_OBJECT
public:
    explicit DrawArea(QWidget *parent = nullptr);
    ~DrawArea();

    QColor penColour() const { return m_PenColour; }

public slots:
    void clearDrawArea();
    void setPenColour(const QColor &c);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

signals:
    void pixmapUpdated(QPixmap &p);

private:
    QColor m_PenColour;
    QPixmap m_Pixmap;       //&& will probably have to change this for something wil less overhead...
    QPoint m_OldPoint;

};

#endif // DRAWAREA_H

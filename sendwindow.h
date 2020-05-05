#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>

#include "drawarea.h"

namespace Ui {
class Window;
}

class SendWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit SendWindow(QWidget *parent = 0);
    ~SendWindow();

    DrawArea* drawAreaPtr() const { return m_DrawArea; }

public slots:
    void handleToolbarAction(QAction *Action);

signals:
    void clearScreen();
    void changePenColour(const QColor &c);

private:
    Ui::Window *ui;

    DrawArea* m_DrawArea;
};

#endif // WINDOW_H

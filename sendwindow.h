#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>

#include "drawarea.h"

#define MESSAGE_TIMEOUT     2000        // time in ms before message dissapears from status bar

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

private slots:
    void handleToolbarAction(QAction *Action);
    void handleMenuBarAction(QAction *Action);
    void handleDrawAreaFeedback();

signals:
    void clearScreen(const QColor &c);
    void changePenColour(const QColor &c);
    void changePenWidth(const int w);

private:
    Ui::Window *ui;

    DrawArea* m_DrawArea;
};

#endif // WINDOW_H

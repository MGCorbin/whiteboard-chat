#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>

namespace Ui {
class Window;
}

class SendWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit SendWindow(QWidget *parent = 0);
    ~SendWindow();
    
private:
    Ui::Window *ui;

    QToolBar* m_toolbar;
};

#endif // WINDOW_H

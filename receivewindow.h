#ifndef RECEIVEWINDOW_H
#define RECEIVEWINDOW_H

#include <QMainWindow>

namespace Ui {
class ReceiveWindow;
}

class ReceiveWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ReceiveWindow(QWidget *parent = nullptr);
    ~ReceiveWindow();

private:
    Ui::ReceiveWindow *ui;
};

#endif // RECEIVEWINDOW_H

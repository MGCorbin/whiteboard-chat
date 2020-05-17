#ifndef RECEIVEWINDOW_H
#define RECEIVEWINDOW_H

#include <QMainWindow>

#include "typedefs.h"
#include "viewarea.h"


namespace Ui {
class ReceiveWindow;
}

class ReceiveWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ReceiveWindow(QWidget *parent = nullptr);
    ~ReceiveWindow();

    ViewArea *viewAreaPtr() const { return m_ViewArea; }

private:
    Ui::ReceiveWindow *ui;

    ViewArea *m_ViewArea;
};

#endif // RECEIVEWINDOW_H

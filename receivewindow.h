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

    void paintEvent(QPaintEvent *event) override;

public slots:
    void setDrawData(QByteArray &arr);

private:
    Ui::ReceiveWindow *ui;

    QPixmap m_Pixmap;
    QByteArray m_DrawData;
};

#endif // RECEIVEWINDOW_H

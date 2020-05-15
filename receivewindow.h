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
    void updatePixmap(const QPixmap &p);
    void updateArray(const QByteArray &arr);

private:
    Ui::ReceiveWindow *ui;

    QPixmap m_Pixmap;
};

#endif // RECEIVEWINDOW_H

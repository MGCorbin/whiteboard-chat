#include "sendwindow.h"
#include "ui_sendwindow.h"

SendWindow::SendWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Window)
{
    ui->setupUi(this);

    m_toolbar = this->addToolBar(tr("Tools"));

    m_toolbar->addAction(tr("Clear"));
    m_toolbar->addAction(tr("Pen Colour"));

    setCentralWidget()
}

SendWindow::~SendWindow()
{
    delete ui;
}

#include <QDebug>
#include <QColorDialog>

#include "sendwindow.h"
#include "ui_sendwindow.h"

SendWindow::SendWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Window), m_DrawArea(new DrawArea(this))
{
    ui->setupUi(this);

    /* SendWindow::handleToolbarAction function is called when a button on the tool bar is pressed */
    connect(ui->mainToolBar, SIGNAL(actionTriggered(QAction*)), this, SLOT(handleToolbarAction(QAction*)));

    /* connect clearScreen signal to DrawAreas clearDrawArea slot (enables user to clear the screen...) */
    connect(this, SIGNAL(clearScreen()), m_DrawArea, SLOT(clearDrawArea()));

    /* connect changePenColour signal to DrawAreas setPenColour slot (enables user to set pen colour...) */
    connect(this, SIGNAL(changePenColour(const QColor &)), m_DrawArea, SLOT(setPenColour(const QColor &)));

    setCentralWidget(m_DrawArea);

    this->setWindowTitle("Send Window");
}

SendWindow::~SendWindow()
{
    delete ui;
    delete m_DrawArea;
}

void SendWindow::handleToolbarAction(QAction *Action)
{
    if(Action == ui->actionClear)
    {
        emit clearScreen();
    }
    else if(Action == ui->actionPenColour)
    {
        QColor newCol = QColorDialog::getColor(m_DrawArea->penColour(), this);
        emit changePenColour(newCol);
    }
}

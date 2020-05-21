#include <QDebug>
#include <QColorDialog>
#include <QInputDialog>
#include <QMessageBox>

#include "sendwindow.h"
#include "ui_sendwindow.h"

SendWindow::SendWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::Window), m_DrawArea(new DrawArea)
{
    ui->setupUi(this);

    /* connect the signals emitted when a user presses a toolbar button to the correct slot for the draw area to deal with */
    connect(this, SIGNAL(clearScreen(const QColor &)), m_DrawArea, SLOT(clearDrawArea(const QColor &)));
    connect(this, SIGNAL(changePenColour(const QColor &)), m_DrawArea, SLOT(setPenColour(const QColor &)));
    connect(this, SIGNAL(changePenWidth(const int)), m_DrawArea, SLOT(setPenWidth(const int)));

    /* SendWindow::handleToolbarAction method is called when a button on the tool bar is pressed */
    connect(ui->mainToolBar, SIGNAL(actionTriggered(QAction*)), this, SLOT(handleToolbarAction(QAction*)));

    /* SendWindow::handleMenuBarAction method is called when a button on the menu bar is pressed */
    connect(ui->menuBar, SIGNAL(triggered(QAction*)), this, SLOT(handleMenuBarAction(QAction*)));

    /* connect all the ui actions to the handleHover slot, means we can display text of what the user is hovering over in the statusBar */
    connect(ui->actionClear, SIGNAL(hovered()), this, SLOT(handleDrawAreaFeedback()));
    connect(ui->actionPenColour, SIGNAL(hovered()), this, SLOT(handleDrawAreaFeedback()));
    connect(ui->actionBackgroundColour, SIGNAL(hovered()), this, SLOT(handleDrawAreaFeedback()));

    /* connect the pressed and released signal from the DrawArea to the handle signals slot, lets us say when we are drawing or not */
    connect(m_DrawArea, SIGNAL(pressed()), this, SLOT(handleDrawAreaFeedback()));
    connect(m_DrawArea, SIGNAL(released()), this, SLOT(handleDrawAreaFeedback()))
;
    setCentralWidget(m_DrawArea);

    this->setWindowTitle("Send Window");

    ui->statusBar->showMessage(tr("Ready!"));
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
        emit clearScreen(m_DrawArea->backgroundColour());
    }
    else if(Action == ui->actionPenColour)
    {
        QColor newPenCol = QColorDialog::getColor(m_DrawArea->penColour(), this);
        emit changePenColour(newPenCol);
    }
    else if(Action == ui->actionBackgroundColour)
    {
        QColor newBgCol = QColorDialog::getColor(m_DrawArea->backgroundColour(), this);
        emit clearScreen(newBgCol);
    }
    else if(Action == ui->actionPenWidth)
    {
        bool ok;
        int newWidth = QInputDialog::getInt(this, tr("Set Pen Width"), tr("Pen width:"), m_DrawArea->penWidth(), 1, 30, 1, &ok);
        emit changePenWidth(newWidth);
    }
}

void SendWindow::handleMenuBarAction(QAction *Action)
{
    if(Action == ui->actionAbout)
    {
        QMessageBox about(this);
        about.setText(tr("About Lab P20"));
        about.setInformativeText(tr("This is a GUI program that allows users to draw in one window, the "
                                    "data is then serialsed and send to the other window using global booleans.\n\n"
                                    "Written by: Marcus Corbin \n"
                                    "Icons made by Freepik from www.flaticon.com"));
        about.exec();
    }
}

void SendWindow::handleDrawAreaFeedback()
{
    QObject *obj = sender();

    /* these if statements check who the sender was and take action accordingly */
    if(obj == ui->actionClear)
    {
        ui->statusBar->showMessage(tr("Clear Screen"), MESSAGE_TIMEOUT);    // show a temporary message
    }
    else if(obj == ui->actionPenColour)
    {
        ui->statusBar->showMessage(tr("Set Pen Colour"), MESSAGE_TIMEOUT);
    }
    else if(obj == ui->actionBackgroundColour)
    {
        ui->statusBar->showMessage(tr("Set Background Colour"), MESSAGE_TIMEOUT);
    }
    else if(obj == ui->actionPenWidth)
    {
        ui->statusBar->showMessage(tr("Set Pen Width"), MESSAGE_TIMEOUT);
    }
    else if(obj == m_DrawArea)                                              // if the signal has come from the draw area...
    {
        static bool toggle;
        toggle = !toggle;                                                   // toggle a bool each time

        if(toggle)                                                          // if the toggle is true, we know the user has pressed the mouse
        {
            ui->statusBar->showMessage(tr("Drawing!"));                     // display drawing in the status bar
        }
        else                                                                // if the toggle is false, we know the user has released the mouse
        {
            ui->statusBar->showMessage(tr("Ready!"));                       // display the default message
        }
    }
}



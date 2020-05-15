#include <QApplication>
#include <QDebug>

#include <unistd.h>

#include "typedefs.h"
#include "sendwindow.h"
#include "receivewindow.h"
#include "test.h"
#include "sender.h"
#include "receiver.h"


void* send(void* sender_ptr)
{
    Sender *sender = (Sender*)sender_ptr;

    qDebug() << "Send thread started!";

    while(1)
    {
        sender->send();
    }

    pthread_exit(NULL);
}

void* receive(void* receiver_ptr)
{
    Receiver *receiver = (Receiver*)receiver_ptr;

    qDebug() << "Receive thread started!";

    while(1)
    {
        receiver->receive();
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    /* setup Qt GUI */
    QApplication a(argc, argv);
    SendWindow sendWindow;
    ReceiveWindow receiveWindow;

    comms_signals_t comms_signals;
    pthread_mutex_init(&comms_signals.comms_mutex, NULL);

    Sender sender(sendWindow.drawAreaPtr(), &comms_signals);
    Receiver receiver(&receiveWindow, &comms_signals);

    sendWindow.show();
    receiveWindow.show();


    /* starting worker thread(s) */
    int rc;
    pthread_t send_thread;
    rc = pthread_create(&send_thread, NULL, send, (void*)&sender);
    if (rc) {
        qDebug() << "Unable to start send thread.";
        exit(1);
    }

    pthread_t receive_thread;
    rc = pthread_create(&receive_thread, NULL, receive, (void*)&receiver);
    if (rc) {
        qDebug() << "Unable to start receive thread.";
        exit(1);
    }

    /* start window event loop */
    qDebug() << "Starting event loop...";
    int ret = a.exec();
    qDebug() << "Event loop stopped.";

    /* cleanup pthreads */
    pthread_exit(NULL);

    /* exit */
    return ret;
}

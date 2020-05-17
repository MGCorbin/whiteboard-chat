/*
 * LAB P20
 * Written by: Marcus Corbin
 * Email: mgc1g18@soton.ac.uk
 *
 * <TO DO>
 * Update completed list!
 * Do something with status bar...
 * Add saving files etc
 * Checksum or parity bit?
 * Resize events
 * Erasing?
 *
 * <COMPLETED>
 */

#include <QApplication>
#include <QDebug>

#include <unistd.h>

#include "typedefs.h"
#include "sendwindow.h"
#include "receivewindow.h"
#include "test.h"
#include "sender.h"
#include "receiver.h"

namespace Comms {
    volatile bool send_receive(false), data(false);
}

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

    qRegisterMetaType<draw_data_t>();

    pthread_mutex_t comms_mutex;
    pthread_mutex_init(&comms_mutex, NULL);

    Sender sender(sendWindow.drawAreaPtr(), &comms_mutex);
    Receiver receiver(receiveWindow.viewAreaPtr(), &comms_mutex);

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

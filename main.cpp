/*
 * LAB P20
 * Written by: Marcus Corbin
 * Email: mgc1g18@soton.ac.uk
 * Icons made by Freepik from www.flaticon.com
 * They are also credited in the "About" section (on the menu bar)
 *
 * <FUTURE>
 * Add saving files?
 *
 * <COMPLETED>
 * Changed toolbar text into icons
 * About secion added & freepik credited
 * Resize Events
 * Checksum added
 * Stop bool added to ensure program terminates correctly
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
    volatile bool sending(false), received(false), data(false);     // 'GPIO' pins used for communcation
    volatile bool enable(true);                                     // used as an enable to ensure correct termination of program
}

/* LOCAL FUNCTIONS */
void* send(void* sender_ptr);
void* receive(void* receiver_ptr);
/* -------------- */

int main(int argc, char *argv[])
{
    /* setup Qt GUI */
    QApplication a(argc, argv);
    SendWindow sendWindow;
    ReceiveWindow receiveWindow;

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

    Comms::enable = false;                                 // once the event loop has finished, we want comms to stop

    pthread_join(send_thread, NULL);
    pthread_join(receive_thread, NULL);

    /* exit */
    return ret;
}

void* send(void* sender_ptr)
{
    Sender *sender = (Sender*)sender_ptr;

    qDebug() << "Send thread started!";

    while(Comms::enable)
    {
        sender->send();
    }

    qDebug() << "Send thread ending!";

    pthread_exit(NULL);
}

void* receive(void* receiver_ptr)
{
    Receiver *receiver = (Receiver*)receiver_ptr;

    qDebug() << "Receive thread started!";

    while(Comms::enable)
    {
        receiver->receive();
    }

    qDebug() << "Receive thread ending!";

    pthread_exit(NULL);
}

uint16_t checksum16(uint8_t *data, uint16_t len)            // this function generates a 2 byte checksum used at the end of all our messages
{
    uint16_t sum = 0, i;

    for(i=0; i<len; i++)                                    // loop through 'len' bytes
    {
        sum += data[i];										// add up all bytes in array, up to length 'len'
    }

    return (~sum);                                          // return the ones compliment
}

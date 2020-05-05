#include <QApplication>
#include <QDebug>

#include <pthread.h>
#include <unistd.h>

#include "sendwindow.h"
#include "receivewindow.h"
#include "test.h"
#include "serial.h"
#include "safequeue.h"

volatile bool sending(false), data(false), receiving(false);
static pthread_mutex_t comms_mutex = PTHREAD_MUTEX_INITIALIZER;

/*
 * fucntions is_sending() and is_receiving() let us
 * check values of sending and receiving bools
 * still using mutexes to avoid race conditions
 */
bool is_sending()
{
    bool ret;
    pthread_mutex_lock(&comms_mutex);
    ret = sending;
    pthread_mutex_unlock(&comms_mutex);
    return ret;
}

bool is_receiving()
{
    bool ret;
    pthread_mutex_lock(&comms_mutex);
    ret = receiving;
    pthread_mutex_unlock(&comms_mutex);
    return ret;
}

void* send(void* queue_ptr)
{
    SafeQueue<char> *send_queue = (SafeQueue<char>*)queue_ptr;

    qDebug() << "Receive thread started!";

    while(1)
    {
        if(!send_queue->isEmpty())
        {
            qDebug() << "Queue size:" << send_queue->size();
            char send_byte = send_queue->font();
//            qDebug() << "Send byte: " << send_byte;

            for(int i=0; i<8; i++)
            {
                pthread_mutex_lock(&comms_mutex);
                data = (send_byte & (1 << i));
//                qDebug() << "S(" << i << "): " << data;
                pthread_mutex_unlock(&comms_mutex);

                pthread_mutex_lock(&comms_mutex);
                sending = true;
                pthread_mutex_unlock(&comms_mutex);

                while(is_receiving() == false);         // wait until the recieve thread has got the data

                pthread_mutex_lock(&comms_mutex);
                sending = false;                        // stop sending, as the recieve thread has got the data
                pthread_mutex_unlock(&comms_mutex);

                while(is_receiving() == true);          // wait until recieve thread has finsihed recieving...

            }
            send_queue->pop();                          // once we have sent our byte, pop if off the queue so we can send the next one
        }
    }

    // end thread
    pthread_exit(NULL);
}

void* receive(void* queue_ptr)
{
    SafeQueue<char> *receive_queue = (SafeQueue<char>*)queue_ptr;

    qDebug() << "Send thread started!";

    while(1)
    {
        char receive_byte = 0;
        for(int i=0; i<8; i++)
        {
            pthread_mutex_lock(&comms_mutex);
            receiving = false;                  // initially we are not receiving
            pthread_mutex_unlock(&comms_mutex);

            while(is_sending() == false);       // wait until we see the send flag go high...

            pthread_mutex_lock(&comms_mutex);
            receive_byte |= data << i;          // load in the data to the correct position on the receive byte
//            qDebug() << "R<" << i << ">: " << data;
            pthread_mutex_unlock(&comms_mutex);

            pthread_mutex_lock(&comms_mutex);
            receiving = true;                   // once we have got the data we can tell the send thread that we have received
            pthread_mutex_unlock(&comms_mutex);

            while(is_sending() == true);        // wait until we see the send flag go low
        }
//        qDebug() << "Received byte: " << receive_byte;
//        qDebug() << " ";

        if(receive_byte != 'A')
        {

        }
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    /* setup Qt GUI */
    QApplication a(argc, argv);
    SendWindow sendWindow;
    ReceiveWindow receiveWindow;

    sendWindow.show();
    receiveWindow.show();

    SafeQueue<char> sendQueue;
    SafeQueue<char> receiveQueue;

    Serial serial(nullptr, sendWindow.drawAreaPtr(), &sendQueue);

    /* starting worker thread(s) */
    int rc;
    pthread_t send_thread;
    rc = pthread_create(&send_thread, NULL, send, (void*)&sendQueue);
    if (rc) {
        qDebug() << "Unable to start send thread.";
        exit(1);
    }

    pthread_t receive_thread;
    rc = pthread_create(&receive_thread, NULL, receive, (void*)1);
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

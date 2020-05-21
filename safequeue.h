#ifndef SAFEQUEUE_H
#define SAFEQUEUE_H

#include <queue>
#include <pthread.h>
#include <QDebug>

/*
 * https://stackoverflow.com/questions/31144949/thread-safe-queue-with-pthreads was used as a reference when making this class
 */

template <typename T>
class SafeQueue
{
public:
    SafeQueue();
    ~SafeQueue();

    void push(T data);
    void pop();
    T front();

    bool isEmpty();
    int size();
    void print();

private:
    std::queue<T> m_Queue;
    pthread_mutex_t m_Mutex;

};

template <typename T>
SafeQueue<T>::SafeQueue()
{
    pthread_mutex_init(&m_Mutex, NULL);
}

template <typename T>
SafeQueue<T>::~SafeQueue()
{
    pthread_mutex_destroy(&m_Mutex);
}

template <typename T>
void SafeQueue<T>::push(T data)
{
    pthread_mutex_lock(&m_Mutex);
    m_Queue.push(data);
    pthread_mutex_unlock(&m_Mutex);
}

template <typename T>
void SafeQueue<T>::pop()
{
    pthread_mutex_lock(&m_Mutex);
    m_Queue.pop();
    pthread_mutex_unlock(&m_Mutex);
}

template <typename T>
T SafeQueue<T>::front()
{
    T ret;                          // make a dummy vairable for the return value
    pthread_mutex_lock(&m_Mutex);
    ret = m_Queue.front();          // must load our dummy variable otherwise we will return before we unlock the mutex
    pthread_mutex_unlock(&m_Mutex);
    return ret;
}

template <typename T>
bool SafeQueue<T>::isEmpty()
{
    bool ret;                       // make a dummy vairable for the return value
    pthread_mutex_lock(&m_Mutex);
    ret = m_Queue.empty();          // must load our dummy variable otherwise we will return before we unlock the mutex
    pthread_mutex_unlock(&m_Mutex);
    return ret;
}

template <typename T>
int SafeQueue<T>::size()
{
    int ret;                        // make a dummy vairable for the return value
    pthread_mutex_lock(&m_Mutex);
    ret = m_Queue.size();           // must load our dummy variable otherwise we will return before we unlock the mutex
    pthread_mutex_unlock(&m_Mutex);
    return ret;
}

template <typename T>
void SafeQueue<T>::print()          // only used for debug, very inefficient
{
    pthread_mutex_lock(&m_Mutex);
    std::queue<T> q = m_Queue;
    pthread_mutex_unlock(&m_Mutex);
    while(q.size() != 0)
    {
        qDebug() << q.front();
        q.pop();
    }
}

#endif // SAFEQUEUE_H

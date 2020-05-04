#ifndef SAFEQUEUE_H
#define SAFEQUEUE_H

#include <queue>
#include <pthread.h>
#include <QDebug>

template <typename T>
class SafeQueue
{
public:
    SafeQueue();
    ~SafeQueue();

    void push(T data);
    void pop();
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
bool SafeQueue<T>::isEmpty()
{
    bool ret;
    pthread_mutex_lock(&m_Mutex);
    ret = m_Queue.empty();          // must make a copy otherwise we will return before we unlock the mutex
    pthread_mutex_unlock(&m_Mutex);
    return ret;
}

template <typename T>
int SafeQueue<T>::size()
{
    int ret;
    pthread_mutex_lock(&m_Mutex);
    ret = m_Queue.size();
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

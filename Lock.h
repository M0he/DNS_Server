#ifndef LOCKER_H
#define LOCKER_H
#include <pthread.h>
#include <exception>
#include <semaphore.h>
//锁
class Lock {
    private:
    pthread_mutex_t m_mutex;
    public:
    Lock();
    ~Lock();
    bool lock();
    bool unlock();
    pthread_mutex_t * get_mutex();
};
//信号量
class Sem {
    private:
    sem_t m_sem;
    public:
    Sem();
    Sem(int number);
    ~Sem();
    bool wait();
    bool post();
};
#endif
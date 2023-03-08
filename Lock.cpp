#include "Lock.h"
    Lock::Lock() {
        int result = pthread_mutex_init(&m_mutex, NULL);
        if (result != 0 ) {
            throw std::exception();
        }
    }
    Lock::~Lock(){
        pthread_mutex_destroy(&m_mutex);
    }
    bool Lock::lock() {
        return pthread_mutex_lock(&m_mutex) == 0;
    }
    bool Lock::unlock() {
        return pthread_mutex_unlock(&m_mutex) ==0;
    }
    pthread_mutex_t * Lock::get_mutex() {
        return &m_mutex;
    }

    Sem::Sem() {
        int result = sem_init(&m_sem, 0, 0);
        if (result != 0) {
            throw std::exception();
        }
    }
    Sem::Sem(int number) {
        int result = sem_init(&m_sem, 0, number);
        if (result != 0) {
            throw std::exception();
        }
    }
    Sem::~Sem() {
        sem_destroy(&m_sem);
    }
    //信号量加1
    bool Sem::wait() {
        return sem_wait(&m_sem) == 0;
    }
    //信号量减1
    bool Sem::post() {
        return sem_post(&m_sem) == 0;
    }

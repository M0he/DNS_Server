#ifndef THREADPOOL_H
#define THREADPOOL_H
#include "Lock.h"
#include<iostream>
#include<stdio.h>
using namespace std;
#include <list>
template<typename T>
class ThreadPool {
    private:
    Lock m_list_lock;//互斥锁
    Sem m_list_sem;//信号量
    pthread_t *m_threads;//线程数组
    std::list<T *> work_list;//待处理工作链表
    bool m_stop;//是否停止
    int threads_number;//线程数量

    private:
    static void *work(void * arg);//线程工作的函数，相当于一个回调
    void run();//线程运行

    public:
    ThreadPool(int number = 8);
    ~ThreadPool();
    bool list_append(T *request);
};
template<typename T>
ThreadPool<T>::ThreadPool(int number) {
    threads_number = number;
    m_stop = false;
    m_threads = NULL;
    if (number <= 0) {
        throw std::exception();
    }
    m_threads = new pthread_t[threads_number];
    if (! m_threads) {
        throw std::exception();
    }
    for (int i = 0; i < threads_number; i++) {
        printf("创建线程%d\n",i + 1);
        if (pthread_create(m_threads + i, NULL, work, this) != 0) {
            delete [] m_threads;
            throw std::exception();
        }
        if (pthread_detach( m_threads[i] ) != 0) {
            delete [] m_threads;
            throw std::exception();
        }
    }
}
template<typename T>
ThreadPool<T>::~ThreadPool() {
    delete [] m_threads;
    m_stop = true;
}
template<typename T>
bool ThreadPool<T>::list_append(T *request) {
    m_list_lock.lock();
    work_list.push_back(request);
    m_list_lock.unlock();
    m_list_sem.post();
    return true;
}
template<typename T>
void ThreadPool<T>::run() {
    while (!m_stop) {
        m_list_sem.wait();
        m_list_lock.lock();
        if (work_list.size() == 0) {
            continue;
        }
       
        T *request = work_list.front();
        work_list.pop_front();
        m_list_lock.unlock();
        if (!request) {
            continue;
        }
        request -> process();
    }
}
template<typename T>
void * ThreadPool<T>::work(void * arg) {
    ThreadPool *pool = (ThreadPool *)arg;
    pool -> run();
    return pool;
}
#endif
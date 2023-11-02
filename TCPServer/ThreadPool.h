#pragma once
#include "TaskQueue.h"
#include <iostream>
#include <condition_variable>
#include <string.h>
#include <string>
#include <chrono>

#define THREAD_DESTROY_NUMBER 2     // The number of threads destroyed at a time
#define THREAD_MAX_NUMBER 10
#define THREAD_MIN_NUMBER 3

class ThreadPool
{
private:
    // task queue
    TaskQueue* taskQ;
    // Thread
    thread managerThread;           // manager thread
    //list<thread> workerThreads;   // worker thread
    int minNum;                     // Minimum number of threads
    int maxNum;                     // Maximum number of threads
    int busyNum;                    // Number of busy threads
    int aliveNum;                   // The number of alive threads
    int exitNum;                    // The number of threads to be destroyed
    // Lock and condition variable
    mutex mutexPool;                // Lock the entire thread pool
    condition_variable notEmpty;    // The task queue is not empty

    bool shutdown;                  // 1 to destroy the thread pool

public:
    //Create a thread pool and initialize it
    ThreadPool();

    //Destroy thread pool
    ~ThreadPool();

    //Add tasks to the thread pool
    void addTask(Task* task);

    //Get the number of busy threads in the thread pool
    int getBusyNum();

    //Get the number of alive threads in the thread pool
    int getAliveNum();

private:
    //Worker thread processing function
    static void workerFunc(void* arg);

    //Manager thread processing function
    static void managerFunc(void* arg);

    //Single thread exits
    void threadExit();


};
#pragma once

#include <Windows.h>
#include <cstdint>
#include "taskqueue.hpp"
#include "thread.hpp"

struct Worker
{
    Worker(uint32_t auiId)
        : muiId(auiId)
    {
        mpThread = new Thread( &workerFunc );
        mpThread->start();
    }
    ~Worker()
    {
        delete mpThread;
    }
    static void workerFunc()
    {
        while (1)
        {
            Task t = mTaskQueue->getNext();
            if (t.fn)
            {
                t.fn(t.data);
            }
            mTaskQueue->markJobDone(t);
        }
    }
    uint32_t muiId;
    Thread* mpThread;
    static TaskQueue* mTaskQueue;
private:
    Worker(const Worker& a);
    Worker& operator=(const Worker& a);
};
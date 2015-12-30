#pragma once

#include <Windows.h>
#include <cstdint>
#include "worker.hpp"
#include <vector>

class MultiProcess{
public:
    MultiProcess()
        : mbWorkInSingleProcess(true)
    {}
    void init(uint32_t auiNbWroker)
    {
        if (auiNbWroker == 0)
        {
            mbWorkInSingleProcess = true;
        }
        else
        {
            mbWorkInSingleProcess = false;
            for (uint32_t i = 0; i < auiNbWroker; ++i)
                mWorkers.push_back(new Worker(i, mTaskQueue));
        }
    }
    ~MultiProcess(){
        for (uint32_t i = 0; i < mWorkers.size(); ++i)
        {
            delete mWorkers[i];
        }
    }
    void addJob(const Task& t)
    {
        if (mbWorkInSingleProcess)
        {
            t.fn(t.data);
        }
        else
        {
            mTaskQueue.addJob(t);
        }
    }
    void flush()
    {
        Task t = mTaskQueue.getNext();
        while (t.fn)
        {
            t.fn(t.data);
            mTaskQueue.markJobDone(t);
            t = mTaskQueue.getNext();
        }
        if (!mbWorkInSingleProcess)
            mTaskQueue.waitForAllDone();
    }
private:
    TaskQueue mTaskQueue;
    std::vector< Worker* > mWorkers;
    bool mbWorkInSingleProcess; // if work in single process, job are executed when added. otherwise they are put in the job queue.

    MultiProcess(const MultiProcess&a);
    MultiProcess& operator=(const MultiProcess&a);
};
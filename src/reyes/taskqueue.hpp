#pragma once

#include "mutex.hpp"
#include "event.hpp"
#include "task.hpp"
#include <list>

struct TaskQueue
{
    TaskQueue()
        : mNbWorkingJob(0)
    {}
    void addJob(const Task& t)
    {
        Mutex::Lock lock(mJobMutex);
        mAllDone.reset();
        mJobs.push_back(t);
    }

    Task getNext()
    {
        Mutex::Lock lock(mJobMutex);
        if (mJobs.size()>0)
        {
            Task t = mJobs.front();
            mJobs.pop_front();
            _InterlockedIncrement(&mNbWorkingJob);
            return t;
        }
        else
        {
            return Task();
        }
    }

    // called when a job is done
    void markJobDone(Task& t)
    {
        Mutex::Lock lock(mJobMutex);
        long bbWorking = _InterlockedDecrement(&mNbWorkingJob);
        if (bbWorking <= 0 && mJobs.size() == 0)
        {
            mAllDone.raise();
        }
    }

    // waits until all job are done
    void waitForAllDone()
    {
        mAllDone.wait();
    }

private:
    TaskQueue(const TaskQueue&);
    TaskQueue& operator=(const TaskQueue&);

    std::list<Task> mJobs;
    Mutex mJobMutex;
    long mNbWorkingJob; // number of job currently executing at the moment
    Event mAllDone;
};
#pragma once

#include <Windows.h>
#include <intrin.h>
#include <cstdint>
#include <list>

inline void globalAssert(bool aCond){
    if (!aCond){
#if _MSC_VER // VC
#if _M_X64
        __debugbreak();
#else
        _asm { int 3 }
#endif
#endif
    }
}
#define GLIB_ASSERT(cond) globalAssert(cond)

#define AAJOB_ENTRY_POINT(_fnName) void _fnName(void* jobDataPtr)

namespace aajob
{
    // ------------------------------------------------------------------------- Event
    struct Event
    {
        Event()
            : mHandle(CreateEvent(nullptr, true, false, nullptr))
        {}
        ~Event()
        {
            CloseHandle(mHandle);
        }

        void raise() { SetEvent(mHandle); }
        void reset() { ResetEvent(mHandle); }
        void wait()
        {
            DWORD res = WaitForSingleObject(mHandle, INFINITE);
            GLIB_ASSERT(res == WAIT_OBJECT_0);
        }
        bool isRaised()
        {
            DWORD res = WaitForSingleObject(mHandle, 0);
            if (res == WAIT_OBJECT_0) return true;
            else if (res == WAIT_TIMEOUT) return false;
            else GLIB_ASSERT(false);
        }
    private:
        Event(const Event& a);
        Event& operator=(const Event&a);
        HANDLE mHandle;
    };

    // ------------------------------------------------------------------------- Mutex
    struct Mutex
    {
        Mutex()
        {
            InitializeCriticalSection(&mCs);
        }
        ~Mutex()
        {
            DeleteCriticalSection(&mCs);
        }

        class Lock;
        friend class Lock;

        struct Lock
        {
            explicit Lock(Mutex& m)
            : mMtx(m)
            {
                EnterCriticalSection(&mMtx.mCs);
            }
            ~Lock()
            {
                LeaveCriticalSection(&mMtx.mCs);
            }
        private:
            Mutex& mMtx;
            Lock(Lock const &);
            Lock& operator=(Lock const &);
        };

    private:
        CRITICAL_SECTION mCs;
        Mutex(Mutex const &);
        Mutex & operator=(Mutex const &);
    };

    // ------------------------------------------------------------------------- Thread
    struct Thread
    {
        Thread(void(*fn)())
            : mFunc(fn)
        {
            mHandle = CreateThread(0, 0, &ThreadFunc, this, CREATE_SUSPENDED, &mId);
        }
        void start() { ResumeThread(mHandle); }
        void stop() { SuspendThread(mHandle); }
        ~Thread()
        {
            SuspendThread(mHandle);
            CloseHandle(mHandle);
        }
        static DWORD WINAPI ThreadFunc(void* fn)
        {
            Thread* pT = (Thread*)fn;
            pT->mFunc();
            return 0;
        }
        void(*mFunc)();
        HANDLE mHandle;
        DWORD  mId;

    private:
        Thread(const Thread& t);
        Thread& operator=(const Thread& t);
    };

    // ------------------------------------------------------------------------- JobDecl
    typedef void(JobFunc)(void*);
    typedef JobFunc* JobFuncPtr;

    struct JobDecl
    {
        JobFuncPtr fn;
        void* data;

        JobDecl(JobFunc fnName = 0, void* dataPtr = 0)
            : fn(fnName)
            , data(dataPtr)
        {}
    };

    // ------------------------------------------------------------------------- JobQueue
    struct JobQueue
    {
        JobQueue()
            : mNbWorkingJob(0)
        {}
        void addJob(const JobDecl& _job)
        {
            Mutex::Lock lock(mJobMutex);
            mAllDone.reset();
            mJobs.push_back(_job);
        }
        JobDecl retrieveNextJob()
        {
            Mutex::Lock lock(mJobMutex);
            if (mJobs.size()>0){
                JobDecl job = mJobs.front();
                mJobs.pop_front();
                _InterlockedIncrement(&mNbWorkingJob);
                return job;
            }
            else
            {
                return nullptr;
            }
        }
        // called when a job is done
        void markJobDone(JobDecl& _job)
        {
            Mutex::Lock lock(mJobMutex);
            long bbWorking = _InterlockedDecrement(&mNbWorkingJob);
            if (bbWorking == 0 && mJobs.size() == 0)
            {
                mAllDone.raise();
            }
        }
        // waits until all job are done
        void waitForAllDone()
        {
            mAllDone.wait();
        }
        bool hasWorking()
        {
            Mutex::Lock lock(mJobMutex);
            return mNbWorkingJob>0;
        }
        bool isDone()
        {
            Mutex::Lock lock(mJobMutex);
            return ((mJobs.size() == 0) && (mNbWorkingJob == 0));
        }
    private:
        JobQueue(const JobQueue&);
        JobQueue& operator=(const JobQueue&);
        
        std::list<JobDecl> mJobs;
        Mutex mJobMutex;
        long mNbWorkingJob;
        Event mAllDone;
    };

    static JobQueue g_jobqueue;

    // ------------------------------------------------------------------------- Worker
    struct Worker
    {
        Worker(uint32_t auiId)
            : muiId(auiId)
        {
            mpThread = new Thread(&workerFunc);
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
                JobDecl t = aajob::g_jobqueue.retrieveNextJob();
                if (t.fn)
                {
                    t.fn(t.data);
                    aajob::g_jobqueue.markJobDone(t);
                }
            }
        }
        uint32_t muiId;
        Thread* mpThread;
    private:
        Worker(const Worker& a);
        Worker& operator=(const Worker& a);
    };

    // ------------------------------------------------------------------------- System
    static uint8_t g_workerCnt;
    static Worker** g_workers;

    void RunJob(const JobDecl& _job)
    {
        if (0 == g_workerCnt)
        {
            _job.fn(_job.data);
        }
        else
        {
            g_jobqueue.addJob(_job);
        }
    }

    void Flush(void)
    {
        JobDecl job = g_jobqueue.retrieveNextJob();
        while (job.fn)
        {
            job.fn(job.data);
            g_jobqueue.markJobDone(job);
            job = g_jobqueue.retrieveNextJob();
        }
        if (g_workerCnt>0 && g_jobqueue.hasWorking())
            g_jobqueue.waitForAllDone();
    }

    void Init(uint8_t workerCnt = 2)
    {
        g_workerCnt = workerCnt;
        if (aajob::g_workerCnt > 0)
        {
            aajob::g_workers = new aajob::Worker*[aajob::g_workerCnt];
            for (DWORD i = 0; i < aajob::g_workerCnt; ++i)
            {
                aajob::g_workers[i] = new aajob::Worker(i);
            }
        }
    }

    void Cleanup()
    {
        if (!g_jobqueue.isDone())
            g_jobqueue.waitForAllDone();

        if (aajob::g_workerCnt > 0)
        {
            for (DWORD i = 0; i < aajob::g_workerCnt; ++i)
                delete aajob::g_workers[i];
            delete[] aajob::g_workers;
        }
    }
}

#undef GLIB_ASSERT
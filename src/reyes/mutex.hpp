#pragma once

#include <Windows.h>

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
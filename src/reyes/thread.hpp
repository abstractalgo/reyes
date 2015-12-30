#pragma once

#include <Windows.h>

class Worker;

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
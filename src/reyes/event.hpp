#pragma once

#include <Windows.h>
#include <cassert>

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
        assert(res == WAIT_OBJECT_0);
    }
    bool isRaised()
    {
        DWORD res = WaitForSingleObject(mHandle, 0);
        if (res == WAIT_OBJECT_0) return true;
        else if (res == WAIT_TIMEOUT) return false;
        else assert(false);
    }
private:
    Event(const Event& a);
    Event& operator=(const Event&a);
    HANDLE mHandle;
};
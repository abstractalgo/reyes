#pragma once

#include <Windows.h>

class Thread{
public:
    Thread(const fastdelegate::FastDelegate0<>& a) :mFunc(a){
        mHandle = CreateThread(0, 0, &ThreadFunc, this, CREATE_SUSPENDED, &mId);
    }
    Thread(const Thread&a) :mFunc(a.mFunc){
        mHandle = CreateThread(0, 0, &ThreadFunc, this, CREATE_SUSPENDED, &mId);
    }
    Thread& operator=(const Thread&a){
        SuspendThread(mHandle);
        CloseHandle(mHandle);
        mFunc = a.mFunc;
        mHandle = CreateThread(0, 0, &ThreadFunc, this, CREATE_SUSPENDED, &mId);
    }
    void start(){
        ResumeThread(mHandle);
    }
    void stop(){
        SuspendThread(mHandle);
    }
    ~Thread(){
        SuspendThread(mHandle);
        CloseHandle(mHandle);
    }
    static DWORD WINAPI ThreadFunc(void* a){
        Thread* pT = (Thread*)a;
        pT->mFunc();
        return 0;
    }
    fastdelegate::FastDelegate0<> mFunc;
    HANDLE mHandle;
    DWORD  mId;
};
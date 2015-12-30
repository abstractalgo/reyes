#pragma once

#include <Windows.h>

struct Task
{
    void(*fn)(void*);
    void* data;

    Task()
        : fn(0)
        , data(0)
    {}
};
#pragma once

#include "renderer.hpp"
#include "mem.hpp"

namespace reyes
{
    extern reyes::renderer REYES;
    extern reyes::mem::mAllocator MEMORY;
    extern reyes::mem::StackAllocator< 1 << 24/*16MB*/ > STACK;
}

extern void appMain();
#pragma once

#include "mem.hpp"
#include "aajob.hpp"

namespace reyes
{
    struct Scene
    {
        aajob::Mutex mJobMutex;
        mem::ObjectArray memory;
        uint16_t cnt;

        Scene()
            : cnt(0)
            , memory(SCENEMEM_SIZE)
        {}

        ~Scene()
        {
            assert(0 == cnt);
        }

        mem::blk alloc(size_t size)
        {
            aajob::Mutex::Lock lock(mJobMutex);
            mem::blk block = memory.alloc(size);
            cnt++;
            return block;
        }

        mem::blk pop()
        {
            cnt--;
            return memory.getNext();
        }

        operator bool()
        {
            return cnt > 0;
        }
    };
}
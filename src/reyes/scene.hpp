#include "mem.hpp"

namespace reyes
{
    struct Scene
    {
        mem::Stack<mem::blk, 2048> allocations;
        mem::mAllocator memory;
        uint16_t cnt;

        Scene() : cnt(0)
        {}

        mem::blk alloc(size_t size)
        {
            mem::blk block = memory.alloc(size);
            allocations.push(block);
            cnt++;
            return block;
        }

        mem::blk pop()
        {
            return allocations.pop();
        }

        void free(mem::blk block)
        {
            cnt--;
            memory.free(block);
        }

        operator bool()
        {
            return allocations.top > 0;
        }
    };

    //typedef mem::StaticObjectStack<char, 1 << 20> Scene;
}
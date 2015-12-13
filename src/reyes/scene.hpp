#include "mem.hpp"

namespace reyes
{
    struct Scene
    {
        mem::Stack<mem::blk, 1024> allocations;
        mem::mAllocator memory;

        mem::blk alloc(size_t size)
        {
            mem::blk block = memory.alloc(size);
            allocations.push(block);
            return block;
        }

        mem::blk pop()
        {
            return allocations.pop();
        }

        void free(mem::blk block)
        {
            memory.free(block);
        }

        operator bool()
        {
            return allocations.top > 0;
        }
    };
}
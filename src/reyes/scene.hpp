#include "mem.hpp"

namespace reyes
{
    struct Scene
    {
        mem::ObjectArray memory;
        uint16_t cnt;

        Scene()
            : cnt(0)
            , memory(SCENEMEM_SIZE)
        {}

        mem::blk alloc(size_t size)
        {
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
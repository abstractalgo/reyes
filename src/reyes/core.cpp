#include "mem.hpp"

namespace reyes
{
    namespace globals
    {
        reyes::mem::mAllocator mem;
        reyes::mem::StackAllocator< 1<<23/*8MB*/ > stackmem;
    }
}

using namespace reyes::globals;

extern void appMain();

int main()
{
    appMain();
    return 0;
}
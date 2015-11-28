#include "core.h"
#include <cstdio>

#define REYES_VERSION "0.1"

//reyes::mem::mAllocator reyes::MEMORY;
//reyes::mem::StackAllocator< 1 << 24/*16MB*/ > reyes::STACK;

using namespace reyes;

int main()
{
    printf("REYES renderer v"REYES_VERSION"\n");
    appMain();
    //system("pause");
    return 0;
}
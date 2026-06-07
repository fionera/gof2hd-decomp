#include "class.h"

extern "C" uint64_t ApplicationManager_GetElapsedTimeMillis(ApplicationManager *self)
{
    return *(uint64_t *)((char *)self + 0x70) - *(uint64_t *)((char *)self + 0x78);
}

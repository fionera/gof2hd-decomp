#include "class.h"

extern "C" uint64_t ApplicationManager_GetCurrentTimeMillis(ApplicationManager *self)
{
    return *(uint64_t *)((char *)self + 0x68);
}

#include "class.h"

extern "C" uint64_t ApplicationManager_GetActionState(ApplicationManager *self)
{
    return *(uint64_t *)((char *)self + 0xa0);
}

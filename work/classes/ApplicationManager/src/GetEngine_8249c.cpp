#include "class.h"

extern "C" void *ApplicationManager_GetEngine(ApplicationManager *self)
{
    return *(void **)((char *)self + 0xa8);
}

#include "class.h"

extern "C" void ApplicationManager_ResetKeyState(ApplicationManager *self)
{
    *(uint64_t *)((char *)self + 0x80) = 0;
}

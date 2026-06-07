#include "class.h"

extern "C" void ApplicationManager_CheatEnable(ApplicationManager *self, bool enable)
{
    *(bool *)((char *)self + 0x34) = enable;
}

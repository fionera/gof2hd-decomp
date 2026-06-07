#include "class.h"

extern "C" void ApplicationManager_SoundFxEnable(ApplicationManager *self, bool enable)
{
    *(bool *)((char *)self + 0xb0) = enable;
}

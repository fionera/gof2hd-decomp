#include "class.h"

extern "C" void ApplicationManager_SoundEnable(ApplicationManager *self, bool enable)
{
    *(volatile bool *)((char *)self + 0xb0) = enable;
    *(volatile bool *)((char *)self + 0xb1) = enable;
}

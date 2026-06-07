#include "class.h"

extern "C" void ApplicationManager_VibrateEnable(ApplicationManager *self, bool enable)
{
    *(bool *)((char *)self + 0xb2) = enable;
}

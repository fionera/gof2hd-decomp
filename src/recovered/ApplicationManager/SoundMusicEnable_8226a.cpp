#include "class.h"

extern "C" void ApplicationManager_SoundMusicEnable(ApplicationManager *self, bool enable)
{
    *(bool *)((char *)self + 0xb1) = enable;
}

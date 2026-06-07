#include "class.h"

extern "C" void ext_001ab318(void *engine);

extern "C" void ApplicationManager_VibrateSupported(ApplicationManager *self)
{
    ext_001ab318(*(void **)((char *)self + 0xa8));
}

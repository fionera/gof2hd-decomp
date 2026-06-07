#include "class.h"

typedef void InitializeCallback(Engine *);

extern "C" void Engine_Initialize(Engine *self, InitializeCallback *callback)
{
    if (callback != 0) {
        callback(self);
    }
}

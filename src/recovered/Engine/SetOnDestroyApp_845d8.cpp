#include "class.h"

typedef void DestroyCallback(Engine *);

extern "C" void Engine_SetOnDestroyApp(Engine *self, DestroyCallback *callback)
{
    *(DestroyCallback **)((char *)self + 0x484) = callback;
}

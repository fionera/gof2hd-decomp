#include "class.h"

extern "C" void Engine_SetAddData(Engine *self, void *data, int size)
{
    *(void **)((char *)self + 0x358) = data;
    *(int *)((char *)self + 0x35c) = size;
}

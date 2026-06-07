#include "class.h"

extern "C" uint32_t Engine_GetDisplayWidth(Engine *self)
{
    return *(uint32_t *)((char *)self + 0x368);
}

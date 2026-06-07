#include "class.h"

extern "C" uint32_t Engine_GetDisplayHeight(Engine *self)
{
    return *(uint32_t *)((char *)self + 0x36c);
}

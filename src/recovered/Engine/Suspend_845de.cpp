#include "class.h"

extern "C" void PaintCanvas_Suspend(void *canvas);

extern "C" uint32_t Engine_Suspend(Engine *self)
{
    PaintCanvas_Suspend(**(void ***)((char *)self + 0x30));
    return 1;
}

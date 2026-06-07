#include "class.h"

extern "C" void PaintCanvas_Resume(void *canvas);

extern "C" uint32_t Engine_Resume(Engine *self)
{
    PaintCanvas_Resume(**(void ***)((char *)self + 0x30));
    for (int index = 0; index != 0x14; index += 1) {
        *(int *)((char *)self + 0x7c + index * 4) = -1;
    }
    return 1;
}

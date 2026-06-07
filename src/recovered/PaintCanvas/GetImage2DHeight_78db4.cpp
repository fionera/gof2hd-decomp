#include "class.h"

unsigned short GetImage2DHeight(AbyssEngine::PaintCanvas *self, unsigned int index)
{
    if (index < *(unsigned int *)((char *)self + 0x14c)) {
        char *img = (*(char ***)((char *)self + 0x150))[index];
        return *(unsigned short *)(img + 0x12);
    }
    return 0;
}

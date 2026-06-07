#include "class.h"

void TransformSetColor(AbyssEngine::PaintCanvas *self, unsigned int index, unsigned int color)
{
    if (index < *(unsigned int *)((char *)self + 0x158)) {
        char *obj = (*(char ***)((char *)self + 0x15c))[index];
        *(unsigned int *)(obj + 0x48) = color;
    }
}

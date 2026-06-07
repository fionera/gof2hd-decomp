#include "class.h"

void *MaterialGetMaterial(AbyssEngine::PaintCanvas *self, unsigned int index)
{
    if (index < *(unsigned int *)((char *)self + 0x174)) {
        return (*(void ***)((char *)self + 0x178))[index];
    }
    return 0;
}

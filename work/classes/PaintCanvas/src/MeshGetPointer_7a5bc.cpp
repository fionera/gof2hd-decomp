#include "class.h"

void *MeshGetPointer(AbyssEngine::PaintCanvas *self, unsigned int index)
{
    if (index < *(unsigned int *)((char *)self + 0x24)) {
        return (*(void ***)((char *)self + 0x28))[index];
    }
    return 0;
}

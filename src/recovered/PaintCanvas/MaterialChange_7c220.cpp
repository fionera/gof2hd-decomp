#include "class.h"

void MaterialChange(AbyssEngine::PaintCanvas *self, unsigned int index,
                    unsigned int param3, unsigned int param4)
{
    if (index < *(unsigned int *)((char *)self + 0x174)) {
        unsigned int *mat = (*(unsigned int ***)((char *)self + 0x178))[index];
        mat[8] = param3;
        mat[0] = param4;
    }
}

#include "class.h"

using AbyssEngine::AEMath::Matrix;

void CameraSetLocal(AbyssEngine::PaintCanvas *self, unsigned int index, const Matrix &matrix)
{
    if (index < *(unsigned int *)((char *)self + 0x164)) {
        char *cam = (*(char ***)((char *)self + 0x168))[index];
        *(Matrix *)(cam + 0xc) = matrix;
    }
}

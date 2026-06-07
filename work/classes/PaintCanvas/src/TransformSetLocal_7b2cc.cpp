#include "class.h"

using AbyssEngine::AEMath::Matrix;

void TransformSetLocal(AbyssEngine::PaintCanvas *self, unsigned int index, const Matrix &matrix)
{
    if (index < *(unsigned int *)((char *)self + 0x158)) {
        Matrix *t = (*(Matrix ***)((char *)self + 0x15c))[index];
        *t = matrix;
    }
}

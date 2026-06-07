#include "class.h"

using AbyssEngine::AEMath::Matrix;
using AbyssEngine::AEMath::Vector;

extern "C" void Trail_MatrixTransformVector(Vector *out, const Matrix *matrix, const Vector *vector);

void Trail::update(const Matrix &a, const Matrix &b, const Vector &v)
{
    char storage[sizeof(Vector) * 2];
    Trail_MatrixTransformVector((Vector *)(void *)(storage + sizeof(Vector)), &a, &v);
    Trail_MatrixTransformVector((Vector *)(void *)storage, &b, &v);
    update(((Vector *)(void *)(storage + sizeof(Vector)))->x,
           ((Vector *)(void *)(storage + sizeof(Vector)))->y,
           ((Vector *)(void *)(storage + sizeof(Vector)))->z,
           ((Vector *)(void *)storage)->x,
           ((Vector *)(void *)storage)->y,
           ((Vector *)(void *)storage)->z);
}

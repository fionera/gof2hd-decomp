#include "class.h"

namespace AbyssEngine {
namespace AEMath {

Vector MatrixRotateVector(const Matrix &matrix, const Vector &vector)
{
    Vector result;
    result.y = vector.y * matrix.m[5];
    result.x = vector.y * matrix.m[1];
    result.z = vector.y * matrix.m[9];
    result.y += vector.x * matrix.m[4];
    result.x += vector.x * matrix.m[0];
    result.z += vector.x * matrix.m[8];
    result.y += vector.z * matrix.m[6];
    result.x += vector.z * matrix.m[2];
    result.z += vector.z * matrix.m[10];
    return result;
}

} // namespace AEMath
} // namespace AbyssEngine

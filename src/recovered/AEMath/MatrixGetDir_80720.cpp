#include "class.h"

namespace AbyssEngine {
namespace AEMath {

Vector MatrixGetDir(const Matrix &matrix)
{
    Vector result;
    result.x = matrix.m[2];
    result.y = matrix.m[6];
    result.z = matrix.m[10];
    return result;
}

} // namespace AEMath
} // namespace AbyssEngine

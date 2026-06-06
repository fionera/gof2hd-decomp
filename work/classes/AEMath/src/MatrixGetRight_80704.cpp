#include "class.h"

namespace AbyssEngine {
namespace AEMath {

Vector MatrixGetRight(const Matrix &matrix)
{
    Vector result;
    result.x = matrix.m[0];
    result.y = matrix.m[4];
    result.z = matrix.m[8];
    return result;
}

} // namespace AEMath
} // namespace AbyssEngine

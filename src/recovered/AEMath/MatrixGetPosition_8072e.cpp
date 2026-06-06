#include "class.h"

namespace AbyssEngine {
namespace AEMath {

Vector MatrixGetPosition(const Matrix &matrix)
{
    Vector result;
    result.x = matrix.m[3];
    result.y = matrix.m[7];
    result.z = matrix.m[11];
    return result;
}

} // namespace AEMath
} // namespace AbyssEngine

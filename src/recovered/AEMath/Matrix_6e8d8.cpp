#include "class.h"

namespace AbyssEngine {
namespace AEMath {

Matrix::Matrix()
{
    m[0] = 1.0f; m[1] = 0.0f; m[2] = 0.0f; m[3] = 0.0f; m[4] = 0.0f;
    m[5] = 1.0f; m[6] = 0.0f; m[7] = 0.0f; m[8] = 0.0f; m[9] = 0.0f;
    m[10] = 1.0f; m[11] = 0.0f; m[12] = 0.0f; m[13] = 0.0f; m[14] = 1.0f;
}

} // namespace AEMath
} // namespace AbyssEngine

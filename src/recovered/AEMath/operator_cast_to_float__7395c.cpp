#include "class.h"

namespace AbyssEngine {
namespace AEMath {

Vector::operator float *()
{
    return &x;
}

} // namespace AEMath
} // namespace AbyssEngine

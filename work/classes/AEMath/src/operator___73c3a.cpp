#include "class.h"

namespace AbyssEngine {
namespace AEMath {

bool operator==(const Vector &lhs, const Vector &rhs)
{
    bool result = false;
    if (lhs.x == rhs.x) {
        if (lhs.y == rhs.y) {
            result = lhs.z == rhs.z;
        }
    }
    return result;
}

} // namespace AEMath
} // namespace AbyssEngine

#include "class.h"

namespace AbyssEngine {
namespace AEMath {

bool operator!=(const Vector &lhs, const Vector &rhs) {
    if (lhs.x == rhs.x) {
        if (__builtin_expect(lhs.y != rhs.y, 1)) {
            return true;
        }
        return lhs.z != rhs.z;
    }
    return true;
}

} // namespace AEMath
} // namespace AbyssEngine

#include "quaternion.h"

namespace AbyssEngine {

Quaternion Quaternion::Normalized() {
    float length = Length();
    x /= length;
    y /= length;
    z /= length;
    w /= length;
    return *this;
}

} // namespace AbyssEngine

#include "quaternion.h"

namespace AbyssEngine {

Quaternion::operator const float *() const {
    return &x;
}

} // namespace AbyssEngine

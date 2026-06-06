#include "quaternion.h"

namespace AbyssEngine {

Quaternion::operator float *() {
    return &x;
}

} // namespace AbyssEngine

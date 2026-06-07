#include "class.h"

namespace AbyssEngine {

EaseInOutMatrix::~EaseInOutMatrix() {
    ((Quaternion *)((char *)this + 0x58))->~Quaternion();
    ((Quaternion *)((char *)this + 0x3c))->~Quaternion();
}

} // namespace AbyssEngine

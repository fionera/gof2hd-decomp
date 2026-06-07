#include "class.h"

namespace AbyssEngine {

void Transform::SetVisible(bool value) {
    *(bool *)((char *)this + 0xec) = value;
}

} // namespace AbyssEngine

#include "class.h"

namespace AbyssEngine {

void Transform::PauseAnimationWithTimeStamp(longlong time) {
    *(longlong *)((char *)this + 0x110) = time;
    InternUpdate(time, false);
    *(bool *)((char *)this + 0xed) = false;
}

} // namespace AbyssEngine

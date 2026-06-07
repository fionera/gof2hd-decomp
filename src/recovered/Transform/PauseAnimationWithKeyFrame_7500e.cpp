#include "class.h"

namespace AbyssEngine {

void Transform::PauseAnimationWithKeyFrame(int index) {
    if (index >= 0 && index < *(int *)((char *)this + 0x11c)) {
        char *keyFrame = *(char **)(*(int *)((char *)this + 0x120) + index * 4);
        longlong time = *(longlong *)(keyFrame + 0x50);
        *(longlong *)((char *)this + 0x110) = time;
        InternUpdate(time, false);
    }
    *(bool *)((char *)this + 0xed) = false;
}

} // namespace AbyssEngine

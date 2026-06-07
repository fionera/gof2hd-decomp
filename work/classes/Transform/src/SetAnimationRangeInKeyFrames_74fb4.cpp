#include "class.h"

namespace AbyssEngine {

void Transform::SetAnimationRangeInKeyFrames(int first, int last) {
    longlong start;
    if (first < 0 || *(int *)((char *)this + 0x11c) <= first) {
        start = 0;
    } else {
        char *keyFrame = *(char **)(*(int *)((char *)this + 0x120) + first * 4);
        start = *(longlong *)(keyFrame + 0x50);
    }

    longlong end;
    if (last >= 0 && last <= *(int *)((char *)this + 0x11c)) {
        char *keyFrame = *(char **)(*(int *)((char *)this + 0x120) + last * 4);
        end = *(longlong *)(keyFrame + 0x50);
    } else {
        end = 0;
    }
    SetAnimationRangeInTime(start, end);
}

} // namespace AbyssEngine

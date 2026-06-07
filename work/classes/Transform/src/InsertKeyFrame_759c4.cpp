#include "class.h"

namespace AbyssEngine {

void Transform::InsertKeyFrame(KeyFrame *keyFrame, int index) {
    ArrayAdd((KeyFrame *)0, *(Array<KeyFrame *> *)((char *)this + 0x11c));
    int count = *(int *)((char *)this + 0x11c);
    int from = count - 2;
    int to = count - 1;
    while (index < to) {
        int *items = *(int **)((char *)this + 0x120);
        items[to] = items[from];
        --from;
        --to;
    }
    KeyFrame **items = *(KeyFrame ***)((char *)this + 0x120);
    items[index] = keyFrame;
}

} // namespace AbyssEngine

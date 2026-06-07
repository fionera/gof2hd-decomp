#include "class.h"

namespace AbyssEngine {

void Transform::AddKeyFrame(const AEMath::Vector &a, const AEMath::Vector &b,
                            const AEMath::Vector &c, const AEMath::Vector &d,
                            const AEMath::Vector &e, const AEMath::Vector &f,
                            int time) {
    char *keyFrame = (char *)operator new(0x60);
    new (keyFrame) KeyFrame();
    *(AEMath::Vector *)(keyFrame + 0x18) = b;
    *(AEMath::Vector *)(keyFrame + 0x00) = c;
    *(AEMath::Vector *)(keyFrame + 0x0c) = a;
    *(AEMath::Vector *)(keyFrame + 0x3c) = e;
    *(AEMath::Vector *)(keyFrame + 0x24) = f;
    *(AEMath::Vector *)(keyFrame + 0x30) = d;
    longlong timestamp = time;
    *(longlong *)(keyFrame + 0x50) = timestamp;
    ArrayAdd((KeyFrame *)keyFrame, *(Array<KeyFrame *> *)((char *)this + 0x11c));
    if (*(longlong *)((char *)this + 0xf8) < timestamp) {
        *(longlong *)((char *)this + 0xf8) = timestamp;
    }
}

} // namespace AbyssEngine

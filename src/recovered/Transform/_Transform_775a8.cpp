#include "class.h"

namespace AbyssEngine {

Transform::~Transform() {
    char *self = (char *)this;
    Array<KeyFrame *> *keyFrames = (Array<KeyFrame *> *)(self + 0x11c);
    if (*(bool *)(self + 0x17d) == false) {
        ArrayReleaseClasses(keyFrames);
    }
    ((Quaternion *)(self + 0x150))->~Quaternion();
    ((Quaternion *)(self + 0x128))->~Quaternion();
    keyFrames->~Array();
    ((Array<Transform *> *)(self + 0x4c))->~Array();
    ((Array<Mesh *> *)(self + 0x3c))->~Array();
}

} // namespace AbyssEngine

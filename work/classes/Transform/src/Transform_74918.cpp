#include "class.h"

namespace AbyssEngine {

Transform::Transform(Transform *other) {
    char *self = (char *)this;

    new ((AEMath::Matrix *)(self + 0x00)) AEMath::Matrix();
    new ((Array<Mesh *> *)(self + 0x3c)) Array<Mesh *>();
    new ((Array<Transform *> *)(self + 0x4c)) Array<Transform *>();
    new ((AEMath::Matrix *)(self + 0x5c)) AEMath::Matrix();
    new ((AEMath::Matrix *)(self + 0x98)) AEMath::Matrix();

    *(float *)(self + 0xd4) = 0.0f;
    *(float *)(self + 0xd8) = 0.0f;
    *(float *)(self + 0xdc) = 0.0f;
    *(float *)(self + 0xe0) = 0.0f;
    *(float *)(self + 0xe4) = 1.0f;

    new ((Array<KeyFrame *> *)(self + 0x11c)) Array<KeyFrame *>();
    new ((Quaternion *)(self + 0x128)) Quaternion();
    *(longlong *)(self + 0x148) = 0;
    *(AEMath::Vector *)(self + 0x138) = AEMath::Vector{0.0f, 0.0f, 0.0f};
    new ((Quaternion *)(self + 0x150)) Quaternion();
    *(longlong *)(self + 0x170) = 0;
    *(AEMath::Vector *)(self + 0x160) = AEMath::Vector{0.0f, 0.0f, 0.0f};

    if (other == 0) {
        Transform temp;
        temp.~Transform();
    } else {
        char *src = (char *)other;
        *(bool *)(self + 0xed) = true;
        *(int *)(self + 0xe8) = *(int *)(src + 0xe8);
        *(float *)(self + 0xf0) = 1.0f;
        *(int *)(self + 0x48) = -1;
        *(longlong *)(self + 0xf8) = *(longlong *)(src + 0xf8);
        *(longlong *)(self + 0x100) = *(longlong *)(src + 0x100);
        *(longlong *)(self + 0x108) = *(longlong *)(src + 0x108);
        *(longlong *)(self + 0x110) = *(longlong *)(src + 0x110);
        *(int *)(self + 0x118) = *(int *)(src + 0x118);
        *(Quaternion *)(self + 0x128) = *(Quaternion *)(src + 0x128);
        *(AEMath::Vector *)(self + 0x138) = *(AEMath::Vector *)(src + 0x138);
        *(AEMath::Vector *)(self + 0x144) = *(AEMath::Vector *)(src + 0x144);
        *(AEMath::Matrix *)(self + 0x5c) = *(AEMath::Matrix *)(src + 0x5c);
        *(Quaternion *)(self + 0x150) = *(Quaternion *)(src + 0x150);
        *(AEMath::Vector *)(self + 0x160) = *(AEMath::Vector *)(src + 0x160);
        *(AEMath::Vector *)(self + 0x16c) = *(AEMath::Vector *)(src + 0x16c);
        *(AEMath::Matrix *)(self + 0x98) = *(AEMath::Matrix *)(src + 0x98);
        *(int *)(self + 0x58) = *(int *)(src + 0x58);

        for (uint i = 0; i < *(uint *)(src + 0x3c); ++i) {
            Mesh *mesh = (Mesh *)operator new(0x88);
            new (mesh) Mesh((*(Mesh ***)(src + 0x40))[i]);
            ArrayAdd(mesh, *(Array<Mesh *> *)(self + 0x3c));
        }
        ArraySet((Array<KeyFrame *> *)(src + 0x11c), (Array<KeyFrame *> *)(self + 0x11c));
        for (uint i = 0; i < *(uint *)(src + 0x4c); ++i) {
            Transform *child = (Transform *)operator new(0x180);
            new (child) Transform((*(Transform ***)(src + 0x50))[i]);
            ArrayAdd(child, *(Array<Transform *> *)(self + 0x4c));
        }
        *(AEMath::BSphere *)(self + 0xd4) = *(AEMath::BSphere *)(src + 0xd4);
        *(bool *)(self + 0xec) = true;
        *(uint16_t *)(self + 0x17c) = 0x101;
    }
}

} // namespace AbyssEngine

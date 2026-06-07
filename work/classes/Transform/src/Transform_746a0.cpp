#include "class.h"

namespace AbyssEngine {

static void make_identity(AEMath::Matrix &m) {
    for (int i = 0; i < 15; ++i) {
        m.m[i] = 0.0f;
    }
    m.m[0] = 1.0f;
    m.m[5] = 1.0f;
    m.m[10] = 1.0f;
    m.m[14] = 1.0f;
}

Transform::Transform() {
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
    *(float *)(self + 0x138) = 0.0f;
    *(float *)(self + 0x13c) = 0.0f;
    *(float *)(self + 0x140) = 0.0f;
    *(float *)(self + 0x144) = 0.0f;
    *(longlong *)(self + 0x148) = 0;
    new ((Quaternion *)(self + 0x150)) Quaternion();

    *(longlong *)(self + 0xf8) = 0;
    *(longlong *)(self + 0x170) = 0;
    *(int *)(self + 0x48) = -1;
    *(bool *)(self + 0xed) = true;
    *(float *)(self + 0xf0) = 1.0f;
    *(longlong *)(self + 0x110) = 0;
    *(int *)(self + 0x118) = 0;
    *(longlong *)(self + 0x100) = 0;
    *(longlong *)(self + 0x108) = 0;

    Quaternion q;
    *(Quaternion *)(self + 0x150) = q;
    *(Quaternion *)(self + 0x128) = q;
    q.~Quaternion();

    AEMath::Vector zero = {0.0f, 0.0f, 0.0f};
    *(AEMath::Vector *)(self + 0x160) = zero;
    *(AEMath::Vector *)(self + 0x138) = zero;
    AEMath::Vector one = {1.0f, 1.0f, 1.0f};
    *(AEMath::Vector *)(self + 0x16c) = one;
    *(AEMath::Vector *)(self + 0x144) = one;

    AEMath::Matrix identity;
    make_identity(identity);
    *(AEMath::Matrix *)(self + 0x98) = identity;
    *(AEMath::Matrix *)(self + 0x5c) = identity;

    *(int *)(self + 0x58) = 2;
    *(float *)(self + 0xac) = 1.0f;
    *(uint16_t *)(self + 0x17c) = 1;
    *(bool *)(self + 0xec) = true;
    *(int *)(self + 0xe8) = 0;
}

} // namespace AbyssEngine

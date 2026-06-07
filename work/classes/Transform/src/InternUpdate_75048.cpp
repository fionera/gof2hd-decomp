#include "class.h"

namespace AbyssEngine {

static void identity_matrix(AEMath::Matrix &m) {
    for (int i = 0; i < 15; ++i) m.m[i] = 0.0f;
    m.m[0] = 1.0f;
    m.m[5] = 1.0f;
    m.m[10] = 1.0f;
    m.m[14] = 1.0f;
}

static float clamp_positive_byte(float value) {
    int v = (int)(value * 255.0f);
    return (float)(v < 0 ? 0 : v);
}

void Transform::InternUpdate(longlong time, bool updateBounds) {
    char *self = (char *)this;

    if (*(int *)(self + 0x11c) != 0) {
        longlong current = *(longlong *)(self + 0x100);
        if (current < time) current = time;

        char **items = *(char ***)(self + 0x120);
        char *last = items[*(int *)(self + 0x11c) - 1];
        if (*(longlong *)(last + 0x50) < current) {
            current = *(longlong *)(last + 0x50);
        }

        int index = 0;
        while (*(longlong *)(items[index] + 0x50) < current) {
            ++index;
        }
        *(int *)(self + 0x118) = index;

        if (index == 0) {
            AEMath::Matrix identity;
            identity_matrix(identity);
            *(AEMath::Matrix *)(self + 0x98) = identity;
            if (!g_transform_matrix_flag) {
                *(float *)(self + 0xac) = 1.0f;
            }
        } else if (current == 0) {
            char *key = items[index];
            Quaternion q(*(float *)(key + 0x18), *(float *)(key + 0x1c), *(float *)(key + 0x20));
            *(Quaternion *)(self + 0x128) = q;
            q.~Quaternion();
            *(AEMath::Vector *)(self + 0x138) = *(AEMath::Vector *)(key + 0x00);
            *(AEMath::Vector *)(self + 0x144) = *(AEMath::Vector *)(key + 0x0c);
            *(float *)(self + 0x178) = *(float *)(key + 0x48);
            Quaternion q2(*(float *)(key + 0x3c), *(float *)(key + 0x40), *(float *)(key + 0x44));
            *(Quaternion *)(self + 0x150) = q2;
            q2.~Quaternion();
            *(AEMath::Vector *)(self + 0x160) = *(AEMath::Vector *)(key + 0x24);
            *(AEMath::Vector *)(self + 0x16c) = *(AEMath::Vector *)(key + 0x30);
        } else {
            char *prev = items[index - 1];
            char *next = items[index];
            float t = (float)(current - *(longlong *)(prev + 0x50)) /
                      (float)(*(longlong *)(next + 0x50) - *(longlong *)(prev + 0x50));

            Quaternion qa(*(float *)(prev + 0x18), *(float *)(prev + 0x1c), *(float *)(prev + 0x20));
            Quaternion qb(*(float *)(next + 0x18), *(float *)(next + 0x1c), *(float *)(next + 0x20));
            Quaternion q;
            Quaternion::Lerp(&q, &qa, &qb, t);
            *(Quaternion *)(self + 0x128) = q;
            q.~Quaternion();
            qb.~Quaternion();
            qa.~Quaternion();

            *(AEMath::Vector *)(self + 0x138) = AEMath::VectorLerp(
                *(AEMath::Vector *)(prev + 0x00), *(AEMath::Vector *)(next + 0x00), t);
            *(AEMath::Vector *)(self + 0x144) = AEMath::VectorLerp(
                *(AEMath::Vector *)(prev + 0x0c), *(AEMath::Vector *)(next + 0x0c), t);
            *(float *)(self + 0x178) = *(float *)(prev + 0x48) +
                                       t * (*(float *)(next + 0x48) - *(float *)(prev + 0x48));

            Quaternion qc(*(float *)(prev + 0x3c), *(float *)(prev + 0x40), *(float *)(prev + 0x44));
            Quaternion qd(*(float *)(next + 0x3c), *(float *)(next + 0x40), *(float *)(next + 0x44));
            Quaternion qout;
            Quaternion::Lerp(&qout, &qc, &qd, t);
            *(Quaternion *)(self + 0x150) = qout;
            qout.~Quaternion();
            qd.~Quaternion();
            qc.~Quaternion();

            *(AEMath::Vector *)(self + 0x160) = AEMath::VectorLerp(
                *(AEMath::Vector *)(prev + 0x24), *(AEMath::Vector *)(next + 0x24), t);
            *(AEMath::Vector *)(self + 0x16c) = AEMath::VectorLerp(
                *(AEMath::Vector *)(prev + 0x30), *(AEMath::Vector *)(next + 0x30), t);
        }

        uint alpha = (uint)clamp_positive_byte(*(float *)(self + 0x178));
        *(uint *)(self + 0x48) = alpha | (alpha << 8) | (alpha << 16) | (alpha << 24);

        AEMath::Matrix rotation;
        identity_matrix(rotation);
        ((Quaternion *)(self + 0x128))->Convert(rotation);
        *(AEMath::Matrix *)(self + 0x5c) = rotation;

        AEMath::Matrix scale;
        AEMath::MatrixSetScaling(scale, *(float *)(self + 0x144),
                                 *(float *)(self + 0x148), *(float *)(self + 0x14c));
        AEMath::MatrixSetTranslation(scale, *(float *)(self + 0x138),
                                     *(float *)(self + 0x13c), *(float *)(self + 0x140));

        AEMath::Matrix local;
        ((Quaternion *)(self + 0x150))->Convert(local);
        AEMath::Matrix composed = AEMath::operator*(local, scale);
        *(AEMath::Matrix *)(self + 0x98) = composed;
    }

    if (!updateBounds) {
        AEMath::BSphere sphere;
        sphere.center.x = 0.0f;
        sphere.center.y = 0.0f;
        sphere.center.z = 0.0f;
        sphere.radius = 0.0f;
        sphere.radius2 = 1.0f;
        *(AEMath::BSphere *)(self + 0xd4) = sphere;

        for (uint i = 0; i < *(uint *)(self + 0x3c); ++i) {
            char *mesh = (*(char ***)(self + 0x40))[i];
            AEMath::BSphere childSphere;
            if (mesh != 0 && *(Transform **)(mesh + 0x34) != 0) {
                (*(Transform **)(mesh + 0x34))->InternUpdate(time, updateBounds);
            }
            childSphere = *(AEMath::BSphere *)(mesh + 0x3c);
            ((AEMath::BSphere *)(self + 0xd4))->Merge(childSphere);
        }

        for (uint i = 0; i < *(uint *)(self + 0x4c); ++i) {
            Transform *child = (*(Transform ***)(self + 0x50))[i];
            child->InternUpdate(time, updateBounds);
            AEMath::BSphere childSphere = *(AEMath::BSphere *)((char *)child + 0xd4);
            ((AEMath::BSphere *)(self + 0xd4))->Merge(childSphere);
        }
    }
}

} // namespace AbyssEngine

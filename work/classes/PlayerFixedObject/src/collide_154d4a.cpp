#include "class.h"

// collide(float,float,float): iterate the active bounding-volume array, calling each
// volume's vtable slot +8 with (bv, x, y, z); return 1 on first hit, else 0.
// NEAR: clang rotates the first loop as break-test (cbnz) while the target uses
// continue-test (cmp;beq) there; the two loops differ only in rotation.
typedef int (*CollideFn)(void *bv, float, float, float);

extern "C" int PlayerFixedObject_collide(PlayerFixedObject *self, float x, float y, float z)
{
    Array<void *> *a = F<Array<void *> *>(self, 0x12c);
    if ((a != 0 || F<int>(self, 0x88) != 4) && F<uint8_t>(self, 0x8c) != 0) {
        if (a != 0 && F<int>(self, 0x88) == 4) {
            for (uint32_t i = 0; i < a->length; i++) {
                void *bv = a->data[i];
                CollideFn fn = *(CollideFn *)(*(char **)bv + 0x8);
                if (fn(bv, x, y, z) != 0) return 1;
                a = F<Array<void *> *>(self, 0x12c);
            }
        } else {
            Array<void *> *b = F<Array<void *> *>(self, 0x128);
            if (b != 0) {
                for (uint32_t i = 0; i < b->length; i++) {
                    void *bv = b->data[i];
                    CollideFn fn = *(CollideFn *)(*(char **)bv + 0x8);
                    if (fn(bv, x, y, z) != 0) return 1;
                    b = F<Array<void *> *>(self, 0x128);
                }
            }
        }
    }
    return 0;
}

#include "class.h"

struct BV;
typedef int (*CollFn)(BV *, float, float, float);

extern "C" int PlayerFighter_collide(PlayerFighter *self, float x, float y, float z)
{
    if ((unsigned)(F<int32_t>(self, 0x88) - 3) > 1) {
        Array<BV *> *a = F<Array<BV *> *>(self, 0x150);
        if (a != 0) {
            for (unsigned i = 0; i < a->length; i++) {
                BV *e = a->data[i];
                CollFn fn = *(CollFn *)(*(char **)e + 8);
                if (fn(e, x, y, z) != 0) {
                    return 1;
                }
                a = F<Array<BV *> *>(self, 0x150);
            }
        }
    }
    return 0;
}

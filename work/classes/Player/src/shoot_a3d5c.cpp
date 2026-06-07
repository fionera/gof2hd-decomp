#include "class.h"

extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);

extern "C" void Gun_ignite(Gun *self);
extern "C" int Gun_shoot(Gun *self,
                         int a8, int a9, int a10, int a11, int a12, int a13, int a14,
                         int a15, int a16, int a17, int a18, int a19, int a20, int a21, int a22,
                         int flag, int a7);
extern "C" void MatrixGetPosition(void *out, float *matrix);
extern "C" void Player_playShootSound(Player *self, int type, void *channel, float volume);

extern "C" unsigned int g_shoot_mask;   // DAT_b3f24
extern "C" const float k_shoot_inc;      // DAT_b3f28

extern "C" int Player_shoot2(Player *self, unsigned int slot, int gunId, int a4_00, int flag,
                             int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13,
                             int a14, int a15, int a16, int a17, int a18, int a19, int a20,
                             int a21, int a22)
{
    void * volatile cookie = __stack_chk_guard;
    unsigned int mask = g_shoot_mask;
    int retval = 1;

    Array<Array<Gun *> *> *guns = self->guns;
    if (guns != 0 && self->shootingEnabled != 0 && (int)slot >= 0 &&
        slot < guns->length) {
        Array<Gun *> *arr = guns->data[slot];
        if (arr != 0) {
            for (unsigned int i = 0; i < arr->length; i++) {
                Gun *g = self->guns->data[slot]->data[i];
                unsigned int sortIdx = *(int *)((char *)g + 0x5c) - 6;
                if (sortIdx < 0x1d && ((1u << (sortIdx & 0xff)) & mask) != 0 &&
                    **(int **)((char *)g + 0x3c) >= 0) {
                    Gun_ignite(g);
                } else if (*(int *)((char *)g + 0x58) == gunId &&
                           *(int *)((char *)g + 0x48) < *(int *)((char *)g + 0x6c)) {
                    if (sortIdx < 0x1d && ((1u << (sortIdx & 0xff)) & mask) != 0) {
                        *(char *)(*(int *)((char *)g + 0x38) + 0x69) = 1;
                    }
                    int r = Gun_shoot(g, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19,
                                      a20, a21, a22, flag, a7);
                    if (r != 0) {
                        *(float *)((char *)self + 0x60) = *(float *)((char *)self + 0x60) + k_shoot_inc;
                        if (self->playShootSound != 0) {
                            float tmp[3];
                            MatrixGetPosition(tmp, self->transform);
                            Gun *g2 = self->guns->data[slot]->data[i];
                            Player_playShootSound(self, *(int *)((char *)g2 + 0x58),
                                                  *(void **)((char *)g2 + 0x5c),
                                                  *(float *)((char *)g2 + 0xb0));
                        }
                        *(int *)((char *)g + 0x6c) = 0;
                        retval = 1;
                        break;
                    }
                }
            }
        }
    }

    uint32_t guardDelta = (uint32_t)__stack_chk_guard - (uint32_t)cookie;
    if (guardDelta != 0) {
        __stack_chk_fail(guardDelta);
    }
    return retval;
}

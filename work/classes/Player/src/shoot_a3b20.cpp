#include "class.h"

extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);

extern "C" int Gun_shootAt(Gun *self,
                           int m0, int m1, int m2, int m3, int m4, int m5, int m6, int m7,
                           int m8, int m9, int m10, int m11, int m12, int m13, int m14,
                           int a, void *self2, int b);
extern "C" void MatrixGetPosition(void *out, float *matrix);
extern "C" void Player_playShootSound(Player *self, int type, void *channel, float volume);

extern "C" const float k_shootAt_inc;   // DAT_b3cb4

extern "C" void Player_shoot1(Player *self, unsigned int slot, int idLo, int idHi, int flag,
                              int m0, int m1, int m2, int m3, int m4, int m5, int m6, int m7,
                              int m8, int m9, int m10, int m11, int m12, int m13, int m14)
{
    void * volatile cookie = __stack_chk_guard;
    (void)idLo; (void)idHi;

    Array<Array<Gun *> *> *guns = self->guns;
    if (guns != 0 && self->shootingEnabled != 0 && (int)slot >= 0 &&
        slot < guns->length) {
        Array<Gun *> *arr = guns->data[slot];
        if (arr != 0) {
            for (unsigned int i = 0; i < arr->length; i++) {
                Gun *g = self->guns->data[slot]->data[i];
                if (*(int *)((char *)g + 0x48) < *(int *)((char *)g + 0x6c)) {
                    int r = Gun_shootAt(g, m0, m1, m2, m3, m4, m5, m6, m7,
                                        m8, m9, m10, m11, m12, m13, m14,
                                        flag, self, idLo);
                    if (r != 0) {
                        *(float *)((char *)self + 0x60) = *(float *)((char *)self + 0x60) + k_shootAt_inc;
                        Gun *g2 = self->guns->data[slot]->data[i];
                        *(int *)((char *)g2 + 0x6c) = 0;
                        if (self->playShootSound != 0 && *(char *)((char *)g2 + 0x89) != 0) {
                            float tmp[3];
                            MatrixGetPosition(tmp, self->transform);
                            Gun *g3 = self->guns->data[slot]->data[i];
                            Player_playShootSound(self, *(int *)((char *)g3 + 0x58),
                                                  *(void **)((char *)g3 + 0x5c),
                                                  *(float *)((char *)g3 + 0xb0));
                        }
                    }
                }
            }
        }
    }

    uint32_t guardDelta = (uint32_t)__stack_chk_guard - (uint32_t)cookie;
    if (guardDelta != 0) {
        __stack_chk_fail(guardDelta);
    }
}

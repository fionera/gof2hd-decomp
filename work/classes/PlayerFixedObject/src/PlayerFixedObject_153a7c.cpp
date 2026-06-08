#include "class.h"
typedef unsigned long long uint64_t;

// PlayerFixedObject::PlayerFixedObject(int kind, int param2, Player*, AEGeometry*, float, float, float, ...)
// (two extra stack floats in_stack_00000004/8/c make up the spawn position passed by value).

extern "C" void KIPlayer_ctor(PlayerFixedObject *self, int kind, int hp, void *player,
                              void *geom, float a, float b, float c, bool d);
extern "C" char PlayerFixedObject_vtable;
extern "C" void *Vector_assign(Vector *dst, Vector *src);
extern "C" void String_ctor_empty(void *s);
extern "C" void String_ctor_cstr(void *s, const char *cstr, bool b);
extern "C" void *String_op_assign(void *dst, void *src);
extern "C" void String_dtor(void *p);

extern "C" void *Status_getMission();
extern "C" int Mission_isCampaignMission(void *m);
extern "C" int Status_getCurrentCampaignMission();
extern "C" void *Status_getStation();
extern "C" int Station_getIndex(void *st);

extern "C" void *operator_new(uint32_t);
extern "C" void operator_delete(void *p);
extern "C" void Generator_ctor(void *g);
extern "C" void *Generator_dtor(void *g);
extern "C" void *Generator_getLootList(void *g, int a, int b);
extern "C" void *Array_int_dtor(void *p);
extern "C" int AERandom_nextInt(int n);

// PC-relative literal table holding the station-index match list (4 ints) and the
// loot-list parameter table (4 x 8-byte pairs).
__attribute__((visibility("hidden"))) extern const int g_pfo_stationIdx[4];
__attribute__((visibility("hidden"))) extern const int g_pfo_lootParams[8]; // pairs at +0 used; [idx*2+1]
// AERandom singleton holder (pc-rel -> holder; *holder is the AERandom object).
__attribute__((visibility("hidden"))) extern void **g_pfo_random;

extern "C" void PlayerFixedObject_ctor(PlayerFixedObject *self, int kind, int param2,
                                       void *player, void *geom,
                                       float p5, float p6, float p7,
                                       float sx, float sy, float sz)
{
    KIPlayer_ctor(self, kind, -1, player, geom, p5, p6, p7, (bool)(int)sx);

    // Three zeroed Vector4 (16-byte) blocks.
    *(uint64_t *)((char *)self + 0x158) = 0; *(uint64_t *)((char *)self + 0x160) = 0;
    *(uint64_t *)((char *)self + 0x148) = 0; *(uint64_t *)((char *)self + 0x150) = 0;
    *(uint64_t *)((char *)self + 0x138) = 0; *(uint64_t *)((char *)self + 0x140) = 0;

    *(void **)self = &PlayerFixedObject_vtable + 8;

    String_ctor_empty((char *)self + 0x1ac);
    F<int32_t>(self, 0x18c) = 0;
    F<uint16_t>(self, 0x184) = 0;
    F<int32_t>(self, 0x28) = param2;
    F<int32_t>(self, 0x124) = 0;
    F<int32_t>(self, 0x128) = 0;
    F<int32_t>(self, 0x12c) = 0;
    F<int32_t>(self, 0x130) = 0;
    F<int32_t>(self, 0x168) = 0;
    F<int32_t>(self, 0x16c) = 0;
    F<uint8_t>(self, 0x170) = 0;
    F<uint8_t>(self, 0x1b8) = 0;
    F<int32_t>(self, 0x190) = 0;
    F<int32_t>(self, 0x174) = 0;
    F<int32_t>(self, 0x178) = 0; F<int32_t>(self, 0x17c) = 0; F<int32_t>(self, 0x180) = 0;

    // Vector::operator=(this+0x2c, {sx,sy,sz})
    {
        char buf[12];
        *(float *)(buf + 0) = sx; *(float *)(buf + 4) = sy; *(float *)(buf + 8) = sz;
        Vector_assign((Vector *)((char *)self + 0x2c), (Vector *)buf);
    }

    F<uint8_t>(self, 0x134) = 0;
    F<int32_t>(self, 0x194) = -1;
    F<int32_t>(self, 0x1a0) = -1;
    F<int32_t>(self, 0x1a4) = 0;
    F<int32_t>(self, 0x178) = (int32_t)sx;
    F<int32_t>(self, 0x17c) = (int32_t)sy;
    F<int32_t>(self, 0x180) = (int32_t)sz;

    // Name string from a fixed literal.
    {
        char tmp[12];
        String_ctor_cstr(tmp, "", false);
        String_op_assign((char *)self + 0x1ac, tmp);
        String_dtor(tmp);
    }

    void *mission = Status_getMission();
    int campaign = Mission_isCampaignMission(mission);
    bool special = false;
    if (campaign != 0) {
        int cm = Status_getCurrentCampaignMission();
        if (cm == 0x28) special = true;
        else {
            cm = Status_getCurrentCampaignMission();
            if (cm == 0x29) special = true;
        }
    }

    if (special) {
        if (F<void *>(self, 0x50) != 0) {
            operator_delete(Array_int_dtor(F<void *>(self, 0x50)));
        }
        F<int32_t>(self, 0x50) = 0;
    } else {
        void *gen = operator_new(1);
        Generator_ctor(gen);
        if (kind == 0x37a3) {
            F<uint8_t>(self, 0x41) = 1;
            void *station = Status_getStation();
            int sidx = Station_getIndex(station);
            for (uint32_t i = 0; i < 4; i++) {
                if (g_pfo_stationIdx[i] == sidx) {
                    void *loot = Generator_getLootList(gen, g_pfo_lootParams[i * 2 + 1], 0);
                    F<void *>(self, 0x50) = loot;
                }
            }
        } else {
            uint32_t *loot = (uint32_t *)Generator_getLootList(gen, -1, -1);
            F<void *>(self, 0x50) = loot;
            if (loot != 0) {
                int second = (kind != 0x498e) ? 0x4a88 : 0x498e;
                if (kind != 0x498e && kind != second) {
                    void *rng = *g_pfo_random;
                    for (int idx = 1; (uint32_t)(idx - 1) < loot[0]; idx += 2) {
                        if (kind == 0xe) {
                            int r = AERandom_nextInt((int)(long)rng);
                            loot = (uint32_t *)F<void *>(self, 0x50);
                            int *cell = (int *)(loot[1] + idx * 4);
                            *cell = *cell * (r + 5);
                        } else {
                            int r = AERandom_nextInt((int)(long)rng);
                            int *base = (int *)(*(int *)((char *)F<void *>(self, 0x50) + 4) + idx * 4);
                            *base = *base * (r + 2);
                            int r2 = AERandom_nextInt((int)(long)rng);
                            loot = (uint32_t *)F<void *>(self, 0x50);
                            int *cell = (int *)(loot[1] + idx * 4);
                            int v = *cell;
                            if (v < r2 + 8) v = r2 + 8;
                            *cell = v;
                        }
                    }
                }
            }
        }
        operator_delete(Generator_dtor(gen));
    }

    *(uint8_t *)(*(char **)((char *)self + 0x4) + 0x45) = 1;
    if (kind != 0x37a3) {
        F<int32_t>(self, 0xf8) = 0x2f;
        if (kind == 0xe) {
            F<int32_t>(self, 0xf8) = -1;
            F<uint8_t>(self, 0x134) = 0;
        }
    }
}

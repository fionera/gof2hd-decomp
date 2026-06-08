#include "class.h"

extern "C" void *__stack_chk_guard;
extern "C" void __stack_chk_fail(unsigned diff);

extern "C" void KIPlayer_ctor(PlayerFighter *self, int p1, int wingmanCmd, void *player,
                              void *geom, float a, float b, float c, int flag);
extern "C" void *__aeabi_memclr4(void *dst, unsigned n);
extern "C" void *__aeabi_memcpy(void *dst, const void *src, unsigned n);
extern "C" void AEMath_Matrix_ctor(void *m);
extern "C" void AEMath_VectorAssign(void *dst, void *src);
extern "C" int AERandom_nextInt(int rng);
extern "C" float VectorSignedToFloat(int v, int mode);
extern "C" void *operator_new(unsigned int sz);
extern "C" void operator_delete(void *p);
extern "C" int *RH_op_new_arr(unsigned int n);
extern "C" void RH_op_delete_arr(void *p);
extern "C" void Route_ctor(void *self, int *points, unsigned n);
extern "C" void Route_setLoop(void *route, int loop);
extern "C" int Route_clone(void *route);
extern "C" int Status_getCurrentCampaignMission();
extern "C" int Status_inAlienOrbit();
extern "C" int Player_getHitpoints(int player);
extern "C" void Generator_ctor(void *g);
extern "C" int Generator_getLootList(void *g, int a);
extern "C" void *Generator_dtor(void *g);
extern "C" void Explosion_ctor(void *e, int flag);
extern "C" void Explosion_addFireStreaks(void *e);

extern void *const gPFC_guard __attribute__((visibility("hidden")));     // DAT_000ec6a4
extern const int gPFC_vtable __attribute__((visibility("hidden")));      // DAT_000ec6a8 (vptr base)
extern void *const gPFC_rng __attribute__((visibility("hidden")));        // DAT_000ec6ac
extern int *const gPFC_sharedRoute __attribute__((visibility("hidden"))); // DAT_000ec818
extern const int gPFC_defaultRoute __attribute__((visibility("hidden"))); // DAT_000ec81c (0x30 bytes)

// PlayerFighter::PlayerFighter(int p1, int wingmanCmd, Player*, AEGeometry*, float, float, float, bool)
extern "C" void PlayerFighter_ctor(PlayerFighter *self, int p1, int wingmanCmd, void *player,
                                   void *geom, float a, float b, float c, int flag)
{
    int *guardP = *(int **)gPFC_guard;
    volatile int saved = *guardP;

    KIPlayer_ctor(self, p1, -1, player, geom, a, b, c, flag);
    *(int *)self = gPFC_vtable + 8;
    F<int32_t>(self, 0x200) = 0;
    F<int32_t>(self, 0x204) = 0;
    F<int32_t>(self, 0x208) = 0;
    __aeabi_memclr4((char *)self + 0x158, 0x48);
    AEMath_Matrix_ctor((char *)self + 0x218);
    AEMath_Matrix_ctor((char *)self + 0x258);

    int rng = *(int *)gPFC_rng;
    // 9 candidate waypoints (3 floats each) drawn from the RNG.
    float wp[27];
    int r;
    r = AERandom_nextInt(rng); wp[0] = VectorSignedToFloat(r - 30000, 0);
    int r1 = AERandom_nextInt(rng);
    int r2 = AERandom_nextInt(rng);
    wp[2] = VectorSignedToFloat(r2 + 20000, 0);
    wp[1] = VectorSignedToFloat(r1 - 10000, 0);
    r = AERandom_nextInt(rng); wp[3] = VectorSignedToFloat(r + 5000, 0);
    r1 = AERandom_nextInt(rng);
    r2 = AERandom_nextInt(rng);
    wp[5] = VectorSignedToFloat(r2 + 20000, 0);
    wp[4] = VectorSignedToFloat(r1 - 10000, 0);
    r = AERandom_nextInt(rng); wp[6] = VectorSignedToFloat(r + 5000, 0);
    r1 = AERandom_nextInt(rng);
    r2 = AERandom_nextInt(rng);
    wp[8] = VectorSignedToFloat(r2 + 55000, 0);
    wp[7] = VectorSignedToFloat(r1 - 10000, 0);
    r = AERandom_nextInt(rng); wp[9] = VectorSignedToFloat(r - 30000, 0);
    r1 = AERandom_nextInt(rng);
    r2 = AERandom_nextInt(rng);
    wp[10] = VectorSignedToFloat(r1 - 10000, 0);
    wp[11] = VectorSignedToFloat(r2 + 55000, 0);

    int count = AERandom_nextInt(rng) * 3 + 6;
    char used[16];
    for (int i = 0; i < 13; i++) used[i] = 0;
    unsigned long long bytes = (unsigned long long)(unsigned)count * 4;
    unsigned sz = (int)(bytes >> 32) != 0 ? 0xffffffff : (unsigned)bytes;
    int *pts = RH_op_new_arr(sz);
    for (int i = 0; i < count; i += 3) {
        int idx;
        do {
            idx = AERandom_nextInt(rng);
        } while (used[idx] != 0);
        used[idx] = 1;
        pts[i] = (int)wp[idx * 3];
        pts[i + 1] = (int)wp[idx * 3 + 1];
        pts[i + 2] = (int)wp[idx * 3 + 2];
    }
    void *route = operator_new(0x18);
    Route_ctor(route, pts, (unsigned)count);
    F<void *>(self, 0x144) = route;
    RH_op_delete_arr(pts);

    int *shared = gPFC_sharedRoute;
    if (*shared == 0) {
        int defPts[12];
        __aeabi_memcpy(defPts, &gPFC_defaultRoute, 0x30);
        void *sr = operator_new(0x18);
        Route_ctor(sr, defPts, 0xc);
        *shared = (int)sr;
    }

    F<int32_t>(self, 0x130) = -1;
    F<int32_t>(self, 0x134) = -1;
    F<int32_t>(self, 0x138) = -1;
    F<int32_t>(self, 0x128) = 50000;
    *(float *)((char *)self + 0x1a4) = 2.0f;
    F<int32_t>(self, 0x1a8) = 0;
    *(float *)((char *)self + 0x1ac) = 2.0f;
    F<int32_t>(self, 0x1b0) = 0x5dc;
    F<int32_t>(self, 0x1b4) = 5;
    F<int32_t>(self, 0x38) = 0;
    F<int32_t>(self, 0x140) = 0;
    F<short>(self, 0x12d) = 0;
    F<short>(self, 0x13d) = 0;
    F<int32_t>(self, 0x1f8) = 0;
    F<int32_t>(self, 0xd8) = 0;
    F<signed char>(self, 0x12f) = 0;
    F<signed char>(self, 0x1fc) = 0;
    F<int32_t>(self, 0x1c8) = 0;
    F<signed char>(self, 0x1e0) = 0;
    F<short>(self, 0x1f4) = 0;
    F<int32_t>(self, 0x28) = wingmanCmd;

    int zero3[4] = {0, 0, 0, 0};
    F<int32_t>(self, 0x1b8) = 0;
    AEMath_VectorAssign((char *)self + 0x1bc, zero3);
    F<int32_t>(self, 0x148) = 0;
    AEMath_VectorAssign((char *)self + 0x14c, zero3);
    F<int32_t>(self, 0x1d0) = 0;
    AEMath_VectorAssign((char *)self + 0x1d4, zero3);
    F<int32_t>(self, 0x1e4) = 0;
    AEMath_VectorAssign((char *)self + 0x1e8, zero3);

    int posVec[4]; posVec[0] = flag; posVec[1] = 0; posVec[2] = 0;
    AEMath_VectorAssign((char *)self + 0x158, posVec);
    F<signed char>(self, 0x13d) = 1;
    F<signed char>(self, 0x4c) = 1;
    F<int32_t>(self, 0x1e8) = F<int32_t>(self, 0x1ac);
    F<int32_t>(self, 0x1f0) = F<int32_t>(self, 0x1a4);
    Route_setLoop(F<void *>(self, 0x144), 0);
    Route_setLoop((void *)(long)*shared, 0);
    F<int32_t>(self, 0x6c) = 0;

    if (Status_getCurrentCampaignMission() != 0x29) {
        int cloned;
        if (wingmanCmd == 9) {
            cloned = Route_clone((void *)(long)*shared);
        } else {
            cloned = Route_clone(F<void *>(self, 0x144));
        }
        F<int32_t>(self, 0x6c) = cloned;
    }
    F<signed char>(self, 0x12d) = 0;

    if (wingmanCmd == 9) {
        F<int32_t>(self, 0x50) = 0;
    } else {
        void *g = operator_new(1);
        Generator_ctor(g);
        F<int32_t>(self, 0x50) = Generator_getLootList(g, -1);
        operator_delete(Generator_dtor(g));
    }

    F<int32_t>(self, 0x128) = (Status_inAlienOrbit() != 0) ? 100000 : 50000;

    void *exp = operator_new(0x68);
    Explosion_ctor(exp, 0);
    F<void *>(self, 0x124) = exp;
    Explosion_addFireStreaks(exp);
    F<signed char>(self, 0x13e) = 1;
    F<int32_t>(self, 0x1d8) = Player_getHitpoints(F<int32_t>(self, 4));
    F<int32_t>(self, 0x1dc) = 0;
    F<signed char>(self, 0x1e0) = 0;
    F<int32_t>(self, 0xe4) = 1;
    F<signed char>(self, 0x25) = 1;
    F<int32_t>(self, 0x1a0) = -1;

    int fov;
    if (Status_getCurrentCampaignMission() == 1) {
        fov = -1;
    } else {
        fov = (F<signed char>(self, 0xdc) == 0) ? 0x2e : 0x30;
    }
    F<int32_t>(self, 0xf8) = fov;

    F<int32_t>(self, 0x20c) = 0;
    F<int32_t>(self, 0x210) = 0;
    F<int32_t>(self, 0x214) = 0;
    F<int32_t>(self, 0x294) = 0;
    F<int32_t>(self, 0x298) = 0;
    for (int off = 0x29c; off != 0x2b0; off += 4) {
        *(int *)((char *)self + off) = 0;
    }
    F<signed char>(self, 0x2b4) = 0;
    F<int32_t>(self, 0x2b0) = 0;
    F<signed char>(self, 0x100) = 0;
    F<short>(self, 0x254) = 0;
    F<int32_t>(self, 0x2b8) = 0;
    F<signed char>(self, 0x13c) = 0;
    F<int32_t>(self, 0x2c4) = 0;
    F<int32_t>(self, 0x2c8) = 0;
    F<int32_t>(self, 0x2d4) = 0;
    F<int32_t>(self, 0x2cd) = 0;
    F<int32_t>(self, 0x2c9) = 0;
    F<signed char>(self, 0x2d8) = 1;
    F<int32_t>(self, 0x2dc) = -1;
    F<signed char>(self, 0x2e4) = 0;
    F<int32_t>(self, 0x2e8) = 0;

    if (*guardP == saved) {
        return;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
}

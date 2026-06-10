#include "PlayerFighter.h"


extern "C" void PlayerFighter_setShipGroup_base(AEGeometry *self, int a, bool b);
extern "C" void Player_setActive(int player, int on);
extern "C" void PlayerFighter_setExhaustVisible(PlayerFighter *self, bool vis);
extern "C" void PlayerFighter_awake_tail(int geom, int on);
extern "C" void PlayerFighter_cloak_off_helper();
extern "C" void *Trail_dtor(Trail *p);
extern "C" void operator_delete(void *p);
extern "C" void *Route_dtor(void *p);
extern "C" void ArrayReleaseClasses_BV(void *arr);
extern "C" void *ArrayBV_dtor(void *p);
extern "C" void *Trail_dtor(void *p);
extern "C" void *Explosion_dtor(void *p);
extern "C" void *EaseInOutMatrix_dtor(void *p);
extern "C" void *PlayerFighter_base_dtor(PlayerFighter *self);
extern "C" void *_ZN13PlayerFighterD1Ev(PlayerFighter *self);
extern "C" void KIPlayer_setLevel(PlayerFighter *self, Level *lvl);
extern "C" void *__stack_chk_guard;
extern "C" void __stack_chk_fail(unsigned diff);
extern "C" void *__aeabi_memclr4(void *dst, unsigned n);
extern "C" void *__aeabi_memcpy(void *dst, const void *src, unsigned n);
extern "C" void AEMath_Matrix_ctor(void *m);
extern "C" void AEMath_VectorAssign(void *dst, void *src);
extern "C" int AERandom_nextInt(int rng);
extern "C" float VectorSignedToFloat(int v, int mode);
extern "C" void *operator_new(unsigned int sz);
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
extern "C" int Explosion_isPlaying(int e);
extern "C" void AEGeometry_getPosition(void *out, int geom);
extern "C" int KIPlayer_isWingMan(PlayerFighter *self);
extern "C" int Status_getStanding();
extern "C" int Standing_isEnemy(int standing);
extern "C" void PF_update_dead(PlayerFighter *self);
extern "C" void PF_update_body(PlayerFighter *self, int dt);
extern "C" void AEGeometry_setPosition3(int geom, float x, float y, float z);
extern "C" void Trail_reset(int trail, int a, int b, int c);
extern "C" int AEGeometry_getMatrix2(int geom);
extern "C" void AEMath_MatrixAssign(void *dst, void *src);
extern "C" int AEGeometry_getMatrix(int geom);
extern "C" void AEMath_MatrixIdentity(void *out, void *m);
extern "C" void AEMath_MatrixSetRotation(void *m, float rx, float ry, float rz);
extern "C" void ArrayInt_ctor(Array<int> *a);
extern "C" int Status_getMission(unsigned status);
extern "C" int Mission_getType(int mission);
extern "C" void ArrayInt_add(int val, Array<int> *a);
extern "C" void PlayerFighter_setMissionCrate_tail(int one, Array<int> *a);
extern "C" void ArrayBV_ctor(Array<BoundingVolume *> *a);
extern "C" void PlayerFighter_setBV_add(BoundingVolume *bv, Array<BoundingVolume *> *a);
extern "C" void KIPlayer_setWingmanCommand(PlayerFighter *self, int cmd, KIPlayer *target);
extern "C" int Level_getPlayerRoute(int level);
extern "C" int Route_getExactClone(int route);
extern "C" int Route_getCurrent(int route);
extern "C" float AEMath_VectorLength(void *v);
extern "C" void AEMath_VectorNormalize(void *out, void *v);
extern "C" int AERandom_nextIntB(int rng, int bound);
extern "C" unsigned PaintCanvas_TransformGetTransform(unsigned t);
extern "C" void PlayerFighter_setExhaustVisible_apply(unsigned transform, bool vis);
extern "C" void AEGeometry_render(int geom);
extern "C" int Player_isActive(int player);
extern "C" void Explosion_render(int e);
extern "C" int PaintCanvas_TransformGetLocal(unsigned t);
extern "C" void PaintCanvas_TransformSetLocal(unsigned t, void *m);
extern "C" void Trail_render(int t);
extern "C" void PlayerFighter_render_tail(int geom);
extern "C" void AEGeometry_setMatrix(void *geom);
extern "C" void AEGeometry_translate(void *geom);
extern "C" void PF_vscale(void *out, void *vec, float scalar);
extern "C" void KIPlayer_reset(PlayerFighter *self);
extern "C" int KIPlayer_isDead(PlayerFighter *self);
extern "C" void PaintCanvas_MeshCloneMaterial(void *canvas, unsigned mesh, unsigned *out);
extern "C" int PaintCanvas_MeshGetPointer(void *canvas, unsigned mesh);
extern "C" int PaintCanvas_MaterialGetMaterial(void *canvas, unsigned mat);
extern "C" void PaintCanvas_MeshChangeMaterial(void *canvas, unsigned mesh, unsigned short mat);
extern "C" int Player_turnedEnemy(int player);
extern "C" void Player_reset(int player);
extern "C" void Player_turnEnemy(int player);
extern "C" void AEString_ctor_default(void *s);
extern "C" void AEString_assign(void *dst, void *src);
extern "C" void AEString_dtor(void *s);
extern "C" void Route_reset(int route);
extern "C" void KIPlayer_setActive(PlayerFighter *self, int on);
extern "C" void Explosion_reset(int e);
extern "C" void AEGeometry_setVisible(int geom, int vis);
extern "C" void *ArrayInt_dtor(void *p);
extern "C" int Generator_getLootList(void *g, int a, int b);

// ---- hasMissionCrateLost_dcb7e.cpp ----
extern "C" uint8_t PlayerFighter_hasMissionCrateLost(PlayerFighter *self)
{
    return F<uint8_t>(self, 0x68);
}

// ---- hasMissionCrateCaptured_dcb78.cpp ----
extern "C" uint8_t PlayerFighter_hasMissionCrateCaptured(PlayerFighter *self)
{
    return F<uint8_t>(self, 0x69);
}

// ---- setShootError_dccf4.cpp ----
extern "C" void PlayerFighter_setShootError(PlayerFighter *self, int v)
{
    F<float>(self, 0x1a8) = (float)v;
}

// ---- setAIDisabled_dcfcc.cpp ----
extern "C" void PlayerFighter_setAIDisabled(PlayerFighter *self, bool v)
{
    F<uint8_t>(self, 0x2e4) = v;
}

// ---- setShipGroup_dcb84.cpp ----
struct AEGeometry;

// PlayerFighter::setShipGroup is a thunk that tail-jumps to the base implementation
// (target is a single b.w to a relocated veneer). Model as a tail call with the same args.

extern "C" void PlayerFighter_setShipGroup(AEGeometry *self, int a, bool b)
{
    return PlayerFighter_setShipGroup_base(self, a, b);
}

// ---- awake_dff2c.cpp ----
extern "C" void PlayerFighter_awake(PlayerFighter *self)
{
    F<int32_t>(self, 0x88) = 1;
    Player_setActive(F<int32_t>(self, 0x4), 1);
    PlayerFighter_setExhaustVisible(self, true);
    int geom = F<int32_t>(self, 0xc);
    F<uint8_t>(self, 0xf5) = 1;
    if (geom == 0) {
        geom = F<int32_t>(self, 0x8);
    }
    return PlayerFighter_awake_tail(geom, 1);
}

// ---- setBV_dfb84.cpp ----
struct BoundingVolume;

extern "C" void PlayerFighter_setBV_a(PlayerFighter *self, Array<BoundingVolume *> *v)
{
    F<Array<BoundingVolume *> *>(self, 0x150) = v;
}

// ---- setBoostProb_dcd02.cpp ----
extern "C" void PlayerFighter_setBoostProb(PlayerFighter *self, int v)
{
    F<int32_t>(self, 0x1b4) = v;
}

// ---- setCloakingPossible_dcd9c.cpp ----
extern "C" void PlayerFighter_setCloakingPossible(PlayerFighter *self, bool v)
{
    F<uint8_t>(self, 0x2d8) = v;
    if (!v && F<uint8_t>(self, 0x13c) != 0) {
        F<int32_t>(self, 0x2c8) = F<int32_t>(self, 0x2cc) + 1;
        return PlayerFighter_cloak_off_helper();
    }
}

// ---- removeTrail_dcd08.cpp ----
struct Trail;

extern "C" void PlayerFighter_removeTrail(PlayerFighter *self)
{
    Trail *t = F<Trail *>(self, 0x154);
    if (t != 0) {
        operator_delete(Trail_dtor(t));
    }
    F<Trail *>(self, 0x154) = 0;
}

// ---- _PlayerFighter_dc824.cpp ----
// PlayerFighter complete-object destructor (D1). Returns this. Sets the vtable, destroys the
// owned members, then tail-calls the base (Fighter/Player) destructor.
extern "C" char PlayerFighter_vtable;   // vtable symbol base

extern "C" void *_ZN13PlayerFighterD1Ev(PlayerFighter *self)
{
    *(void **)self = &PlayerFighter_vtable + 8;

    void *r = F<void *>(self, 0x144);
    if (r != 0) operator_delete(Route_dtor(r));
    F<void *>(self, 0x144) = 0;

    void *bv = F<void *>(self, 0x150);
    if (bv != 0) {
        ArrayReleaseClasses_BV(bv);
        void *bv2 = F<void *>(self, 0x150);
        if (bv2 != 0) operator_delete(ArrayBV_dtor(bv2));
    }
    F<void *>(self, 0x150) = 0;

    void *t = F<void *>(self, 0x154);
    if (t != 0) operator_delete(Trail_dtor(t));
    F<void *>(self, 0x154) = 0;

    void *e = F<void *>(self, 0x124);
    if (e != 0) operator_delete(Explosion_dtor(e));
    F<void *>(self, 0x124) = 0;

    void *m = F<void *>(self, 0x2b8);
    if (m != 0) operator_delete(EaseInOutMatrix_dtor(m));
    F<void *>(self, 0x2b8) = 0;

    return PlayerFighter_base_dtor(self);
}

// ---- _PlayerFighter_dc922.cpp ----
// PlayerFighter deleting destructor (D0): run the complete-object dtor, then tail-call delete.

extern "C" void _ZN13PlayerFighterD0Ev(PlayerFighter *self)
{
    return operator_delete(_ZN13PlayerFighterD1Ev(self));
}

// ---- hasCrateCaptured_dcb88.cpp ----
extern "C" uint8_t PlayerFighter_hasCrateCaptured(PlayerFighter *self)
{
    return F<uint8_t>(self, 0x4c) == 0;
}

// ---- setPosition_dcb9a.cpp ----
// setPosition(Vector const&) — virtual dispatch through vtable slot 0x48 with the
// three unpacked vector components.
typedef void (*SetPosFn)(PlayerFighter *, float, float, float);

extern "C" void PlayerFighter_setPosition_ref(PlayerFighter *self, const Vector &v)
{
    SetPosFn fn = *(SetPosFn *)(*(char **)self + 0x48);
    return fn(self, v.x, v.y, v.z);
}

// ---- setRotate_dccdc.cpp ----
extern "C" void PlayerFighter_setRotate(PlayerFighter *self, int v)
{
    float f = (float)v;
    F<uint8_t>(self, 0x13d) = 0;
    F<float>(self, 0x1a4) = f;
    F<float>(self, 0x1f0) = f;
}

// ---- hasCrateLost_dcb94.cpp ----
extern "C" uint8_t PlayerFighter_hasCrateLost(PlayerFighter *self)
{
    return F<uint8_t>(self, 0x6a);
}

// ---- setLevel_dca30.cpp ----
struct Level;
// Three function pointers fetched once from hidden PC-relative globals (each a ptr-to-fnptr).
extern void *const gSL_f1 __attribute__((visibility("hidden")));
extern void *const gSL_f2 __attribute__((visibility("hidden")));
extern void *const gSL_f3 __attribute__((visibility("hidden")));

typedef int (*F1)(int geom);
typedef int (*F2)(int base, int v, int kind, int z);
typedef void (*F3)(int base, int v, int z);

extern "C" void PlayerFighter_setLevel(PlayerFighter *self, Level *lvl)
{
    KIPlayer_setLevel(self, lvl);
    F1 f1 = (F1)gSL_f1;
    F2 f2 = (F2)gSL_f2;
    F3 f3 = (F3)gSL_f3;
    int base = F<int32_t>(self, 0x54);

    int v;
    v = f2(*(int *)(base + 0x74), f1(F<int32_t>(self, 8)), 9, 0);
    F<int32_t>(self, 0x1a0) = v;
    f3(*(int *)(F<int32_t>(self, 0x54) + 0x74), v, 0);

    v = f2(*(int *)(F<int32_t>(self, 0x54) + 0x78), f1(F<int32_t>(self, 8)), 0xf, 0);
    F<int32_t>(self, 0x80) = v;
    f3(*(int *)(F<int32_t>(self, 0x54) + 0x78), v, 0);

    v = f2(*(int *)(F<int32_t>(self, 0x54) + 0x84), f1(F<int32_t>(self, 8)), 0x2a, 0);
    F<int32_t>(self, 0x84) = v;
    f3(*(int *)(F<int32_t>(self, 0x54) + 0x84), v, 0);

    v = f2(*(int *)(F<int32_t>(self, 0x54) + 0x8c), f1(F<int32_t>(self, 8)), 0x11, 0);
    F<int32_t>(self, 0x134) = v;
    f3(*(int *)(F<int32_t>(self, 0x54) + 0x8c), v, 0);

    v = f2(*(int *)(F<int32_t>(self, 0x54) + 0x8c), f1(F<int32_t>(self, 8)), 0x12, 0);
    F<int32_t>(self, 0x138) = v;
    return f3(*(int *)(F<int32_t>(self, 0x54) + 0x8c), v, 0);
}

// ---- PlayerFighter_dc2f8.cpp ----
extern "C" void KIPlayer_ctor(PlayerFighter *self, int p1, int wingmanCmd, void *player,
                              void *geom, float a, float b, float c, int flag);

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

// ---- update_dcfd8.cpp ----
// Tail veneer taken on the dead+exploded early-out path (-> 0x1abe18).
// The remaining per-frame update: AI steering, weapons, cloaking, trail, camera, route
// following. A ~10KB state machine; delegated to a helper so this translation keeps the
// recoverable entry flow (early-out, timers, position, enemy flag) while staying tractable.

extern void *const gUpd_guard __attribute__((visibility("hidden")));   // DAT_000ed348

// PlayerFighter::update(int dt) — self in r0, dt in r1.
extern "C" void PlayerFighter_update(PlayerFighter *self, int dt)
{
    int *guardP = *(int **)gUpd_guard;
    volatile int saved = *guardP;

    // Dead-and-explosion-finished early-out: tear down via the dead veneer.
    if (F<int32_t>(self, 0x88) == 4 && Explosion_isPlaying(F<int32_t>(self, 0x124)) == 0 &&
        (F<signed char>(self, 0x4c) == 0 || 60000 < F<int32_t>(self, 0xd8))) {
        if (*guardP != saved) {
            __stack_chk_fail((unsigned)(*guardP - saved));
        }
        PF_update_dead(self);
        return;
    }

    // Advance per-frame timers.
    F<int32_t>(self, 0x1c0) += dt;
    F<int32_t>(self, 0x1b8) += dt;
    if (F<int32_t>(self, 0x28) == 1) {
        F<int32_t>(self, 0x28) = 1;
    }
    F<int32_t>(self, 0x1d0) = dt;
    F<int32_t>(self, 0x1d4) = dt >> 31;

    // Sync world position from the geometry.
    float pos[3];
    AEGeometry_getPosition(pos, F<int32_t>(self, 8));
    AEMath_VectorAssign((char *)self + 0x2c, pos);

    // Recompute the "is enemy" flag unless the ship is in a non-combat mode.
    if (F<signed char>(self, 0x43) == 0) {
        unsigned char enemy;
        if ((F<uint32_t>(self, 0x28) & 0xfffffffe) == 8) {
            enemy = 1;
        } else if (KIPlayer_isWingMan(self) != 0) {
            enemy = 0;
        } else {
            enemy = (unsigned char)Standing_isEnemy(Status_getStanding());
        }
        *(unsigned char *)(F<int32_t>(self, 4) + 0x5c) = enemy;
    }

    // Remaining per-frame state machine.
    PF_update_body(self, dt);

    if (*guardP == saved) {
        return;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
}

// ---- setPosition_dcbb8.cpp ----
// PlayerFighter::setPosition(float, float, float). r0=self, r1..r3 = x,y,z (raw bits).
extern "C" void PlayerFighter_setPosition3(PlayerFighter *self, int x, int y, int z)
{
    volatile uint32_t stackGuard = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard;

    F<int32_t>(self, 0x58) = x;
    F<int32_t>(self, 0x5c) = y;
    F<int32_t>(self, 0x60) = z;

    int stackVec[3];
    AEGeometry_setPosition3(F<int32_t>(self, 8), 0, 0, 0);  // forwards x,y,z via regs
    AEMath_VectorAssign((char *)self + 0x158, stackVec);
    if (F<int32_t>(self, 0x154) != 0) {
        Trail_reset(F<int32_t>(self, 0x154), F<int32_t>(self, 0x158),
                    F<int32_t>(self, 0x15c), F<int32_t>(self, 0x160));
    }
    int m = AEGeometry_getMatrix2(F<int32_t>(self, 8));
    AEMath_MatrixAssign((char *)*(int *)((char *)self + 4) + 4, (void *)m);

    uint32_t diff = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - stackGuard;
    if (diff == 0) {
        return;
    }
    __stack_chk_fail(diff);
}

// ---- roll_df860.cpp ----
extern void *const gRoll_guard __attribute__((visibility("hidden")));   // DAT_000efa4c
extern const float gRoll_threshold __attribute__((visibility("hidden"))); // DAT_000efa14
extern const float gRoll_f18 __attribute__((visibility("hidden")));
extern const float gRoll_f1c __attribute__((visibility("hidden")));
extern const float gRoll_f20 __attribute__((visibility("hidden")));
extern const float gRoll_f24 __attribute__((visibility("hidden")));
extern const float gRoll_f28 __attribute__((visibility("hidden")));
extern const float gRoll_f2c __attribute__((visibility("hidden")));
extern const float gRoll_f30 __attribute__((visibility("hidden")));
extern const float gRoll_f34 __attribute__((visibility("hidden")));
extern const float gRoll_f38 __attribute__((visibility("hidden")));
extern const float gRoll_f3c __attribute__((visibility("hidden")));
extern const float gRoll_f40 __attribute__((visibility("hidden")));
extern const float gRoll_f44 __attribute__((visibility("hidden")));
extern const float gRoll_f48 __attribute__((visibility("hidden")));

// PlayerFighter::roll(int angle) — self in r0, angle in r1.
extern "C" void PlayerFighter_roll(PlayerFighter *self, int angle)
{
    int *guardP = *(int **)gRoll_guard;
    volatile int saved = *guardP;

    if (F<signed char>(self, 0x100) == 0) {
        goto done;
    }

    {
        int m = AEGeometry_getMatrix(F<int32_t>(self, 8));
        float fwdX = *(float *)(m + 0x10);
        float fwdY = *(float *)(m + 0x14);
        float absX = (0.0f < fwdX) ? fwdX : -fwdX;
        if (0x3b < angle) angle = 0x3c;

        // Snap-back to identity when banked back near level.
        if (fwdY >= 0.0f && absX < gRoll_threshold) {
            unsigned char tmp[60];
            AEMath_MatrixIdentity(tmp, (char *)self + 0x258);
            AEMath_MatrixAssign((char *)self + 0x258, tmp);
            F<signed char>(self, 0x100) = 0;
            F<short>(self, 0x254) = 0;
            goto done;
        }

        float bank;    // fVar11
        float roll;    // fVar10 (z rotation)
        if (F<signed char>(self, 0x254) != 0) {
            roll = (fwdY == fwdY) ? gRoll_f20 : gRoll_f1c;   // NaN(fVar10) selection
            bank = (fwdX < 0.0f) ? roll : gRoll_f18;
        } else {
            bank = 0.0f;
            roll = 0.0f;
            if (fwdX > 0.0f && F<signed char>(self, 0x255) == 1) {
                F<signed char>(self, 0x254) = 1;
                bank = gRoll_f48;
            } else if (fwdX < 0.0f && F<signed char>(self, 0x255) == 2) {
                F<signed char>(self, 0x254) = 1;
                bank = gRoll_f44;
            } else if (fwdX >= 0.0f) {
                if (fwdY >= 0.0f) {
                    bank = absX;
                    const float *p = (gRoll_f38 < fwdX) ? &gRoll_f40 : &gRoll_f3c;
                    bank = *p;
                } else {
                    bank = gRoll_f24;
                }
            } else {
                bank = gRoll_f28;
                if (fwdY >= 0.0f) {
                    const float *p = &gRoll_f30;
                    if (fwdX != gRoll_f2c && fwdX < gRoll_f2c) p = &gRoll_f34;
                    bank = *p;
                }
            }
        }

        if (fwdX != 0.0f) {
            F<signed char>(self, 0x255) = (fwdX < 0.0f) ? 1 : 2;
        }
        float fa = VectorSignedToFloat(angle, 0);
        F<signed char>(self, 0x100) = 1;
        unsigned char tmp[60];
        AEMath_MatrixSetRotation(tmp, bank * fa, 0.0f, roll);
        AEMath_MatrixAssign((char *)self + 0x258, tmp);
    }

done:
    if (*guardP == saved) {
        return;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
}

// ---- cloak_dcd58.cpp ----
extern "C" void *gCloakRand;  // hidden PC-relative global (deref'd twice)

extern "C" void PlayerFighter_cloak(PlayerFighter *self, int dur, bool b)
{
    unsigned v;
    if (dur > 0) {
        v = (unsigned)dur;
    } else {
        v = AERandom_nextInt(**(int **)&gCloakRand) + 5000;
    }
    F<uint8_t>(self, 0x2d0) = 1;
    F<uint32_t>(self, 0x2cc) = v + 4000;
    F<uint8_t>(self, 0x2d9) = b;
}

// ---- setMissionCrate_dcb18.cpp ----
// hidden PC-relative global: the App singleton pointer, deref'd twice.
extern void *const gMissionCrateApp __attribute__((visibility("hidden")));

extern "C" void PlayerFighter_setMissionCrate(PlayerFighter *self, bool on)
{
    F<uint8_t>(self, 0xd0) = on;
    if (on) {
        F<Array<int> *>(self, 0x50) = 0;
        Array<int> *a = (Array<int> *)operator_new(0xc);
        ArrayInt_ctor(a);
        F<Array<int> *>(self, 0x50) = a;
        int mission = Status_getMission(*(unsigned *)gMissionCrateApp);
        int type = Mission_getType(mission);
        int item = (type == 5) ? 0x74 : 0x75;
        ArrayInt_add(item, F<Array<int> *>(self, 0x50));
        PlayerFighter_setMissionCrate_tail(1, F<Array<int> *>(self, 0x50));
    }
}

// ---- collide_dfbe8.cpp ----
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

// ---- setBV_dfb8a.cpp ----
struct BoundingVolume;

extern "C" void PlayerFighter_setBV_b(PlayerFighter *self, BoundingVolume *bv)
{
    Array<BoundingVolume *> *a = (Array<BoundingVolume *> *)operator_new(0xc);
    ArrayBV_ctor(a);
    F<Array<BoundingVolume *> *>(self, 0x150) = a;
    return PlayerFighter_setBV_add(bv, a);
}

// ---- setWingmanCommand_dcc2c.cpp ----
struct KIPlayer;

extern "C" void PlayerFighter_setWingmanCommand(PlayerFighter *self, int cmd, KIPlayer *target)
{
    int saved = F<int32_t>(self, 0xe4);
    KIPlayer_setWingmanCommand(self, cmd, target);
    if ((unsigned)(cmd - 2) < 2) {
        F<int32_t>(self, 0x1b8) = 0x1389;
        if (F<float>(self, 0x1e8) != 5.5f) {
            F<int32_t>(self, 0x1c0) = 0x1389;
        }
        if (cmd == 2) {
            if (Level_getPlayerRoute(F<int32_t>(self, 0x54)) != 0) {
                int r = Level_getPlayerRoute(F<int32_t>(self, 0x54));
                F<int32_t>(self, 0x14c) = Route_getExactClone(r);
                F<int32_t>(self, 0x1e4) = Route_getCurrent(F<int32_t>(self, 0x14c));
                goto done;
            }
        } else if (target != 0) {
            goto done;
        }
        F<int32_t>(self, 0xe4) = saved;
    } else if (cmd == 0) {
        F<int32_t>(self, 0xe4) = saved;
        F<uint32_t>(self, 0x140) = (F<int32_t>(self, 0x140) == 0);
    } else if (cmd == 1) {
        F<uint8_t>(self, 0x13d) = 0;
        F<uint8_t>(self, 0x1e0) = 0;
    }
done:
    F<uint32_t>(self, 0x1e8) = F<uint32_t>(self, 0x1ac);
    F<uint32_t>(self, 0x1f0) = F<uint32_t>(self, 0x1a4);
}

// ---- setSpeed_dcccc.cpp ----
extern "C" void PlayerFighter_setSpeed(PlayerFighter *self, float v)
{
    F<float>(self, 0x1ac) = v;
    F<uint8_t>(self, 0x13d) = 0;
    F<float>(self, 0x1e8) = v;
}

// ---- outerCollide_dfc38.cpp ----
struct BV;
typedef int (*CollFn)(BV *, float, float, float);

extern "C" int PlayerFighter_outerCollide(PlayerFighter *self, float x, float y, float z)
{
    if ((unsigned)(F<int32_t>(self, 0x88) - 3) > 1) {
        Array<BV *> *a = F<Array<BV *> *>(self, 0x150);
        if (a != 0) {
            for (unsigned i = 0; i < a->length; i++) {
                BV *e = a->data[i];
                CollFn fn = *(CollFn *)(*(char **)e + 0xc);
                if (fn(e, x, y, z) != 0) {
                    return 1;
                }
                a = F<Array<BV *> *>(self, 0x150);
            }
        }
    }
    return 0;
}

// ---- initPush_dfc88.cpp ----
extern "C" void AEMath_VectorSub(void *out, void *a, void *b);     // operator-(out, a) with b in r2
extern "C" void AEMath_VectorAssign(void *dst, void *src);         // Vector::operator=
extern "C" void AEMath_VectorScale(void *out, float s, void *v);   // operator*(s, v)

extern void *const gIP_guard __attribute__((visibility("hidden")));    // DAT_000efdbc
extern const float gIP_strength __attribute__((visibility("hidden"))); // DAT_000efdb4
extern void *const gIP_rngFn __attribute__((visibility("hidden")));     // DAT_000efdc0 (fn ptr)
extern void *const gIP_rng __attribute__((visibility("hidden")));       // DAT_000efdc4 (rng holder)

typedef void (*GetPosFn)(void *outVec, PlayerFighter *self);
typedef int (*RngFn)(int rng, int bound);

// PlayerFighter::initPush(Vector const& target, int radius) — target in r1, radius in r2.
extern "C" void PlayerFighter_initPush(PlayerFighter *self, void *target, int radius)
{
    int *guardP = *(int **)gIP_guard;
    volatile int saved = *guardP;

    // pos = self->getPosition()  (vtable slot +0x28)
    GetPosFn getPos = *(GetPosFn *)(*(char **)self + 0x28);
    float pos[3];
    getPos(pos, self);

    float diff[3];
    AEMath_VectorSub(diff, target, pos);   // diff = pos - target
    float len = AEMath_VectorLength(diff);
    float r = VectorSignedToFloat(radius, 0);
    float t = 1.0f;
    if (len / r < 1.0f) t = len / r;
    int strength = (int)((1.0f - t) * gIP_strength);
    F<int32_t>(self, 0x104) = strength;
    F<int32_t>(self, 0x108) = strength;

    float pos2[3];
    getPos(pos2, self);
    float dir[3];
    AEMath_VectorSub(dir, pos, pos2);
    float norm[3];
    AEMath_VectorNormalize(norm, dir);
    AEMath_VectorAssign((char *)self + 0x10c, norm);

    RngFn rng = (RngFn)gIP_rngFn;
    int *rngObj = *(int **)gIP_rng;
    float rx = VectorSignedToFloat(rng(*rngObj, 200) - 100, 0);
    float ry = VectorSignedToFloat(rng(*rngObj, 200) - 100, 0);
    float rz = VectorSignedToFloat(rng(*rngObj, 200) - 100, 0);
    float rvec[3] = {rx, ry, rz};
    float rnorm[3];
    AEMath_VectorNormalize(rnorm, rvec);
    float scaled[3];
    AEMath_VectorScale(scaled, (float)strength, rnorm);
    AEMath_VectorAssign((char *)self + 0x118, scaled);

    if (*guardP == saved) {
        return;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
}

// ---- setExhaustVisible_dcd24.cpp ----
// hidden PC-relative pointer-to-pointer global (deref'd twice).
extern void *const gExhaustCanvas __attribute__((visibility("hidden")));

extern "C" void PlayerFighter_setExhaustVisible(PlayerFighter *self, bool vis)
{
    int geom = F<int32_t>(self, 0x8);
    if (geom != 0) {
        int sub = F<int32_t>(self, 0xc);
        int id = (sub != 0) ? *(int *)(sub + 0x14) : *(int *)(geom + 0x14);
        if (id != -1) {
            unsigned t = PaintCanvas_TransformGetTransform(*(unsigned *)gExhaustCanvas);
            return PlayerFighter_setExhaustVisible_apply(t, vis);
        }
    }
}

// ---- render_dfa50.cpp ----
extern void *const gR_g3 __attribute__((visibility("hidden")));   // case 3 transform-id global
extern void *const gR_g4 __attribute__((visibility("hidden")));   // case 4 transform-id global
extern void *const gR_g5 __attribute__((visibility("hidden")));   // default-case transform-id global

extern "C" void PlayerFighter_render(PlayerFighter *self)
{
    if (F<int32_t>(self, 0x78) != 0) {
        AEGeometry_render(0);
    }
    int active = Player_isActive(F<int32_t>(self, 4));
    int st = F<int32_t>(self, 0x88);
    unsigned *idp;
    if (active != 0 && (unsigned)(st - 3) < 2) {
        if (F<int32_t>(self, 0x124) != 0) {
            Explosion_render(0);
            st = F<int32_t>(self, 0x88);
        }
        if (st == 4) {
            if (F<int32_t>(self, 0x1f8) >= 300) {
                goto done;
            }
            idp = *(unsigned **)gR_g4;
        } else {
            if (st != 3) {
                goto done;
            }
            if (F<int32_t>(self, 0xc) == 0) {
                return PlayerFighter_render_tail(F<int32_t>(self, 8));
            }
            idp = *(unsigned **)gR_g3;
        }
    } else {
        if (active != 0 || st != 5) {
            goto done;
        }
        if (F<int32_t>(self, 0xc) == 0) {
            AEGeometry_render(0);
        } else {
            idp = *(unsigned **)gR_g5;
            unsigned char tmp[60];
            unsigned id = *idp;
            void *src = (void *)PaintCanvas_TransformGetLocal(id);
            __aeabi_memcpy(tmp, src, 0x3c);
            PaintCanvas_TransformSetLocal(*idp, *(void **)(F<int32_t>(self, 0xc) + 0xc));
            AEGeometry_render(0);
            PaintCanvas_TransformSetLocal(*idp, *(void **)(F<int32_t>(self, 0xc) + 0xc));
        }
        if (F<int32_t>(self, 0x154) != 0) {
            Trail_render(0);
        }
        goto done;
    }
    {
        unsigned char tmp[60];
        unsigned id = *idp;
        void *src = (void *)PaintCanvas_TransformGetLocal(id);
        __aeabi_memcpy(tmp, src, 0x3c);
        PaintCanvas_TransformSetLocal(*idp, *(void **)(F<int32_t>(self, 0xc) + 0xc));
        AEGeometry_render(0);
        PaintCanvas_TransformSetLocal(*idp, *(void **)(F<int32_t>(self, 0xc) + 0xc));
    }
done:
    ;
}

// ---- push_dfdc8.cpp ----
extern "C" void AEMath_MatrixMul(void *out, void *m);          // operator*(out, m)
extern "C" float l2f(long long v);                              // __aeabi_l2f
// Vector scale helper used 3x: out = vec * scalar.

extern void *const gPush_guard __attribute__((visibility("hidden")));     // DAT_000eff24
extern const float gPush_div __attribute__((visibility("hidden")));       // DAT_000eff20

// PlayerFighter::push(int dt) — self in r0, dt in r1.
extern "C" void PlayerFighter_push(PlayerFighter *self, int dt)
{
    int *guardP = *(int **)gPush_guard;
    volatile int saved = *guardP;

    if (0 < F<int32_t>(self, 0x104)) {
        int remaining = F<int32_t>(self, 0x104) - dt;
        float fr = VectorSignedToFloat(remaining, 0);
        float ftotal = VectorSignedToFloat(F<int32_t>(self, 0x108), 0);
        F<int32_t>(self, 0x104) = remaining;
        float frac = fr / ftotal;

        unsigned char rot[60];
        AEMath_MatrixSetRotation(rot, frac * *(float *)((char *)self + 0x120), 0.0f,
                                 frac * *(float *)((char *)self + 0x11c));

        int lo = F<int32_t>(self, 0x1d0);
        int hi = F<int32_t>(self, 0x1d4);
        if ((int)(unsigned)(lo == 0) <= hi) {
            void *geom = F<void *>(self, 8);
            void *m = (void *)(long)AEGeometry_getMatrix((int)(long)geom);
            AEMath_MatrixMul(rot, m);
            AEGeometry_setMatrix(geom);
            lo = F<int32_t>(self, 0x1d0);
            hi = F<int32_t>(self, 0x1d4);
        }
        float speed = l2f(((long long)hi << 32) | (unsigned)lo);
        void *geom = F<void *>(self, 8);
        float ftotal2 = VectorSignedToFloat(F<int32_t>(self, 0x108), 0);

        unsigned char a[12], b[12], c[60];
        PF_vscale(a, (char *)self + 0x10c, speed);
        PF_vscale(b, a, *(float *)((char *)self + 0x1e8));
        PF_vscale(c, b, (2.0f - frac) * 3.0f * (ftotal2 / gPush_div));
        AEGeometry_translate(geom);
    }

    if (*guardP == saved) {
        return;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
}

// ---- reset_dc934.cpp ----
// hidden PC-relative function-pointer global (Vector zero-set helper), used 3x.
extern void *const gReset_vfn __attribute__((visibility("hidden")));

typedef void (*VFn)(void *dst, void *zeroVec);

__attribute__((minsize)) extern "C" void PlayerFighter_reset(PlayerFighter *self)
{
    volatile uint32_t stackGuard = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard;

    KIPlayer_reset(self);
    F<uint8_t>(self, 0x4c) = 1;

    int v[3];
    v[0] = F<int32_t>(self, 0x58);
    v[1] = F<int32_t>(self, 0x5c);
    v[2] = F<int32_t>(self, 0x60);
    AEMath_VectorAssign((char *)self + 0x158, v);
    AEMath_VectorAssign((char *)self + 0x2c, v);

    F<int32_t>(self, 0x1d0) = 0;
    F<int32_t>(self, 0x1d4) = 0;
    F<int32_t>(self, 0x38) = 0;
    F<uint8_t>(self, 0x12e) = 0;
    F<int32_t>(self, 0x148) = 0;
    F<uint8_t>(self, 0x12c) = 0;
    F<int32_t>(self, 0x1b8) = 0;
    F<int32_t>(self, 0x1c0) = 0;
    F<int32_t>(self, 0x1c4) = 0;
    F<int32_t>(self, 0x1c8) = 0;
    if (F<int32_t>(self, 0x88) != 5) {
        F<int32_t>(self, 0x88) = 0;
    }

    VFn vfn = (VFn)gReset_vfn;
    int z[3] = {0, 0, 0};
    vfn((char *)self + 0x90, z);
    vfn((char *)self + 0x164, z);
    vfn((char *)self + 0x170, z);

    F<uint8_t>(self, 0xd0) = 0;
    F<uint16_t>(self, 0x68) = 0;
    F<uint8_t>(self, 0x6a) = 0;
    F<uint8_t>(self, 0x4c) = 1;
    F<int32_t>(self, 0x140) = 0;
    F<uint8_t>(self, 0x1fc) = 0;
    F<uint8_t>(self, 0x13c) = 0;
    F<uint8_t>(self, 0x2d0) = 0;
    F<int32_t>(self, 0x2c8) = 0;
    F<int32_t>(self, 0x2cc) = 0;
    F<int32_t>(self, 0x2d4) = 0;
    F<uint8_t>(self, 0x2d8) = 1;
    F<uint8_t>(self, 0x2e4) = 0;

    uint32_t diff = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - stackGuard;
    if (diff == 0) {
        return;
    }
    __stack_chk_fail(diff);
}

// ---- handleCloaking_dcdbc.cpp ----
// Tail-call veneers (cloak start / stop / material-fade-apply).
extern "C" void PF_cloakStart(PlayerFighter *self);                       // -> 0x1ac1e8
extern "C" void PF_cloakStop(PlayerFighter *self, int on);               // -> 0x1ac1f8
extern "C" void PF_cloakApply(void *meshPtr, int arg, float alpha, int flag); // -> 0x1ac208

extern void *const gHC_rng1 __attribute__((visibility("hidden")));        // DAT_000ecfb0
extern void *const gHC_canvasClone __attribute__((visibility("hidden"))); // DAT_000ecfb8
extern void *const gHC_canvasMat __attribute__((visibility("hidden")));   // DAT_000ecfbc
extern void *const gHC_rng2 __attribute__((visibility("hidden")));        // DAT_000ecfb4
extern void *const gHC_canvasA __attribute__((visibility("hidden")));     // DAT_000ecfc0
extern void *const gHC_canvasB __attribute__((visibility("hidden")));     // DAT_000ecfc4
extern void *const gHC_canvasC __attribute__((visibility("hidden")));     // DAT_000ecfc8
extern const float gHC_divIn __attribute__((visibility("hidden")));       // DAT_000ecfac
extern const float gHC_divOut __attribute__((visibility("hidden")));      // DAT_000ecfa8

// PlayerFighter::handleCloaking() — self in r0.
extern "C" void PlayerFighter_handleCloaking(PlayerFighter *self)
{
    if (F<int32_t>(self, 0x28) != 10) return;
    if (F<int32_t>(self, 8) == 0) return;
    if (KIPlayer_isDead(self) != 0) return;
    if (*(char *)(F<int32_t>(self, 4) + 0x68) != 0) return;
    if (F<signed char>(self, 0x2d8) == 0) return;

    if (F<signed char>(self, 0x2d0) != 0) {
        int delta = F<int32_t>(self, 0x2c8);
        if (delta == 0) {
            unsigned matId = F<uint32_t>(self, 0x2dc);
            F<signed char>(self, 0x13c) = 1;
            if (matId == 0xffffffff) {
                void **cv = *(void ***)gHC_canvasClone;
                PaintCanvas_MeshCloneMaterial(*cv, *(unsigned *)(F<int32_t>(self, 0xc) + 0x1c),
                                              (unsigned *)((char *)self + 0x2dc));
                int mp = PaintCanvas_MeshGetPointer(*cv,
                                                    *(unsigned *)(F<int32_t>(self, 0xc) + 0x1c));
                matId = F<uint32_t>(self, 0x2dc);
                F<int32_t>(self, 0x2e0) = *(int *)(*(int *)(mp + 0x30) + 0x20);
            }
            void **cvm = *(void ***)gHC_canvasMat;
            int mat = PaintCanvas_MaterialGetMaterial(*cvm, matId);
            *(int *)(mat + 0x20) = 0xe;
            PaintCanvas_MeshChangeMaterial(*cvm, *(unsigned *)(F<int32_t>(self, 0xc) + 0x1c),
                                           *(unsigned short *)((char *)self + 0x2dc));
            if (F<signed char>(self, 0x2d9) == 0) {
                delta = F<int32_t>(self, 0x2c8);
            } else {
                F<signed char>(self, 0x2d9) = 0;
                delta = 2000;
                F<int32_t>(self, 0x2c8) = 2000;
            }
        }
        long long acc = (long long)delta +
                        (((long long)F<int32_t>(self, 0x1d4) << 32) | (unsigned)F<int32_t>(self, 0x1d0));
        int total = (int)acc;
        F<int32_t>(self, 0x2c8) = total;

        if (total - F<int32_t>(self, 0x1d0) <= 2000) {
            if (1999 < total) {
                PlayerFighter_setExhaustVisible(self, false);
                F<signed char>(self, 0x74) = 1;
            }
            void *cv = *(void **)gHC_canvasA;
            cv = *(void **)cv;
            int mp = PaintCanvas_MeshGetPointer(cv, *(unsigned *)(F<int32_t>(self, 0xc) + 0x1c));
            float a = VectorSignedToFloat(F<int32_t>(self, 0x2c8), 0) / gHC_divIn;
            PF_cloakApply((void *)(long)mp, (int)(long)cv, a, 1);
            return;
        } else {
            if (F<int32_t>(self, 0x2cc) < total) {
                int restore = F<int32_t>(self, 0x2e0);
                void **cvc = *(void ***)gHC_canvasC;
                F<int32_t>(self, 0x2c8) = 0;
                F<signed char>(self, 0x2d0) = 0;
                F<signed char>(self, 0x13c) = 0;
                int mat = PaintCanvas_MaterialGetMaterial(*cvc, F<uint32_t>(self, 0x2dc));
                *(int *)(mat + 0x20) = restore;
                PF_cloakStop(self, 1);
                return;
            }
            if (total <= F<int32_t>(self, 0x2cc) - 2000) {
                return;
            }
            void **cvb = *(void ***)gHC_canvasB;
            F<signed char>(self, 0x74) = 0;
            void *cv = *cvb;
            int mp = PaintCanvas_MeshGetPointer(cv, *(unsigned *)(F<int32_t>(self, 0xc) + 0x1c));
            float a = VectorSignedToFloat(F<int32_t>(self, 0x2c8), 0);
            float b = VectorSignedToFloat(F<int32_t>(self, 0x2cc) - 2000, 0);
            float alpha = (a - b) / gHC_divOut + 1.0f;
            PF_cloakApply((void *)(long)mp, (int)(long)cv, alpha, 1);
            return;
        }
    }

    if (F<signed char>(self, 0x1e0) != 0 && AERandom_nextInt(**(int **)gHC_rng1) < 0x32) {
        PF_cloakStart(self);
        return;
    }
    int acc = F<int32_t>(self, 0x1d0) + F<int32_t>(self, 0x2d4);
    F<int32_t>(self, 0x2d4) = acc;
    if (8000 < acc) {
        F<int32_t>(self, 0x2d4) = 0;
        if (AERandom_nextInt(**(int **)gHC_rng2) < 0x1e) {
            PF_cloakStart(self);
        }
    }
}

// ---- revive_dff5c.cpp ----
__attribute__((minsize)) extern "C" void PlayerFighter_revive(PlayerFighter *self)
{
    volatile uint32_t stackGuard = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard;

    int enemy = Player_turnedEnemy(F<int32_t>(self, 4));
    Player_reset(F<int32_t>(self, 4));
    if (enemy != 0) {
        Player_turnEnemy(F<int32_t>(self, 4));
    }
    {
        String s;
        AEString_ctor_default(&s);
        AEString_assign((char *)self + 0x18, &s);
        AEString_dtor(&s);
    }
    F<int32_t>(self, 0x78) = 0;
    F<int32_t>(self, 0x88) = 1;
    F<uint8_t>(self, 0x12e) = 0;
    F<int32_t>(self, 0x38) = -1;
    Route_reset(F<int32_t>(self, 0x6c));
    F<int32_t>(self, 0x1d8) = Player_getHitpoints(F<int32_t>(self, 4));
    F<int32_t>(self, 0x1dc) = 0;
    F<uint8_t>(self, 0x1e0) = 0;
    F<int32_t>(self, 0xd8) = 0;
    KIPlayer_setActive(self, 1);
    F<int32_t>(self, 0xf0) = 0;
    F<int32_t>(self, 0x1e8) = F<int32_t>(self, 0x1ac);
    Explosion_reset(F<int32_t>(self, 0x124));
    F<int32_t>(self, 0x104) = 0;
    F<uint8_t>(self, 0x24) = 0;
    PlayerFighter_setExhaustVisible(self, true);
    int geom = F<int32_t>(self, 0xc);
    F<uint8_t>(self, 0xf5) = 1;
    if (geom == 0) {
        geom = F<int32_t>(self, 8);
    }
    AEGeometry_setVisible(geom, 1);
    if ((unsigned)(F<int32_t>(self, 0x28) - 9) < 2) {
        void *a = F<void *>(self, 0x50);
        if (a != 0) {
            operator_delete(ArrayInt_dtor(a));
        }
        F<void *>(self, 0x50) = 0;
    } else {
        void *g = operator_new(1);
        Generator_ctor(g);
        void *a = F<void *>(self, 0x50);
        if (a != 0) {
            operator_delete(ArrayInt_dtor(a));
            F<void *>(self, 0x50) = 0;
        }
        F<int32_t>(self, 0x50) = Generator_getLootList(g, -1, -1);
        operator_delete(Generator_dtor(g));
    }

    uint32_t diff = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - stackGuard;
    if (diff == 0) {
        return;
    }
    __stack_chk_fail(diff);
}

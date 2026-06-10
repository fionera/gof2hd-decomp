#include "gof2/PlayerStaticFar.h"


extern "C" void *PlayerStatic_base_dtor(PlayerStaticFar *self);
extern "C" void operator_delete(void *p);
extern "C" Vector BoundingVolume_getProjectionVector(void *volume, const Vector &value);
extern "C" void PlayerStaticFar_render_tail(PlayerStaticFar *self);
extern "C" void *PlayerStatic_base_dtor_thunk(PlayerStaticFar *self);
extern "C" void Player_setRadius(void *player, int value);

// ---- _PlayerStaticFar_11c2c8.cpp ----
// PlayerStaticFar deleting destructor (D0): PlayerStaticFar has no owned members,
// so it directly runs the base (PlayerStatic) destructor, then tail-calls
// operator delete (the b.w 0x1ab098 trampoline).

extern "C" void _ZN15PlayerStaticFarD0Ev(PlayerStaticFar *self)
{
    return operator_delete(PlayerStatic_base_dtor(self));
}

// ---- getProjectionVector_11c510.cpp ----
Vector PlayerStaticFar::getProjectionVector(const Vector &value)
{
    void *volumes = pp(this, 0x130);
    if (volumes != 0) {
        void *items = pp(volumes, 0x4);
        void *volume = *(void **)items;
        return BoundingVolume_getProjectionVector(volume, value);
    }

    Vector r = {0.0f, 0.0f, 0.0f};
    return r;
}

// ---- render_11c2da.cpp ----
// render(): a single unconditional tail-call thunk (b.w into the PLT-ish
// trampoline). The body is one branch to the resolved render routine.

void PlayerStaticFar::render()
{
    return PlayerStaticFar_render_tail(this);
}

// ---- projectCollisionOnSurface_11c52e.cpp ----
extern "C" Vector BoundingVolume_staticProjectCollisionOnSurface(
    const Vector &value, void *volumes);

Vector PlayerStaticFar::projectCollisionOnSurface(const Vector &value)
{
    void *volumes = pp(this, 0x130);
    if (volumes != 0) {
        return BoundingVolume_staticProjectCollisionOnSurface(value, volumes);
    }

    Vector r = {0.0f, 0.0f, 0.0f};
    return r;
}

// ---- outerCollide_11c4fc.cpp ----
// outerCollide(float,float,float): tail-call the virtual slot at vtable+0x38,
// forwarding the same (this,x,y,z) arguments unchanged.
typedef bool (*OuterCollideFn)(PlayerStaticFar *self, float x, float y, float z);

bool PlayerStaticFar::outerCollide(float x, float y, float z)
{
    OuterCollideFn fn = *(OuterCollideFn *)(*(char **)this + 0x38);
    return fn(this, x, y, z);
}

// ---- _PlayerStaticFar_11c2c4.cpp ----
// PlayerStaticFar base-object destructor (D2): no owned members, so the whole
// body is a single tail-call thunk forwarding to the base PlayerStatic dtor
// (b.w into the trampoline).

extern "C" void *_ZN15PlayerStaticFarD2Ev(PlayerStaticFar *self)
{
    return PlayerStatic_base_dtor_thunk(self);
}

// ---- getInitPosition_11c4ee.cpp ----
// Returns the init position (fields at +0x58,+0x5c,+0x60) by value: sret in r0,
// this in r1. ldrd/strd pair the first two floats; the third is a single str.
Vector PlayerStaticFar::getInitPosition(Vector)
{
    Vector r;
    r.x = f32(this, 0x58);
    r.y = f32(this, 0x5c);
    r.z = f32(this, 0x60);
    return r;
}

// ---- outerCollide_11c506.cpp ----
// outerCollide(Vector): the Vector is passed in r0..? In the target this is a
// 3-instruction thunk: load vtable, fetch slot+0x38, tail-call with args
// already in place. Model as a tail-call through the virtual slot forwarding
// the same registers.
typedef void (*OuterCollideVecFn)(PlayerStaticFar *self, float x, float y, float z);

void PlayerStaticFar::outerCollide(Vector value)
{
    OuterCollideVecFn fn = *(OuterCollideVecFn *)(*(char **)this + 0x38);
    return fn(this, value.x, value.y, value.z);
}

// ---- update_11c2e0.cpp ----
// PlayerStaticFar::update(int):
// When the object has a geometry (field 0x8), reproject the object's stored integer
// position relative to the current camera. If the camera-relative distance is below a
// global limit, hide it (scale 1,1,1 at the integer position); otherwise place it on a
// sphere of fixed radius along the view direction and scale it down by distance.
//
// pcVar3 is AEMath::Vector::operator= (copy 3 floats), resolved at runtime from a global
// function-pointer slot; modeled here as an extern "C" callee.

struct Vec3 { float x, y, z; };

extern "C" {
// Camera / matrix helpers (PaintCanvas + AEMath).
void *AbyssEngine_PaintCanvas_CameraGetCurrent(void *camera);   // 0x717f4
void *AbyssEngine_PaintCanvas_CameraGetLocal(void *camera, void *cur); // 0x6ff1c
void AbyssEngine_AEMath_MatrixGetPosition(Vec3 *out, void *matrix);     // 0x6f16c

// Vector ops.
Vec3 *AEMath_Vector_assign(Vec3 *dst, const Vec3 *src);                  // *pcVar3 (0x73534 path uses r5)
void AbyssEngine_AEMath_operator_sub(Vec3 *out, const Vec3 *a, const Vec3 *b); // 0x6ec38
float AbyssEngine_AEMath_VectorLength(const Vec3 *v);                    // 0x6ec44
void AbyssEngine_AEMath_VectorNormalize(Vec3 *out, const Vec3 *v);       // 0x6ec80
float AEMath_Vector_assign2(Vec3 *dst, const Vec3 *src);                 // 0x6eb3c
void AEMath_Vector_mul_eq(Vec3 *v, float s);                             // 0x72628
void AEMath_Vector_add_eq(Vec3 *v, const Vec3 *o);                       // 0x73534

// AEGeometry placement.
void AEGeometry_setPositionVec(void *geometry, const Vec3 *v);          // 0x72148
void AEGeometry_setPosition(void *geometry, float x, float y, float z); // 0x73048
void AEGeometry_setScaling(void *geometry, float x, float y, float z);  // 0x727b4

// Globals (camera object holder, limits/factors materialized as PC-relative loads).
void *g_PlayerStaticFar_cameraHolder; // **(DAT_0012c458 + 0x12c30a)
int g_PlayerStaticFar_distLimit;      // DAT_0012c444
float g_PlayerStaticFar_radius;       // DAT_0012c448 (sphere radius numerator)
float g_PlayerStaticFar_scaleNum;     // DAT_0012c44c
float g_PlayerStaticFar_scaleFactor;  // DAT_0012c450
}

void PlayerStaticFar::update(int /*delta*/)
{
    if (pp(this, 0x8) == 0)
        return;

    void *camera = g_PlayerStaticFar_cameraHolder;

    Vec3 local;
    void *cur = AbyssEngine_PaintCanvas_CameraGetCurrent(camera);
    void *matrix = AbyssEngine_PaintCanvas_CameraGetLocal(camera, cur);
    AbyssEngine_AEMath_MatrixGetPosition(&local, matrix);

    // this+0x90 = camera position.
    AEMath_Vector_assign((Vec3 *)((char *)this + 0x90), &local);

    // this+0x9c = object's integer position converted to float.
    local.x = (float)i32(this, 0x124);
    local.y = (float)i32(this, 0x128);
    local.z = (float)i32(this, 0x12c);
    AEMath_Vector_assign((Vec3 *)((char *)this + 0x9c), &local);

    // local = (this+0x9c) - (this+0x90); store into this+0x134.
    AbyssEngine_AEMath_operator_sub(&local, (Vec3 *)((char *)this + 0x9c),
                                    (Vec3 *)((char *)this + 0x90));
    Vec3 *dir = (Vec3 *)((char *)this + 0x134);
    AEMath_Vector_assign(dir, &local);

    float len = AbyssEngine_AEMath_VectorLength(dir);

    if ((int)len < g_PlayerStaticFar_distLimit) {
        // Close: full-scale at the literal integer position.
        AEGeometry_setScaling(pp(this, 0x8), 1.0f, 1.0f, 1.0f);
        float fx = (float)i32(this, 0x124);
        float fy = (float)i32(this, 0x128);
        float fz = (float)i32(this, 0x12c);
        AEGeometry_setPosition(pp(this, 0x8), fx, fy, fz);
    } else {
        // Far: place on a sphere of radius g_radius along the view direction.
        Vec3 n;
        AbyssEngine_AEMath_VectorNormalize(&n, dir);
        AEMath_Vector_assign2(dir, &n);
        AEMath_Vector_mul_eq(dir, g_PlayerStaticFar_radius);
        AEMath_Vector_add_eq(dir, (Vec3 *)((char *)this + 0x90));
        AEGeometry_setPositionVec(pp(this, 0x8), dir);

        float s = (float)(int)((g_PlayerStaticFar_radius / (float)(int)len) *
                               g_PlayerStaticFar_scaleNum);
        s = s * g_PlayerStaticFar_scaleFactor;
        AEGeometry_setScaling(pp(this, 0x8), s, s, s);
    }
}

// ---- collide_11c460.cpp ----
// collide(x,y,z): true iff (x,y,z) lies strictly inside the +/- bound box centered
// at the stored integer position (fields 0x124/0x128/0x12c), where bound is the
// radius integer at *(this+4)+0x40 converted to float.
bool PlayerStaticFar::collide(float x, float y, float z)
{
    float px = (float)i32(this, 0x124);
    float dx = x - px;
    int boundI = i32(pp(this, 0x4), 0x40);
    float bound = (float)boundI;
    if (dx < bound) {
        float negBound = (float)(-boundI);
        if (dx > negBound) {
            float py = (float)i32(this, 0x128);
            float dy = y - py;
            if (dy < bound) {
                if (dy > negBound) {
                    float pz = (float)i32(this, 0x12c);
                    float dz = z - pz;
                    if (dz < bound && dz > negBound) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

// ---- PlayerStaticFar_11c254.cpp ----
// Base PlayerStatic constructor (blx 0x75fdc) and Player::setRadius (blx 0x730d8).
extern "C" void PlayerStatic_ctor(PlayerStaticFar *self, int playerId,
                                  AEGeometry *geometry, float x, float y, float z);

// Hidden-visibility vtable so the address is materialized via a PC-relative load.
__attribute__((visibility("hidden"))) extern void *volatile g_PlayerStaticFar_vtable;

PlayerStaticFar::PlayerStaticFar(int playerId, AEGeometry *geometry, float x, float y, float z)
{
    PlayerStatic_ctor(this, playerId, geometry, x, y, z);

    void *vtable = g_PlayerStaticFar_vtable;
    i32(this, 0x134) = 0;
    i32(this, 0x138) = 0;
    i32(this, 0x13c) = 0;
    f32(this, 0x58) = x;
    pp(this, 0x0) = (char *)vtable + 8;
    f32(this, 0x5c) = y;
    f32(this, 0x60) = z;
    Player_setRadius(pp(this, 0x4), 0x1d4c);
    i32(this, 0x130) = 0;
}

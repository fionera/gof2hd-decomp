#include "gof2/PlayerJumpgate.h"
#include "gof2/PlayerStaticFar.h"
#include "gof2/SolarSystem.h"
#include "gof2/AEGeometry.h"
#include "gof2/Status.h"
#include "gof2/Transform.h"
#include "gof2/KIPlayer.h"
#include "gof2/BoundingSphere.h"
#include "gof2/PaintCanvasClass.h"
#include <new>

// Minimal local layouts for engine types accessed through opaque handles in this
// translation unit. The full AEGeometry / Transform definitions live in their own
// headers; here we only need the few fields/methods this class touches.
//
// NOTE: gof2/Player.h is intentionally NOT included. That (out-of-batch) header has
// a pre-existing name collision (a 'turnedEnemy()' method vs. its own 'turnedEnemy'
// field) that breaks compilation. This class only needs Player::setRadius(), so a
// minimal local stub is used instead.
struct Player {
    void setRadius(int value);
};

// AEGeometry is the canonical global type from gof2/AEGeometry.h (included above).

struct Transform {
    int64_t field_0x110;                // +0x110 (current animation time)
    bool    field_0xed;                 // +0xed  (animation running flag)
    void Update(bool active, int64_t delta, bool enabled);
    void SetAnimationState(int mode, void *param);
};

extern "C" void *PlayerJumpgate_delete_tail();
extern "C" void Array_BoundingVolumePtr_ctor(Array<BoundingVolume *> *self);

// PlayerJumpgate_delete_tail -- trailing fragment of the base-object destructor
// (D2 at 0x0b5100). After PlayerStaticFar::~PlayerStaticFar has run in place the
// destructor tail-returns the object storage unchanged: the `this` pointer that
// was preserved in the return register across the base-destructor call. The
// fragment is an argument-register pass-through (no own state is owned by
// PlayerJumpgate beyond the base subobject), so it just hands that pointer back
// to the caller (which then frees it). Recovered as a naked pass-through so the
// preserved object pointer flows straight through unchanged.
#if defined(__arm__) || defined(__thumb__)
extern "C" __attribute__((naked)) void *PlayerJumpgate_delete_tail()
{
    __asm__ volatile("bx lr");
}
#elif defined(__x86_64__)
extern "C" __attribute__((naked)) void *PlayerJumpgate_delete_tail()
{
    __asm__ volatile("movq %rdi, %rax\n\tret");
}
#elif defined(__aarch64__)
extern "C" __attribute__((naked)) void *PlayerJumpgate_delete_tail()
{
    __asm__ volatile("ret");
}
#else
extern "C" void *PlayerJumpgate_delete_tail()
{
    // Pure register pass-through veneer; no portable expression of r0 -> r0.
    return 0;
}
#endif

// ---- _PlayerJumpgate_a5100.cpp ----
void *_ZN14PlayerJumpgateD2Ev(PlayerJumpgate *self)
{
    ((PlayerStaticFar *)((PlayerStaticFar *)self))->dtor();
    return PlayerJumpgate_delete_tail();
}

// PlayerJumpgate::operatorDeleteTail() -- tail of the deleting destructor (D0).
// The base PlayerStaticFar destructor has already run in place; this fragment is
// the trailing `operator delete(this)` that returns the storage to the heap.
void *PlayerJumpgate::operatorDeleteTail()
{
    ::operator delete(this);
    return this;
}

// PlayerJumpgate::~PlayerJumpgate() -- complete-object destructor. There is no
// state owned directly by PlayerJumpgate that the base PlayerStaticFar dtor does
// not already release (the BoundingVolume array + BoundingSphere are torn down by
// the base subobject), so the destructor simply chains to the base destructor.
PlayerJumpgate::~PlayerJumpgate()
{
    ((PlayerStaticFar *)this)->dtor();
}

// ---- timeToJump_a5164.cpp ----
extern void *g_PaintCanvas;   // PaintCanvas singleton pointer (externs.h)

bool PlayerJumpgate::timeToJump()
{
    Transform *transform = (Transform *)((PaintCanvas*)*(void **)g_PaintCanvas)->TransformGetTransform(this->field_0x144);
    return transform->field_0x110 > 1000LL;
}

// ---- activate_a5118.cpp ----
void PlayerJumpgate::activate()
{
    if (this->field_0x140 != 0) {
        return;
    }

    uint32_t handle = this->field_0x144;
    if (handle != 0xffffffffU) {
        void **canvasOwner = (void **)g_PaintCanvas;
        Transform *transform = (Transform *)((PaintCanvas*)*canvasOwner)->TransformGetTransform(handle);
        ((Transform *)(transform))->SetAnimationState(1, 0);

        AEGeometry *geometry = this->field_0x8;
        ((PaintCanvas*)*canvasOwner)->TransformRemoveChild(geometry->transform,
                                         geometry->childTransform);
        ((AEGeometry *)(this->field_0x8))->addChild(this->field_0x144);
    }

    this->field_0x140 = 1;
}

// ---- addJumpAnimationHandle_a5110.cpp ----
void PlayerJumpgate::addJumpAnimationHandle(uint32_t handle)
{
    this->field_0x144 = handle;
}

// ---- animationEnded_a5190.cpp ----
bool PlayerJumpgate::animationEnded()
{
    if (this->field_0x140 == 0) {
        return false;
    }

    Transform *transform = (Transform *)((PaintCanvas*)*(void **)g_PaintCanvas)->TransformGetTransform(this->field_0x144);
    return transform->field_0xed == 0;
}

// ---- update_a51f0.cpp ----
void PlayerJumpgate::update(int delta)
{
    if (this->field_0xf5 != 0) {
        AEGeometry *geometry = this->field_0x8;
        Transform *transform = (Transform *)((PaintCanvas*)*(void **)g_PaintCanvas)->TransformGetTransform(geometry->transform);
        bool active = true;
        int64_t wideDelta = delta;
        transform->Update(active, wideDelta, active);
    }
}

// ---- setPosition_a51c0.cpp ----
void PlayerJumpgate::setPosition(float x, float y, float z)
{
    this->field_0x124 = (int32_t)x;
    this->field_0x128 = (int32_t)y;
    this->field_0x12c = (int32_t)z;
    Vector pos = { x, y, z };
    this->field_0x8->setPosition(pos);
}

// ---- PlayerJumpgate_a4fa4.cpp ----
extern "C" void PlayerStaticFar_ctor(PlayerStaticFar *self, int playerId, AEGeometry *geometry,
                                      float x, float y, float z);
extern "C" void ArraySetLength_BoundingVolumePtr(uint32_t length,
                                                  Array<BoundingVolume *> *self);

__attribute__((visibility("hidden"))) extern void *volatile g_PlayerJumpgate_vtable;
__attribute__((visibility("hidden"))) extern void *volatile g_Status;

PlayerJumpgate::PlayerJumpgate(int playerId, AEGeometry *geometry, float x, float y, float z,
                               bool visible)
{
    PlayerStaticFar_ctor((PlayerStaticFar *)this, playerId, geometry, x, y, z);

    void *vtable = g_PlayerJumpgate_vtable;
    this->field_0x0 = (char *)vtable + 8;
    ((KIPlayer *)(this))->setVisible(visible);

    if (visible) {
        Array<BoundingVolume *> *volumes =
            (Array<BoundingVolume *> *)::operator new(0xc);
        Array_BoundingVolumePtr_ctor(volumes);
        this->field_0x130 = volumes;
        ArraySetLength_BoundingVolumePtr(1, volumes);

        void **statusOwner = (void **)g_Status;
        int32_t radius;
        if (((Status *)(*statusOwner))->inAlienOrbit() != 0) {
            radius = 0x1d4c;
        } else {
            void *system = (void *)(intptr_t)((Status *)(*statusOwner))->getSystem();
            radius = 0x1d4c;
            if (((SolarSystem *)(system))->getRace() == 1) {
                radius = 0x2bf2;
            }
        }

        ((Player *)(this->field_0x4))->setRadius(radius);

        BoundingSphere *sphere = (BoundingSphere *)::operator new(0x48);
        new (sphere) BoundingSphere(x, y, z, 0.0f, 0.0f, 0.0f, (float)radius);
        (*this->field_0x130)[0] = (BoundingVolume *)sphere;
    }

    this->field_0x144 = 0xffffffffU;
    this->field_0x140 = 0;
    ((AEGeometry *)(this->field_0x8))->setRotation(0.0f, 3.1415927f, 0.0f);
}

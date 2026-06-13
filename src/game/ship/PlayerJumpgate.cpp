#include "gof2/game/ship/PlayerJumpgate.h"
#include "gof2/game/ship/PlayerStaticFar.h"
#include "gof2/game/world/SolarSystem.h"
#include "gof2/engine/render/AEGeometry.h"
#include "gof2/game/mission/Status.h"
#include "gof2/engine/math/Transform.h"
#include "gof2/game/ship/KIPlayer.h"
#include "gof2/engine/math/BoundingSphere.h"
#include "gof2/game/core/PaintCanvasClass.h"
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

// PlayerJumpgate base-object destructor (D2 @0x000a50fc). The whole body is a
// single unconditional tail-call (`b.w` long-branch veneer @0x19bdb8 -> PLT stub
// @0x000630f0 -> _ZN15PlayerStaticFarD2Ev, i.e. PlayerStaticFar::~PlayerStaticFar).
// PlayerJumpgate owns no members beyond the base subobject, so the destructor
// simply chains into the base-class destructor -- this IS the inherited tail-call,
// not a separate "pass-through" fragment (the former PlayerJumpgate_delete_tail
// shim was that same veneer, misdecompiled).
void _ZN14PlayerJumpgateD2Ev(PlayerJumpgate *self)
{
    ((PlayerStaticFar *)self)->dtor();
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

extern void *g_PaintCanvas;   // PaintCanvas singleton pointer (externs.h)

bool PlayerJumpgate::timeToJump()
{
    Transform *transform = (Transform *)((PaintCanvas*)*(void **)g_PaintCanvas)->TransformGetTransform(this->transformHandle);
    return transform->field_0x110 > 1000LL;
}

void PlayerJumpgate::activate()
{
    if (this->activated != 0) {
        return;
    }

    uint32_t handle = this->transformHandle;
    if (handle != 0xffffffffU) {
        void **canvasOwner = (void **)g_PaintCanvas;
        Transform *transform = (Transform *)((PaintCanvas*)*canvasOwner)->TransformGetTransform(handle);
        ((Transform *)(transform))->SetAnimationState(1, 0);

        AEGeometry *geometry = this->geometry;
        ((PaintCanvas*)*canvasOwner)->TransformRemoveChild(geometry->transform,
                                         geometry->childTransform);
        ((AEGeometry *)(this->geometry))->addChild(this->transformHandle);
    }

    this->activated = 1;
}

void PlayerJumpgate::addJumpAnimationHandle(uint32_t handle)
{
    this->transformHandle = handle;
}

bool PlayerJumpgate::animationEnded()
{
    if (this->activated == 0) {
        return false;
    }

    Transform *transform = (Transform *)((PaintCanvas*)*(void **)g_PaintCanvas)->TransformGetTransform(this->transformHandle);
    return transform->field_0xed == 0;
}

void PlayerJumpgate::update(int delta)
{
    if (this->visible != 0) {
        AEGeometry *geometry = this->geometry;
        Transform *transform = (Transform *)((PaintCanvas*)*(void **)g_PaintCanvas)->TransformGetTransform(geometry->transform);
        bool active = true;
        int64_t wideDelta = delta;
        transform->Update(active, wideDelta, active);
    }
}

void PlayerJumpgate::setPosition(float x, float y, float z)
{
    this->posX = (int32_t)x;
    this->posY = (int32_t)y;
    this->posZ = (int32_t)z;
    Vector pos = { x, y, z };
    this->geometry->setPosition(pos);
}

extern "C" void PlayerStaticFar_ctor(PlayerStaticFar *self, int playerId, AEGeometry *geometry,
                                      float x, float y, float z);

__attribute__((visibility("hidden"))) extern void *volatile g_PlayerJumpgate_vtable;
__attribute__((visibility("hidden"))) extern void *volatile g_Status;

PlayerJumpgate::PlayerJumpgate(int playerId, AEGeometry *geometry, float x, float y, float z,
                               bool visible)
{
    PlayerStaticFar_ctor((PlayerStaticFar *)this, playerId, geometry, x, y, z);

    void *vtable = g_PlayerJumpgate_vtable;
    this->vtable = (char *)vtable + 8;
    ((KIPlayer *)(this))->setVisible(visible);

    if (visible) {
        Array<BoundingVolume *> *volumes = new Array<BoundingVolume *>();
        this->boundingVolumes = volumes;
        volumes->resize(1);

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

        ((Player *)(this->player))->setRadius(radius);

        BoundingSphere *sphere = (BoundingSphere *)::operator new(0x48);
        new (sphere) BoundingSphere(x, y, z, 0.0f, 0.0f, 0.0f, (float)radius);
        (*this->boundingVolumes)[0] = (BoundingVolume *)sphere;
    }

    this->transformHandle = 0xffffffffU;
    this->activated = 0;
    ((AEGeometry *)(this->geometry))->setRotation(0.0f, 3.1415927f, 0.0f);
}

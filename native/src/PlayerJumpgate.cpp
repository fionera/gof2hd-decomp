#include "gof2/PlayerJumpgate.h"

// Minimal local layouts for engine types accessed through opaque handles in this
// translation unit. The full AEGeometry / Transform definitions live in their own
// headers; here we only need the few fields/methods this class touches.
struct AEGeometry {
    uint16_t field_0x8;                 // +0x8
    int32_t  field_0xc;                 // +0xc
    uint32_t field_0x14;                // +0x14
    void setPosition();
};

struct Transform {
    int64_t field_0x110;                // +0x110 (current animation time)
    bool    field_0xed;                 // +0xed  (animation running flag)
    void Update(bool active, int64_t delta, bool enabled);
};

extern "C" void PlayerStaticFar_dtor(PlayerStaticFar *self);
extern "C" void *PlayerJumpgate_delete_tail();
extern "C" Transform *PaintCanvas_TransformGetTransform(void *canvas, uint32_t handle);
extern "C" void Transform_SetAnimationState(Transform *transform, int state, int loop);
extern "C" void PaintCanvas_TransformRemoveChild(void *canvas, uint32_t parent, uint32_t child);
extern "C" void AEGeometry_addChild(AEGeometry *geometry, uint32_t handle);
extern "C" void KIPlayer_setVisible(PlayerJumpgate *self, bool visible);
extern "C" void *operator_new(uint32_t size);
extern "C" void Array_BoundingVolumePtr_ctor(Array<BoundingVolume *> *self);
extern "C" int Status_inAlienOrbit(void *status);
extern "C" void *Status_getSystem(void *status);
extern "C" int SolarSystem_getRace(void *system);
extern "C" void Player_setRadius(void *player, int32_t radius);
extern "C" void AEGeometry_setRotation(AEGeometry *geometry, float x, float y, float z);

// ---- _PlayerJumpgate_a5100.cpp ----
extern "C" void *_ZN14PlayerJumpgateD2Ev(PlayerJumpgate *self)
{
    PlayerStaticFar_dtor((PlayerStaticFar *)self);
    return PlayerJumpgate_delete_tail();
}

// ---- timeToJump_a5164.cpp ----
__attribute__((visibility("hidden"))) extern void *volatile PaintCanvas_global;

bool PlayerJumpgate::timeToJump()
{
    Transform *transform = PaintCanvas_TransformGetTransform(*(void **)PaintCanvas_global,
                                                             this->field_0x144);
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
        void **canvasOwner = (void **)PaintCanvas_global;
        Transform *transform = PaintCanvas_TransformGetTransform(*canvasOwner, handle);
        Transform_SetAnimationState(transform, 1, 0);

        AEGeometry *geometry = this->field_0x8;
        PaintCanvas_TransformRemoveChild(*canvasOwner, geometry->field_0xc,
                                         geometry->field_0x14);
        AEGeometry_addChild(this->field_0x8, this->field_0x144);
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

    Transform *transform = PaintCanvas_TransformGetTransform(*(void **)PaintCanvas_global,
                                                             this->field_0x144);
    return transform->field_0xed == 0;
}

// ---- update_a51f0.cpp ----
void PlayerJumpgate::update(int delta)
{
    if (this->field_0xf5 != 0) {
        AEGeometry *geometry = this->field_0x8;
        Transform *transform = PaintCanvas_TransformGetTransform(*(void **)PaintCanvas_global,
                                                                 geometry->field_0xc);
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
    this->field_0x8->setPosition();
}

// ---- PlayerJumpgate_a4fa4.cpp ----
extern "C" void PlayerStaticFar_ctor(PlayerStaticFar *self, int playerId, AEGeometry *geometry,
                                      float x, float y, float z);
extern "C" void ArraySetLength_BoundingVolumePtr(uint32_t length,
                                                  Array<BoundingVolume *> *self);
extern "C" void BoundingSphere_ctor(BoundingSphere *self, float x, float y, float z,
                                     float a, float b, float c, float radius);

__attribute__((visibility("hidden"))) extern void *volatile g_PlayerJumpgate_vtable;
__attribute__((visibility("hidden"))) extern void *volatile g_Status;

PlayerJumpgate::PlayerJumpgate(int playerId, AEGeometry *geometry, float x, float y, float z,
                               bool visible)
{
    PlayerStaticFar_ctor((PlayerStaticFar *)this, playerId, geometry, x, y, z);

    void *vtable = g_PlayerJumpgate_vtable;
    this->field_0x0 = (char *)vtable + 8;
    KIPlayer_setVisible(this, visible);

    if (visible) {
        Array<BoundingVolume *> *volumes =
            (Array<BoundingVolume *> *)operator_new(0xc);
        Array_BoundingVolumePtr_ctor(volumes);
        this->field_0x130 = volumes;
        ArraySetLength_BoundingVolumePtr(1, volumes);

        void **statusOwner = (void **)g_Status;
        int32_t radius;
        if (Status_inAlienOrbit(*statusOwner) != 0) {
            radius = 0x1d4c;
        } else {
            void *system = Status_getSystem(*statusOwner);
            radius = 0x1d4c;
            if (SolarSystem_getRace(system) == 1) {
                radius = 0x2bf2;
            }
        }

        Player_setRadius(this->field_0x4, radius);

        BoundingSphere *sphere = (BoundingSphere *)operator_new(0x48);
        BoundingSphere_ctor(sphere, x, y, z, 0.0f, 0.0f, 0.0f, (float)radius);
        (*this->field_0x130)[0] = (BoundingVolume *)sphere;
    }

    this->field_0x144 = 0xffffffffU;
    this->field_0x140 = 0;
    AEGeometry_setRotation(this->field_0x8, 0.0f, 3.1415927f, 0.0f);
}

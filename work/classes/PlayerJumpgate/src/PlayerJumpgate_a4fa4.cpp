#include "class.h"

extern "C" void PlayerStaticFar_ctor(PlayerStaticFar *self, int playerId, AEGeometry *geometry,
                                      float x, float y, float z);
extern "C" void KIPlayer_setVisible(PlayerJumpgate *self, bool visible);
extern "C" void *operator_new(uint32_t size);
extern "C" void Array_BoundingVolumePtr_ctor(Array<BoundingVolume *> *self);
extern "C" void ArraySetLength_BoundingVolumePtr(uint32_t length,
                                                  Array<BoundingVolume *> *self);
extern "C" int Status_inAlienOrbit(void *status);
extern "C" void *Status_getSystem(void *status);
extern "C" int SolarSystem_getRace(void *system);
extern "C" void Player_setRadius(void *player, int32_t radius);
extern "C" void BoundingSphere_ctor(BoundingSphere *self, float x, float y, float z,
                                     float a, float b, float c, float radius);
extern "C" void AEGeometry_setRotation(AEGeometry *geometry, float x, float y, float z);

__attribute__((visibility("hidden"))) extern void *volatile g_PlayerJumpgate_vtable;
__attribute__((visibility("hidden"))) extern void *volatile g_Status;

PlayerJumpgate::PlayerJumpgate(int playerId, AEGeometry *geometry, float x, float y, float z,
                               bool visible)
{
    PlayerStaticFar_ctor((PlayerStaticFar *)this, playerId, geometry, x, y, z);

    void *vtable = g_PlayerJumpgate_vtable;
    F<void *>(this, 0x0) = (char *)vtable + 8;
    KIPlayer_setVisible(this, visible);

    if (visible) {
        Array<BoundingVolume *> *volumes =
            (Array<BoundingVolume *> *)operator_new(0xc);
        Array_BoundingVolumePtr_ctor(volumes);
        F<Array<BoundingVolume *> *>(this, 0x130) = volumes;
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

        Player_setRadius(F<void *>(this, 0x4), radius);

        BoundingSphere *sphere = (BoundingSphere *)operator_new(0x48);
        BoundingSphere_ctor(sphere, x, y, z, 0.0f, 0.0f, 0.0f, (float)radius);
        F<Array<BoundingVolume *> *>(this, 0x130)->data[0] = (BoundingVolume *)sphere;
    }

    VF<uint32_t>(this, 0x144) = 0xffffffffU;
    VF<uint8_t>(this, 0x140) = 0;
    AEGeometry_setRotation(F<AEGeometry *>(this, 0x8), 0.0f, 3.1415927f, 0.0f);
}

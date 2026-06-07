#include "class.h"

using AbyssEngine::AEMath::Vector;

extern void *gPlayerTurretVtable_ctor __attribute__((visibility("hidden")));
extern uint32_t *gPlayerTurretCanvas __attribute__((visibility("hidden")));

extern "C" void KIPlayer_ctor(PlayerTurret *self, int mesh, int team, Player *player,
                              AEGeometry *geometry, float x, float y, float z, bool flag);
extern "C" void AEGeometry_ctorMesh(AEGeometry *self, uint16_t mesh, void *canvas, bool flag);
extern "C" void AEGeometry_ctor(AEGeometry *self, void *canvas);
extern "C" void AEGeometry_setRotationOrder(AEGeometry *self, int order);
extern "C" void AEGeometry_setPosition(AEGeometry *self, const Vector *position);
extern "C" void AEGeometry_rotate(AEGeometry *self, float x, float y, float z);
extern "C" void AEGeometry_addChild(AEGeometry *self, uint32_t child);
extern "C" void *AEGeometry_dtor(AEGeometry *self);
extern "C" void AEGeometry_setScaling(AEGeometry *self, float x, float y, float z);
extern "C" uint32_t PaintCanvas_TransformGetTransform(uint32_t canvas, uint32_t transform);
extern "C" void Explosion_ctor(Explosion *self, int kind);
extern "C" void Explosion_addFireStreaks(Explosion *self);
extern "C" void PlayerTurret_operator_delete(void *ptr);

struct PlayerTurret {
    PlayerTurret(int mesh, Player *player, AEGeometry *geometry, float x, float y, float z);
};

typedef void (*SetPositionFn)(PlayerTurret *self, const Vector *position);

PlayerTurret::PlayerTurret(int mesh, Player *player, AEGeometry *geometry, float x, float y, float z)
{
    char vectorBytes[12];
    KIPlayer_ctor(this, mesh, -1, player, geometry, x, y, z, false);

    P(this, 0x0) = (char *)gPlayerTurretVtable_ctor + 8;
    UC(this, 0x25) = 1;
    UC(this, 0x3e) = 1;
    P(this, 0x140) = 0;
    P(this, 0x144) = 0;
    I(this, 0x128) = 0;
    P(this, 0x154) = 0;
    P(this, 0x158) = 0;
    P(this, 0x15c) = 0;
    I(this, 0x164) = 50000;

    AEGeometry *base = (AEGeometry *)operator new(0xc0);
    uint32_t *canvasHolder = gPlayerTurretCanvas;
    AEGeometry_ctorMesh(base, (uint16_t)mesh, (void *)*canvasHolder, false);
    P(this, 0x140) = base;

    if (mesh == 0x381b) {
        AEGeometry *turret = (AEGeometry *)operator new(0xc0);
        AEGeometry_ctorMesh(turret, 0x381c, (void *)*canvasHolder, false);
        P(this, 0x144) = turret;
        AEGeometry_setRotationOrder(turret, 2);
        Vector *v = (Vector *)vectorBytes;
        v->x = 0.0f;
        v->y = 0.0f;
        v->z = 0.0f;
        AEGeometry_setPosition(turret, v);
    } else if (mesh == 0x1a76) {
        AEGeometry *turret = (AEGeometry *)operator new(0xc0);
        AEGeometry_ctorMesh(turret, 0x1a77, (void *)*canvasHolder, false);
        P(this, 0x144) = turret;
        AEGeometry_setRotationOrder(turret, 2);
        Vector *v = (Vector *)vectorBytes;
        v->x = 0.0f;
        v->y = 0.0f;
        v->z = 0.0f;
        AEGeometry_setPosition(turret, v);
    } else if (mesh == 0x1a74) {
        AEGeometry *turret = (AEGeometry *)operator new(0xc0);
        AEGeometry_ctorMesh(turret, 0x1a75, (void *)*canvasHolder, false);
        P(this, 0x144) = turret;
        AEGeometry_setRotationOrder(turret, 2);
        Vector *v = (Vector *)vectorBytes;
        v->x = 0.0f;
        v->y = 0.0f;
        v->z = 0.0f;
        AEGeometry_setPosition(turret, v);
    }

    AEGeometry *helper = (AEGeometry *)operator new(0xc0);
    AEGeometry_ctor(helper, (void *)*canvasHolder);
    P(this, 0x148) = helper;
    uint32_t transform = PaintCanvas_TransformGetTransform(*canvasHolder, U(helper, 0xc));
    I((void *)transform, 0xe0) = 0;

    Vector *initial = (Vector *)vectorBytes;
    initial->x = x;
    initial->y = y;
    initial->z = z;
    SetPositionFn setPosition = *(SetPositionFn *)((char *)P(this, 0x0) + 0x44);
    setPosition(this, initial);

    if (mesh == 0x381b) {
        AEGeometry_rotate(TP<AEGeometry>(this, 0x140), 0.0f, 0.0f, 0.0f);
        AEGeometry_rotate(TP<AEGeometry>(this, 0x144), 0.0f, 0.0f, 0.0f);
        AEGeometry_addChild(TP<AEGeometry>(this, 0x148), U(P(this, 0x144), 0xc));
    } else if ((mesh | 2U) == 0x1a76) {
        AEGeometry_addChild(TP<AEGeometry>(this, 0x148), U(P(this, 0x144), 0xc));
    } else if ((uint32_t)(mesh - 0x49c0) < 3) {
        UC(this, 0x3f) = 1;
        AEGeometry *child = (AEGeometry *)operator new(0xc0);
        uint16_t childMesh = 0x49c8;
        if (mesh == 0x49c1) {
            childMesh = 0x49c7;
        }
        if (mesh == 0x49c0) {
            childMesh = 0x49c6;
        }
        AEGeometry_ctorMesh(child, childMesh, (void *)*canvasHolder, false);
        AEGeometry_addChild(geometry, U(child, 0xc));
        PlayerTurret_operator_delete(AEGeometry_dtor(child));
        AEGeometry_setScaling(geometry, 0.5f, 0.5f, 0.5f);
    }

    AEGeometry_addChild(TP<AEGeometry>(this, 0x148), U(P(this, 0x140), 0xc));
    if (UC(this, 0x3f) == 0) {
        AEGeometry_addChild(geometry, U(P(this, 0x148), 0xc));
    }

    Explosion *explosion = (Explosion *)operator new(0x68);
    Explosion_ctor(explosion, 0);
    P(this, 0x13c) = explosion;
    Explosion_addFireStreaks(explosion);

    P(this, 0x14c) = 0;
    P(this, 0x150) = 0;
    I(this, 0x12c) = 0;
    I(this, 0x130) = 0;
    I(this, 0x134) = 0;
    I(this, 0x138) = 0;
}

#include "class.h"

struct BombGun {
    BombGun(Gun *gun, uint32_t mesh, int param3, int type, bool playerControlled, Level *level);
};

extern "C" __attribute__((visibility("hidden"))) void *BombGun_vtable;
extern "C" __attribute__((visibility("hidden"))) void **BombGun_player_canvas;
extern "C" __attribute__((visibility("hidden"))) void **BombGun_canvas;
extern "C" __attribute__((visibility("hidden"))) void **BombGun_final_canvas;

extern "C" void RocketGun_ctor(BombGun *self, int param3, Gun *gun, uint32_t mesh,
                                int zero0, int zero1, int type, bool flag, Level *level);
extern "C" void *operator_new(uint32_t size);
extern "C" void Explosion_ctor(Explosion *self, int type);
extern "C" void Explosion_setWeaponIndex(Explosion *self, int index);
extern "C" void Explosion_setScaling(Explosion *self, float scaling);
extern "C" void PaintCanvas_TransformCreate(void *canvas, uint32_t *out);
extern "C" void PaintCanvas_TransformAddMesh(void *canvas, uint32_t transform, uint16_t mesh,
                                              int flags);
extern "C" Transform *PaintCanvas_TransformGetTransform(void *canvas, uint32_t transform);
extern "C" void Transform_SetAnimationState(Transform *self, int state, void *arg);
extern "C" void AEGeometry_ctor(AEGeometry *self, uint16_t mesh, void *canvas, bool flag);
extern "C" void PaintCanvas_TransformAddChild(void *canvas, uint32_t parent, uint32_t child);
extern "C" void PaintCanvas_TransformRemoveMesh(void *canvas, uint32_t transform, uint16_t mesh);
extern "C" void *AEGeometry_dtor(AEGeometry *self);
extern "C" void operator_delete(void *p);
extern "C" void *Vector_assign(void *dst, void *src);
extern "C" void AEGeometry_ctor_canvas(AEGeometry *self, void *canvas);

static const float kBombScale = 50000.0f;
static const float kOffsetY = 450.0f;
static const float kOffsetZ = -1400.0f;
static const float kRocketOffsetZ = 1700.0f;

extern "C" BombGun *_ZN7BombGunC1EP3GunjiibP5Level(
    BombGun *self, Gun *gun, uint32_t mesh, int param3, int type, int playerControlled,
    Level *level)
{
    char local[12];

    RocketGun_ctor(self, param3, gun, mesh, 0, 0, type, false, level);

    *(v4i *)((char *)self + 0x110) = (v4i){0, 0, 0, 0};
    *(void * volatile *)self = (char *)BombGun_vtable + 8;
    *(volatile uint64_t *)((char *)self + 0xf8) = 0;
    *(volatile int *)((char *)self + 0x100) = 0;
    *(volatile uint64_t *)((char *)self + 0x120) = 0;

    Explosion *explosion = (Explosion *)operator_new(0x68);
    int explosionType;
    if (type == 6) {
        explosionType = 7;
    } else if (type == 0x2a) {
        explosionType = 0xb;
    } else {
        explosionType = 0;
        if (F<int>(gun, 0x58) == 0xe8)
            explosionType = 0xd;
    }
    Explosion_ctor(explosion, explosionType);
    F<Explosion *>(self, 0xf0) = explosion;
    Explosion_setWeaponIndex(explosion, F<int>(gun, 0x58));

    int playerFlag = *(volatile int *)&playerControlled;
    *(volatile uint8_t *)((char *)self + 0x24) = playerFlag;
    bool scaledBomb = type == 0x2a;
    *(volatile uint8_t *)((char *)self + 0x104) = 1;
    *(volatile int *)((char *)self + 0x128) = type;
    *(volatile int *)((char *)self + 0xf4) = -1;
    if (scaledBomb)
        Explosion_setScaling(F<Explosion *>(self, 0xf0), kBombScale);

    AEGeometry *geometry;
    if (playerFlag) {
        void **canvasHolder = BombGun_player_canvas;
        uint32_t *createdTransform = &F<uint32_t>(self, 0x14);
        PaintCanvas_TransformCreate(*canvasHolder, createdTransform);
        PaintCanvas_TransformAddMesh(*canvasHolder, *createdTransform,
                                     F<uint16_t>(self, 0x28), 0);
        Transform *transform =
            PaintCanvas_TransformGetTransform(*canvasHolder, *createdTransform);
        Transform_SetAnimationState(transform, 1, 0);

        geometry = (AEGeometry *)operator_new(0xc0);
        AEGeometry_ctor(geometry, 0x37d6, *canvasHolder, false);
        PaintCanvas_TransformAddChild(*canvasHolder, *createdTransform,
                                      F<uint32_t>(geometry, 0xc));
        PaintCanvas_TransformRemoveMesh(*canvasHolder, F<uint32_t>(self, 0x10),
                                        F<uint16_t>(self, 0x28));
        PaintCanvas_TransformAddChild(*canvasHolder, F<uint32_t>(self, 0x10),
                                      F<uint32_t>(self, 0x14));
    } else {
        int weapon = F<int>(gun, 0x58);
        bool normal = weapon != 0xe8;
        if (normal)
            weapon = F<int>(gun, 0x5c);
        if (!normal || weapon == 0x22)
            goto after_geometry;

        geometry = (AEGeometry *)operator_new(0xc0);
        uint16_t geomMesh = 0x395d;
        void **canvasHolder = BombGun_canvas;
        if (mesh == 0x395a)
            geomMesh = 0x395b;
        if (mesh == 0x3958)
            geomMesh = 0x3959;

        AEGeometry_ctor(geometry, geomMesh, *canvasHolder, false);
        F<uint32_t>(self, 0xf4) = F<uint32_t>(geometry, 0xc);
        PaintCanvas_TransformAddChild(*canvasHolder, F<uint32_t>(self, 0x10),
                                      F<uint32_t>(geometry, 0xc));

        Transform *transform;
        if (mesh == 0x395c) {
            transform = PaintCanvas_TransformGetTransform(*canvasHolder, F<uint32_t>(self, 0xf4));
            Transform_SetAnimationState(transform, 2, 0);
        } else {
            transform = PaintCanvas_TransformGetTransform(*canvasHolder, F<uint32_t>(self, 0x10));
            Transform_SetAnimationState(transform, 1, 0);
        }
    }

    operator_delete(AEGeometry_dtor(geometry));

after_geometry:
    *(volatile float *)(local + 0x4) = kOffsetY;
    *(volatile float *)(local + 0x0) = 0.0f;
    *(volatile float *)(local + 0x8) = kOffsetZ;
    Vector_assign((char *)self + 0x110, local);

    *(volatile float *)(local + 0x0) = 0.0f;
    *(volatile float *)(local + 0x4) = 0.0f;
    *(volatile float *)(local + 0x8) = kRocketOffsetZ;
    Vector_assign((char *)self + 0x11c, local);

    F<PlayerEgo *>(self, 0xec) = 0;
    AEGeometry *finalGeometry = (AEGeometry *)operator_new(0xc0);
    void **finalCanvas = BombGun_final_canvas;
    AEGeometry_ctor_canvas(finalGeometry, *finalCanvas);
    F<AEGeometry *>(self, 0xe8) = finalGeometry;
    return self;
}

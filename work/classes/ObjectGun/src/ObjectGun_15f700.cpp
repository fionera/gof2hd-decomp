#include "class.h"

__attribute__((visibility("hidden"))) extern "C" void *ObjectGun_vtable;
__attribute__((visibility("hidden"))) extern "C" void *g_PaintCanvas;
__attribute__((visibility("hidden"))) extern "C" void *g_ObjectGunScaleFlag;
__attribute__((visibility("hidden"))) extern "C" MeshId g_ObjectGunGeometryIds[];
__attribute__((visibility("hidden"))) extern "C" int g_ObjectGunPlayerGunIds[];

extern "C" void Matrix_ctor(Matrix *self);
extern "C" void TransformCreate(void *canvas, uint32_t *transform);
extern "C" void TransformAddMesh(void *canvas, uint32_t transform, uint16_t mesh, int flags);
extern "C" void *operator_new(uint32_t size);
extern "C" void *operator_new_array(uint32_t size);
extern "C" void Array_Explosion_ctor(Array *self);
extern "C" void ArraySetLength_Explosion(uint32_t length, Array *self);
extern "C" void Explosion_ctor(Explosion *self, int type);
extern "C" void Explosion_setWeaponIndex(Explosion *self, int weapon);
extern "C" int Gun_isPlayerGun(Gun *self);
extern "C" void AEGeometry_ctor(AEGeometry *self, uint16_t mesh, void *canvas, bool flag);

static const Vec4 kZeroVec = {0.0f, 0.0f, 0.0f, 0.0f};
static const Vec4 kDefaultScale = {1.0f, 1.0f, 1.0f, 0.0f};

ObjectGun::ObjectGun(int, Gun *gun, int mesh, uint32_t, Level *level)
{
    ObjectGun *self = this;
    F<Vec4>(self, 0x60) = kZeroVec;
    F<Vec4>(self, 0x50) = kZeroVec;
    F<uint32_t>(self, 0x70) = 0;
    F<void *>(self, 0x0) = (char *)ObjectGun_vtable + 8;
    Matrix_ctor((Matrix *)((char *)self + 0x74));

    void **canvas = (void **)g_PaintCanvas;
    F<uint8_t>(self, 0x24) = 0;
    F<Array *>(self, 0x2c) = 0;
    F<void *>(self, 0x30) = 0;
    F<int>(self, 0x4) = -1;
    F<Gun *>(self, 0x8) = gun;
    F<Level *>(self, 0xc) = level;
    F<int>(self, 0x14) = -1;
    TransformCreate(*canvas, &F<uint32_t>(self, 0x10));
    F<int>(self, 0x28) = mesh;
    TransformAddMesh(*canvas, F<uint32_t>(self, 0x10), (uint16_t)mesh, 0);
    F<uint32_t>(self, 0x20) = 0;
    F<Vec4>(self, 0x3c) = kDefaultScale;

    uint32_t type = F<uint32_t>(gun, 0x5c);
    uint32_t visible = 1;
    if (type > 8)
        goto visible_zero;
    {
        uint32_t mask = visible << type;
        if ((mask & 0x10aU) == 0)
            visible = 0;
    }
    goto visible_done;
visible_zero:
    visible = 0;
visible_done:
    F<uint8_t>(self, 0x4c) = (uint8_t)visible;

    if (*(uint8_t *)g_ObjectGunScaleFlag == 0)
        goto check_special_type;
    if (type == 1)
        goto scale_to_small;
    if (type == 3)
        goto scale_to_small;
    if (type == 0x19)
        goto make_explosions;
    if (type == 0xb)
        goto scale_to_medium;
    if (type == 8)
        goto scale_to_small;
    goto after_special_type;

scale_to_small:
    F<float>(self, 0x3c) = 0.6f;
    F<float>(self, 0x40) = 0.6f;
    F<float>(self, 0x44) = 0.6f;

check_special_type:
    if (type == 0x19)
        goto make_explosions;
    if (type == 0xb)
        goto scale_to_medium;
    goto after_special_type;

scale_to_medium:
    F<float>(self, 0x3c) = 0.7f;
    F<float>(self, 0x40) = 0.7f;
    F<float>(self, 0x44) = 0.7f;
    goto after_special_type;

make_explosions:
    {
        Array *explosions = (Array *)operator_new(0xc);
        Array_Explosion_ctor(explosions);
        F<Array *>(self, 0x2c) = explosions;
        ArraySetLength_Explosion(F<uint32_t>(gun, 0x8), explosions);
        explosions = F<Array *>(self, 0x2c);
        F<void *>(self, 0x30) = operator_new_array(F<uint32_t>(explosions, 0x0));

        for (uint32_t i = 0; i < F<uint32_t>(explosions, 0x0); ++i) {
            Explosion *explosion = (Explosion *)operator_new(0x68);
            int explosionType = 10;
            int weapon = F<int>(gun, 0x58);
            if (weapon == 0xb1)
                explosionType = 9;
            if (weapon == 0xb0)
                explosionType = 8;
            Explosion_ctor(explosion, explosionType);
            F<Explosion **>(F<Array *>(self, 0x2c), 0x4)[i] = explosion;
            Explosion_setWeaponIndex(F<Explosion **>(F<Array *>(self, 0x2c), 0x4)[i],
                                     F<int>(gun, 0x58));
            F<uint8_t *>(self, 0x30)[i] = 1;
            explosions = F<Array *>(self, 0x2c);
        }
    }

after_special_type:

    AEGeometry *geometry;
    if (F<uint8_t>(gun, 0xa8) == 0) {
        if (Gun_isPlayerGun(gun) == 0 ||
            g_ObjectGunPlayerGunIds[F<int>(gun, 0x58)] < 0) {
            geometry = 0;
            F<uint8_t>(self, 0x1c) = 0;
            goto done;
        }
        int createGeometry = F<int>(gun, 0x5c) - 0xb;
        if (createGeometry != 0)
            createGeometry = 1;
        F<uint8_t>(self, 0x1c) = createGeometry;
        if (createGeometry == 0) {
            geometry = 0;
            goto done;
        }
    } else {
        F<uint8_t>(self, 0x1c) = 1;
    }

    geometry = (AEGeometry *)operator_new(0xc0);
    AEGeometry_ctor(geometry, g_ObjectGunGeometryIds[F<int>(gun, 0x58)].id, *canvas, false);

done:
    F<uint8_t>(self, 0x1d) = 0;
    F<AEGeometry *>(self, 0x18) = geometry;
    F<uint32_t>(self, 0x34) = 0;
}

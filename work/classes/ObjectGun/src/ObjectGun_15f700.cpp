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
    self->f_60 = kZeroVec;
    self->f_50 = kZeroVec;
    self->f_70 = 0;
    self->f_0 = (char *)ObjectGun_vtable + 8;
    Matrix_ctor((Matrix *)((char *)self + 0x74));

    void **canvas = (void **)g_PaintCanvas;
    self->f_24 = 0;
    self->f_2c = 0;
    self->f_30 = 0;
    self->f_4 = -1;
    self->f_8 = gun;
    self->f_c = level;
    self->f_14 = -1;
    TransformCreate(*canvas, &self->f_10);
    self->f_28 = mesh;
    TransformAddMesh(*canvas, self->f_10, (uint16_t)mesh, 0);
    self->f_20 = 0;
    self->f_3c = kDefaultScale;

    uint32_t type = gun->f_5c;
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
    self->f_4c = (uint8_t)visible;

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
    self->f_3c = 0.6f;
    self->f_40 = 0.6f;
    self->f_44 = 0.6f;

check_special_type:
    if (type == 0x19)
        goto make_explosions;
    if (type == 0xb)
        goto scale_to_medium;
    goto after_special_type;

scale_to_medium:
    self->f_3c = 0.7f;
    self->f_40 = 0.7f;
    self->f_44 = 0.7f;
    goto after_special_type;

make_explosions:
    {
        Array *explosions = (Array *)operator_new(0xc);
        Array_Explosion_ctor(explosions);
        self->f_2c = explosions;
        ArraySetLength_Explosion(gun->f_8, explosions);
        explosions = self->f_2c;
        self->f_30 = operator_new_array(explosions->f_0);

        for (uint32_t i = 0; i < explosions->f_0; ++i) {
            Explosion *explosion = (Explosion *)operator_new(0x68);
            int explosionType = 10;
            int weapon = gun->f_58;
            if (weapon == 0xb1)
                explosionType = 9;
            if (weapon == 0xb0)
                explosionType = 8;
            Explosion_ctor(explosion, explosionType);
            F<Explosion **>(self->f_2c, 0x4)[i] = explosion;
            Explosion_setWeaponIndex(F<Explosion **>(self->f_2c, 0x4)[i],
                                     gun->f_58);
            self->f_30[i] = 1;
            explosions = self->f_2c;
        }
    }

after_special_type:

    AEGeometry *geometry;
    if (gun->f_a8 == 0) {
        if (Gun_isPlayerGun(gun) == 0 ||
            g_ObjectGunPlayerGunIds[gun->f_58] < 0) {
            geometry = 0;
            self->f_1c = 0;
            goto done;
        }
        int createGeometry = gun->f_5c - 0xb;
        if (createGeometry != 0)
            createGeometry = 1;
        self->f_1c = createGeometry;
        if (createGeometry == 0) {
            geometry = 0;
            goto done;
        }
    } else {
        self->f_1c = 1;
    }

    geometry = (AEGeometry *)operator_new(0xc0);
    AEGeometry_ctor(geometry, g_ObjectGunGeometryIds[gun->f_58].id, *canvas, false);

done:
    F<uint8_t>(self, 0x1d) = 0;
    self->f_18 = geometry;
    self->f_34 = 0;
}

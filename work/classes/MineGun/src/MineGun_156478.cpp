#include "class.h"

extern "C" __attribute__((visibility("hidden"))) void *MineGun_vtable;
extern "C" __attribute__((visibility("hidden"))) void **MineGun_canvas_holder;
extern "C" void ObjectGun_ctor(MineGun *self, int param_3, Gun *gun, int param_2,
                                int zero, Level *level);
extern "C" void *operator_new(uint32_t size);
extern "C" void *operator_new_array(uint32_t size);
extern "C" void Array_Explosion_ctor(Array<Explosion *> *self);
extern "C" void ArraySetLength_Explosion(uint32_t length, Array<Explosion *> *self);
extern "C" void Explosion_ctor(Explosion *self, int kind);
extern "C" void Explosion_setWeaponIndex(Explosion *self, int index);
extern "C" void AEGeometry_ctor(AEGeometry *self, uint16_t mesh, void *canvas, bool flag);
extern "C" void PaintCanvas_TransformAddChild(void *canvas, uint32_t parent, uint32_t child);
extern "C" uint32_t PaintCanvas_TransformGetTransform(void *canvas, uint32_t transform);
extern "C" void Transform_SetAnimationState(uint32_t transform, int state, int frame);

extern "C" MineGun *_ZN7MineGunC1EP3GuniiiP5Level(MineGun *self, Gun *gun, int param_2,
                                                   int param_3, int, Level *level)
{
    int zero = 0;
    ObjectGun_ctor(self, param_3, gun, param_2, zero, level);

    I(self, 0xc0) = zero;
    I(self, 0xc4) = zero;
    I(self, 0xc8) = zero;
    P(self, 0x0) = (char *)MineGun_vtable + 8;

    Array<Explosion *> *explosions = (Array<Explosion *> *)operator_new(0xc);
    Array_Explosion_ctor(explosions);
    P(self, 0xb4) = explosions;
    uint32_t length = U(gun, 0x8);
    ArraySetLength_Explosion(length, explosions);

    explosions = (Array<Explosion *> *)P(self, 0xb4);
    P(self, 0xb8) = operator_new_array(explosions->length);
    uint32_t i = 0;
    for (; i < explosions->length; ++i) {
        Explosion *explosion = (Explosion *)operator_new(0x68);
        int kind = 0;
        if (I(gun, 0x60) == 0) {
            kind = 7;
        }
        Explosion_ctor(explosion, kind);
        ((Array<Explosion *> *)P(self, 0xb4))->data[i] = explosion;
        Explosion_setWeaponIndex(((Array<Explosion *> *)P(self, 0xb4))->data[i], I(gun, 0x58));
        F<uint8_t>(P(self, 0xb8), i) = 1;
        explosions = (Array<Explosion *> *)P(self, 0xb4);
    }

    AEGeometry *geometry = (AEGeometry *)operator_new(0xc0);
    void **holder = MineGun_canvas_holder;
    void *canvas = *holder;
    uint16_t mesh = (uint16_t)(param_2 + 1);
    AEGeometry_ctor(geometry, mesh, canvas, false);
    uint32_t parent = U(self, 0x10);
    P(self, 0xbc) = geometry;
    void *addCanvas = *holder;
    uint32_t child = U(geometry, 0xc);
    PaintCanvas_TransformAddChild(addCanvas, parent, child);
    AEGeometry *stored = (AEGeometry *)P(self, 0xbc);
    uint32_t transformId = U(stored, 0xc);
    void *getCanvas = *holder;
    uint32_t transform = PaintCanvas_TransformGetTransform(getCanvas, transformId);
    Transform_SetAnimationState(transform, 2, 0);
    return self;
}

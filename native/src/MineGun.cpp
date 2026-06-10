#include "gof2/MineGun.h"


extern "C" void ObjectGun_render(MineGun *self);
extern "C" void Explosion_render(Explosion *self);
extern "C" __attribute__((visibility("hidden"))) void *MineGun_vtable;
extern "C" void ArrayReleaseClasses_Explosion(Array<Explosion *> *array);
extern "C" void *Array_Explosion_dtor(Array<Explosion *> *array);
extern "C" void operator_delete(void *ptr);
extern "C" void operator_delete_array(void *ptr);
extern "C" void *AEGeometry_dtor(AEGeometry *self);
extern "C" void *ObjectGun_dtor(MineGun *self);
extern "C" void *_ZN7MineGunD1Ev(MineGun *self);
extern "C" __attribute__((visibility("hidden"))) void **MineGun_canvas_holder;
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
extern "C" void ObjectGun_update(MineGun *self, int delta);
extern "C" void Transform_Update(uint32_t transform, long long delta, int zero);
extern "C" void PlayerEgo_getPosition(Vector *out, PlayerEgo *self);
extern "C" void AEMath_operator_sub(Vector *out, const Vector *a, const Vector *b);
extern "C" float VectorLength(const Vector *self);
extern "C" TargetFollowCamera *PlayerEgo_getTargetFollowCamera(PlayerEgo *self);
extern "C" void Explosion_start(Explosion *self, const Vector *position, const Vector *zero);
extern "C" void Explosion_update(Explosion *self, int delta, TargetFollowCamera *camera);
extern "C" int Explosion_isPlaying(Explosion *self);
extern "C" void Explosion_reset(Explosion *self);

// ---- render_1566bc.cpp ----
void MineGun::render()
{
    ObjectGun_render(this);
    for (uint32_t i = 0; i < U(P(this, 0x8), 0x8); ++i) {
        if (F<uint8_t>(P(P(this, 0x8), 0x40), i) != 0) {
            Array<Explosion *> *explosions = (Array<Explosion *> *)P(this, 0xb4);
            Explosion_render(explosions->data()[i]);
        }
    }
}

// ---- _MineGun_1565e4.cpp ----
extern "C" void *_ZN7MineGunD1Ev(MineGun *self)
{
    P(self, 0x0) = (char *)MineGun_vtable + 8;

    Array<Explosion *> *explosions = (Array<Explosion *> *)P(self, 0xb4);
    if (explosions != 0) {
        ArrayReleaseClasses_Explosion(explosions);
        if (P(self, 0xb4) != 0) {
            operator_delete(Array_Explosion_dtor((Array<Explosion *> *)P(self, 0xb4)));
        }
        P(self, 0xb4) = 0;
    }

    operator_delete_array(P(self, 0xb8));
    AEGeometry *geometry = (AEGeometry *)P(self, 0xbc);
    P(self, 0xb8) = 0;
    if (geometry != 0) {
        operator_delete(AEGeometry_dtor(geometry));
    }
    P(self, 0xbc) = 0;
    return ObjectGun_dtor(self);
}

// ---- setPlayer_1566b6.cpp ----
void MineGun::setPlayer(PlayerEgo *player)
{
    P(this, 0xb0) = player;
}

// ---- _MineGun_1566a6.cpp ----
extern "C" void _ZN7MineGunD0Ev(MineGun *self)
{
    return operator_delete(_ZN7MineGunD1Ev(self));
}

// ---- MineGun_156478.cpp ----
extern "C" void ObjectGun_ctor(MineGun *self, int param_3, Gun *gun, int param_2,
                                int zero, Level *level);

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
    P(self, 0xb8) = operator_new_array(explosions->size());
    uint32_t i = 0;
    for (; i < explosions->size(); ++i) {
        Explosion *explosion = (Explosion *)operator_new(0x68);
        int kind = 0;
        if (I(gun, 0x60) == 0) {
            kind = 7;
        }
        Explosion_ctor(explosion, kind);
        ((Array<Explosion *> *)P(self, 0xb4))->data()[i] = explosion;
        Explosion_setWeaponIndex(((Array<Explosion *> *)P(self, 0xb4))->data()[i], I(gun, 0x58));
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

// ---- update_1566ec.cpp ----
extern "C" void TargetFollowCamera_setRumblePercentage(TargetFollowCamera *self, float pct,
                                                       int duration);

static inline Explosion *explosion_at(MineGun *self, uint32_t index)
{
    return ((Array<Explosion *> *)P(self, 0xb4))->data()[index];
}

void MineGun::update(int delta)
{
    char vectorBytes[24];

    ObjectGun_update(this, delta);
    if (UC(P(this, 0x8), 0x4c) != 0) {
        void **holder = MineGun_canvas_holder;
        uint32_t transform = PaintCanvas_TransformGetTransform(*holder, U(P(this, 0xbc), 0xc));
        Transform_Update(transform, (long long)delta, 0);
    }

    float one = 1.0f;
    float decay = -2000.0f;
    float range = 30000.0f;
    int positionOffset = 0;
    uint32_t i = 0;
    int zero = 0;
    for (; i < U(P(this, 0x8), 0x8); ++i) {
        void *gun = P(this, 0x8);
        if (F<uint8_t>(P(gun, 0x40), i) != 0) {
            if (F<uint8_t>(P(this, 0xb8), i) != 0) {
                I(this, 0xcc) = zero;
                char *positions = (char *)P(gun, 0x30);
                PlayerEgo_getPosition((Vector *)vectorBytes, (PlayerEgo *)P(this, 0xb0));
                Vector *minePosition = (Vector *)(positions + positionOffset);
                AEMath_operator_sub((Vector *)(vectorBytes + 12), minePosition,
                                    (Vector *)vectorBytes);
                float length = VectorLength((Vector *)(vectorBytes + 12));
                float clamped = range;
                if (length < range) {
                    clamped = length;
                }
                FL(this, 0xd0) = one - clamped / range;
                TargetFollowCamera *camera =
                    PlayerEgo_getTargetFollowCamera((PlayerEgo *)P(this, 0xb0));
                TargetFollowCamera_setRumblePercentage(camera, FL(this, 0xd0), 0x32);

                ((int *)(vectorBytes + 12))[0] = zero;
                ((int *)(vectorBytes + 12))[1] = zero;
                ((int *)(vectorBytes + 12))[2] = zero;
                Explosion_start(explosion_at(this, i),
                                (Vector *)((char *)P(P(this, 0x8), 0x30) + positionOffset),
                                (Vector *)(vectorBytes + 12));
                F<uint8_t>(P(this, 0xb8), i) = 0;
            }

            Explosion *explosion = explosion_at(this, i);
            Explosion_update(explosion, delta, (TargetFollowCamera *)0);
            int timer = I(this, 0xcc) + delta;
            if (timer > 1999) {
                timer = 2000;
            }
            I(this, 0xcc) = timer;
            TargetFollowCamera *camera =
                PlayerEgo_getTargetFollowCamera((PlayerEgo *)P(this, 0xb0));
            float pct = FL(this, 0xd0) * ((float)I(this, 0xcc) / decay + one);
            TargetFollowCamera_setRumblePercentage(camera, pct, 0x32);

            if (Explosion_isPlaying(explosion) == 0) {
                TargetFollowCamera *camera =
                    PlayerEgo_getTargetFollowCamera((PlayerEgo *)P(this, 0xb0));
                TargetFollowCamera_setRumblePercentage(camera, 0.0f, zero);
                gun = P(this, 0x8);
                I(this, 0xcc) = zero;
                F<uint8_t>(P(gun, 0x40), i) = zero;
                UC(gun, 0x88) = zero;
                F<uint8_t>(P(this, 0xb8), i) = 1;
                Explosion_reset(explosion_at(this, i));
            }
        }
        positionOffset += 0xc;
    }
}

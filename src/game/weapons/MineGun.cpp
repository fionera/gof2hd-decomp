#include "gof2/game/weapons/MineGun.h"
#include "gof2/game/ship/TargetFollowCamera.h"
#include "gof2/engine/render/AEGeometry.h"
#include "gof2/game/weapons/ObjectGun.h"
#include "gof2/engine/math/Transform.h"
#include "gof2/game/mission/Explosion.h"
#include "gof2/game/ship/PlayerEgo.h"
#include "gof2/game/core/PaintCanvasClass.h"

namespace AbyssEngine { namespace AEMath {
Vector operator-(const Vector &lhs, const Vector &rhs);
float VectorLength(const Vector &value);
} }

extern "C" __attribute__((visibility("hidden"))) void *MineGun_vtable;
extern "C" void *ObjectGun_dtor(MineGun *self);
void *_ZN7MineGunD1Ev(MineGun *self);
extern "C" __attribute__((visibility("hidden"))) void **MineGun_canvas_holder;
extern "C" void Explosion_ctor(Explosion *self, int kind);

void MineGun::render()
{
    ((ObjectGun *)(this))->render();
    for (uint32_t i = 0; i < U(P(this, 0x8), 0x8); ++i) {
        if (F<uint8_t>(P(P(this, 0x8), 0x40), i) != 0) {
            Array<Explosion *> *explosions = (Array<Explosion *> *)P(this, 0xb4);
            ((Explosion *)(explosions->data()[i]))->render();
        }
    }
}

void *_ZN7MineGunD1Ev(MineGun *self)
{
    P(self, 0x0) = (char *)MineGun_vtable + 8;

    Array<Explosion *> *explosions = (Array<Explosion *> *)P(self, 0xb4);
    if (explosions != 0) {
        for (Explosion *explosion : *explosions) {
            delete explosion;
        }
        explosions->clear();
        delete explosions;
        P(self, 0xb4) = 0;
    }

    ::operator delete[](P(self, 0xb8));
    AEGeometry *geometry = (AEGeometry *)P(self, 0xbc);
    P(self, 0xb8) = 0;
    if (geometry != 0) {
        ((AEGeometry *)geometry)->~AEGeometry();
        ::operator delete(geometry);
    }
    P(self, 0xbc) = 0;
    return ObjectGun_dtor(self);
}

void MineGun::setPlayer(PlayerEgo *player)
{
    P(this, 0xb0) = player;
}

void _ZN7MineGunD0Ev(MineGun *self)
{
    return ::operator delete(_ZN7MineGunD1Ev(self));
}

extern "C" void ObjectGun_ctor(MineGun *self, int param_3, Gun *gun, int param_2,
                                int zero, Level *level);

MineGun *_ZN7MineGunC1EP3GuniiiP5Level(MineGun *self, Gun *gun, int param_2,
                                                   int param_3, int, Level *level)
{
    int zero = 0;
    ObjectGun_ctor(self, param_3, gun, param_2, zero, level);

    I(self, 0xc0) = zero;
    I(self, 0xc4) = zero;
    I(self, 0xc8) = zero;
    P(self, 0x0) = (char *)MineGun_vtable + 8;

    Array<Explosion *> *explosions = new Array<Explosion *>();
    P(self, 0xb4) = explosions;
    uint32_t length = U(gun, 0x8);
    explosions->resize(length);

    explosions = (Array<Explosion *> *)P(self, 0xb4);
    P(self, 0xb8) = ::operator new[](explosions->size());
    uint32_t i = 0;
    for (; i < explosions->size(); ++i) {
        Explosion *explosion = (Explosion *)::operator new(0x68);
        int kind = 0;
        if (I(gun, 0x60) == 0) {
            kind = 7;
        }
        Explosion_ctor(explosion, kind);
        ((Array<Explosion *> *)P(self, 0xb4))->data()[i] = explosion;
        ((Explosion *)(((Array<Explosion *> *)P(self, 0xb4))->data()[i]))->setWeaponIndex(I(gun, 0x58));
        F<uint8_t>(P(self, 0xb8), i) = 1;
        explosions = (Array<Explosion *> *)P(self, 0xb4);
    }

    AEGeometry *geometry = (AEGeometry *)::operator new(0xc0);
    void **holder = MineGun_canvas_holder;
    void *canvas = *holder;
    uint16_t mesh = (uint16_t)(param_2 + 1);
    new ((void *)geometry) AEGeometry((uint16_t)mesh, (PaintCanvas *)canvas, false);
    uint32_t parent = U(self, 0x10);
    P(self, 0xbc) = geometry;
    void *addCanvas = *holder;
    uint32_t child = U(geometry, 0xc);
    ((PaintCanvas*)addCanvas)->TransformAddChild(parent, child);
    AEGeometry *stored = (AEGeometry *)P(self, 0xbc);
    uint32_t transformId = U(stored, 0xc);
    void *getCanvas = *holder;
    uint32_t transform = (uint32_t)(uintptr_t)((PaintCanvas*)getCanvas)->TransformGetTransform(transformId);
    ((AbyssEngine::Transform *)(transform))->SetAnimationState((AbyssEngine::AnimationMode)2, 0);
    return self;
}

static inline Explosion *explosion_at(MineGun *self, uint32_t index)
{
    return ((Array<Explosion *> *)P(self, 0xb4))->data()[index];
}

void MineGun::update(int delta)
{
    char vectorBytes[24];

    ((ObjectGun *)(this))->update(delta);
    if (UC(P(this, 0x8), 0x4c) != 0) {
        void **holder = MineGun_canvas_holder;
        uint32_t transform = (uint32_t)(uintptr_t)((PaintCanvas*)*holder)->TransformGetTransform(U(P(this, 0xbc), 0xc));
        ((AbyssEngine::Transform *)(transform))->Update((long long)delta, 0);
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
                ((PlayerEgo *)((Vector *)vectorBytes))->getPosition();
                Vector *minePosition = (Vector *)(positions + positionOffset);
                *(Vector *)(vectorBytes + 12) =
                    AbyssEngine::AEMath::operator-(*minePosition, *(Vector *)vectorBytes);
                float length = AbyssEngine::AEMath::VectorLength(*(Vector *)(vectorBytes + 12));
                float clamped = range;
                if (length < range) {
                    clamped = length;
                }
                FL(this, 0xd0) = one - clamped / range;
                TargetFollowCamera *camera =
                    (TargetFollowCamera *)((PlayerEgo *)((PlayerEgo *)P(this, 0xb0)))->getTargetFollowCamera();
                ((TargetFollowCamera *)(camera))->setRumblePercentage(FL(this, 0xd0), 0x32);

                ((int *)(vectorBytes + 12))[0] = zero;
                ((int *)(vectorBytes + 12))[1] = zero;
                ((int *)(vectorBytes + 12))[2] = zero;
                ((Explosion *)(explosion_at(this, i)))->start((Vector *)((char *)P(P(this, 0x8), 0x30) + positionOffset), (Vector *)(vectorBytes + 12));
                F<uint8_t>(P(this, 0xb8), i) = 0;
            }

            Explosion *explosion = explosion_at(this, i);
            ((Explosion *)(explosion))->update(delta, (TargetFollowCamera *)0);
            int timer = I(this, 0xcc) + delta;
            if (timer > 1999) {
                timer = 2000;
            }
            I(this, 0xcc) = timer;
            TargetFollowCamera *camera =
                (TargetFollowCamera *)((PlayerEgo *)((PlayerEgo *)P(this, 0xb0)))->getTargetFollowCamera();
            float pct = FL(this, 0xd0) * ((float)I(this, 0xcc) / decay + one);
            ((TargetFollowCamera *)(camera))->setRumblePercentage(pct, 0x32);

            if (((Explosion *)(explosion))->isPlaying() == 0) {
                TargetFollowCamera *camera =
                    (TargetFollowCamera *)((PlayerEgo *)((PlayerEgo *)P(this, 0xb0)))->getTargetFollowCamera();
                ((TargetFollowCamera *)(camera))->setRumblePercentage(0.0f, zero);
                gun = P(this, 0x8);
                I(this, 0xcc) = zero;
                F<uint8_t>(P(gun, 0x40), i) = zero;
                UC(gun, 0x88) = zero;
                F<uint8_t>(P(this, 0xb8), i) = 1;
                ((Explosion *)(explosion_at(this, i)))->reset();
            }
        }
        positionOffset += 0xc;
    }
}

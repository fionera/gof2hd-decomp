#include "class.h"

extern "C" __attribute__((visibility("hidden"))) void **MineGun_canvas_holder;
extern "C" void ObjectGun_update(MineGun *self, int delta);
extern "C" uint32_t PaintCanvas_TransformGetTransform(void *canvas, uint32_t transform);
extern "C" void Transform_Update(uint32_t transform, long long delta, int zero);
extern "C" void PlayerEgo_getPosition(Vector *out, PlayerEgo *self);
extern "C" void AEMath_operator_sub(Vector *out, const Vector *a, const Vector *b);
extern "C" float VectorLength(const Vector *self);
extern "C" TargetFollowCamera *PlayerEgo_getTargetFollowCamera(PlayerEgo *self);
extern "C" void TargetFollowCamera_setRumblePercentage(TargetFollowCamera *self, float pct,
                                                       int duration);
extern "C" void Explosion_start(Explosion *self, const Vector *position, const Vector *zero);
extern "C" void Explosion_update(Explosion *self, int delta, TargetFollowCamera *camera);
extern "C" int Explosion_isPlaying(Explosion *self);
extern "C" void Explosion_reset(Explosion *self);

static inline Explosion *explosion_at(MineGun *self, uint32_t index)
{
    return ((Array<Explosion *> *)self->f_b4)->data[index];
}

void MineGun::update(int delta)
{
    char vectorBytes[24];

    ObjectGun_update(this, delta);
    if (UC(this->f_8, 0x4c) != 0) {
        void **holder = MineGun_canvas_holder;
        uint32_t transform = PaintCanvas_TransformGetTransform(*holder, U(this->f_bc, 0xc));
        Transform_Update(transform, (long long)delta, 0);
    }

    float one = 1.0f;
    float decay = -2000.0f;
    float range = 30000.0f;
    int positionOffset = 0;
    uint32_t i = 0;
    int zero = 0;
    for (; i < U(this->f_8, 0x8); ++i) {
        void *gun = this->f_8;
        if (F<uint8_t>(gun->f_40, i) != 0) {
            if (F<uint8_t>(this->f_b8, i) != 0) {
                I(this, 0xcc) = zero;
                char *positions = (char *)gun->f_30;
                PlayerEgo_getPosition((Vector *)vectorBytes, (PlayerEgo *)this->f_b0);
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
                    PlayerEgo_getTargetFollowCamera((PlayerEgo *)this->f_b0);
                TargetFollowCamera_setRumblePercentage(camera, FL(this, 0xd0), 0x32);

                ((int *)(vectorBytes + 12))[0] = zero;
                ((int *)(vectorBytes + 12))[1] = zero;
                ((int *)(vectorBytes + 12))[2] = zero;
                Explosion_start(explosion_at(this, i),
                                (Vector *)((char *)P(this->f_8, 0x30) + positionOffset),
                                (Vector *)(vectorBytes + 12));
                F<uint8_t>(this->f_b8, i) = 0;
            }

            Explosion *explosion = explosion_at(this, i);
            Explosion_update(explosion, delta, (TargetFollowCamera *)0);
            int timer = I(this, 0xcc) + delta;
            if (timer > 1999) {
                timer = 2000;
            }
            I(this, 0xcc) = timer;
            TargetFollowCamera *camera =
                PlayerEgo_getTargetFollowCamera((PlayerEgo *)this->f_b0);
            float pct = FL(this, 0xd0) * ((float)I(this, 0xcc) / decay + one);
            TargetFollowCamera_setRumblePercentage(camera, pct, 0x32);

            if (Explosion_isPlaying(explosion) == 0) {
                TargetFollowCamera *camera =
                    PlayerEgo_getTargetFollowCamera((PlayerEgo *)this->f_b0);
                TargetFollowCamera_setRumblePercentage(camera, 0.0f, zero);
                gun = this->f_8;
                I(this, 0xcc) = zero;
                F<uint8_t>(gun->f_40, i) = zero;
                UC(gun, 0x88) = zero;
                F<uint8_t>(this->f_b8, i) = 1;
                Explosion_reset(explosion_at(this, i));
            }
        }
        positionOffset += 0xc;
    }
}

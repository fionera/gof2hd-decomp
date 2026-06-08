#include "class.h"

struct BombGun {
    void update(int elapsed);
};

extern "C" __attribute__((visibility("hidden"))) void **BombGun_update_canvas_a;
extern "C" __attribute__((visibility("hidden"))) void **BombGun_update_canvas_b;
extern "C" __attribute__((visibility("hidden"))) void **BombGun_sound_play;
extern "C" __attribute__((visibility("hidden"))) void **BombGun_sound_param;
extern "C" __attribute__((visibility("hidden"))) TargetFollowCamera *(*BombGun_getCamera)(
    PlayerEgo *player);
extern "C" __attribute__((visibility("hidden"))) void **BombGun_status;
extern "C" __attribute__((visibility("hidden"))) void **BombGun_sound_stop;

extern "C" void PlayerEgo_getPosition(void *out, PlayerEgo *self);
extern "C" void *Vector_assign(void *dst, void *src);
extern "C" uint64_t PaintCanvas_TransformGetTransform(void *canvas, uint32_t transform);
extern "C" void Transform_Update(uint64_t transform, int64_t elapsed, int zero);
extern "C" void Transform_SetAnimationState(void *transform, int state, void *arg);
extern "C" void Matrix_assign(void *dst, void *src);
extern "C" void FModSound_play(void *self, int sound, void *a, void *b, float volume);
extern "C" void *PaintCanvas_TransformGetLocal(void *canvas, uint32_t transform);
extern "C" void AEGeometry_setMatrix(AEGeometry *self, void *matrix);
extern "C" void Vector_scale_scalar(void *out, float scale, void *vec);
extern "C" void Vector_add(void *out, void *a, void *b);
extern "C" void AEGeometry_setPosition(AEGeometry *self, void *position);
extern "C" void AEGeometry_updateReferenceMatrix(AEGeometry *self);
extern "C" void TargetFollowCamera_setTarget(TargetFollowCamera *self, AEGeometry *target);
extern "C" void TargetFollowCamera_setCamOffset(TargetFollowCamera *self, void *offset);
extern "C" void TargetFollowCamera_setTargetOffset(TargetFollowCamera *self, void *offset);
extern "C" void TargetFollowCamera_useTargetsUpVector(TargetFollowCamera *self, bool enabled);
extern "C" void PlayerEgo_setRocketControl(PlayerEgo *self, Gun *gun, AEGeometry *geometry);
extern "C" float PlayerEgo_getRocketBanking(PlayerEgo *self);
extern "C" void FModSound_setParamValue(void *self, int param, int sound, float value);
extern "C" void RocketGun_update(BombGun *self, int elapsed);
extern "C" void LevelScript_resetCamera(LevelScript *self, Level *level);
extern "C" void FModSound_stop(void *self, int sound);
extern "C" void Vector_sub(void *out, void *a, void *b);
extern "C" float VectorLength(void *self);
extern "C" int Gun_getMagnitude(Gun *self);
extern "C" int Status_hardCoreMode(void *self);
extern "C" float Player_damage(Player *self, int amount, int zero, int minusOne);
extern "C" void PlayerEgo_addNukeVolatileForce(PlayerEgo *self, float value);
extern "C" TargetFollowCamera *PlayerEgo_getTargetFollowCamera(PlayerEgo *self);
extern "C" void TargetFollowCamera_setRumblePercentage(TargetFollowCamera *self, float pct,
                                                        int duration);
extern "C" void PlayerEgo_GetDirVector(void *out, PlayerEgo *self);
extern "C" void Explosion_start(Explosion *self, void *position, void *direction);
extern "C" void Explosion_update(Explosion *self, int elapsed, TargetFollowCamera *camera);
extern "C" int Explosion_isPlaying(Explosion *self);
extern "C" void Explosion_reset(Explosion *self);

static const float kRocketOffsetScale = 350.0f;
static const float kBombScale = 0.2f;
static const float kZero = 0.0f;
static const float kRumbleDistance = 30000.0f;
static const float kRumbleTime = -2000.0f;

void BombGun::update(int elapsed)
{
    char scaled[12];
    char position[12];
    char work[12];

    PlayerEgo *player = F<PlayerEgo *>(this, 0xec);
    if (player == 0)
        return;

    Gun *gun = F<Gun *>(this, 0x8);
    if (F<uint8_t>(gun, 0x88) == 0) {
        if (F<int>(this, 0x128) == 0x2a) {
            PlayerEgo_getPosition(position, player);
        } else {
            void *gunPos = F<void *>(gun, 0xc);
            *(uint64_t *)position = *(uint64_t *)gunPos;
            *(uint32_t *)(position + 8) = F<uint32_t>(gunPos, 0x8);
        }
        Vector_assign((char *)this + 0xf8, position);
    }

    gun = F<Gun *>(this, 0x8);
    bool playerControlled = F<uint8_t>(this, 0x24) != 0;
    if (!playerControlled) {
        if (F<uint8_t>(gun, 0x4c) != 0)
            goto update_transforms;
    } else if (F<uint8_t>(gun, 0x4c) != 0) {
        if (F<uint8_t>(gun, 0x88) != 0)
            goto update_transforms;
        {
            void **canvas = BombGun_update_canvas_b;
            uint32_t *activeTransform = &F<uint32_t>(this, 0x10);
            if (F<uint8_t>(gun, 0x4d) != 0) {
                F<uint8_t>(gun, 0x4d) = 0;
                uint64_t transform = PaintCanvas_TransformGetTransform(*canvas, *activeTransform);
                Transform_SetAnimationState((void *)(uint32_t)transform, 3, 0);
                transform = PaintCanvas_TransformGetTransform(*canvas, *activeTransform);
                Transform_SetAnimationState((void *)(uint32_t)transform, 1, 0);
                Matrix_assign((char *)player + 0x10, (char *)F<void *>(player, 0x0) + 4);
                FModSound_play(*BombGun_sound_play, 0x45c, 0, 0, 0.0f);
            }

            AEGeometry *geometry = F<AEGeometry *>(this, 0xe8);
            void *local = PaintCanvas_TransformGetLocal(*canvas, *activeTransform);
            AEGeometry_setMatrix(geometry, local);
            Vector_scale_scalar(scaled, kRocketOffsetScale, F<void *>(gun, 0x18));
            Vector_add(position, F<void *>(gun, 0xc), scaled);
            AEGeometry_setPosition(geometry, position);
            AEGeometry_updateReferenceMatrix(geometry);

            TargetFollowCamera *camera = BombGun_getCamera(player);
            TargetFollowCamera_setTarget(camera, geometry);
            camera = BombGun_getCamera(player);
            TargetFollowCamera_setCamOffset(camera, (char *)this + 0x110);
            camera = BombGun_getCamera(player);
            TargetFollowCamera_setTargetOffset(camera, (char *)this + 0x11c);
            camera = BombGun_getCamera(player);
            TargetFollowCamera_useTargetsUpVector(camera, false);
            PlayerEgo_setRocketControl(player, gun, geometry);

            if (*(int *)F<void *>(gun, 0x3c) < F<int>(gun, 0x44) - 500) {
                uint64_t transform = PaintCanvas_TransformGetTransform(*canvas, *activeTransform);
                Transform_Update(transform, (int64_t)elapsed, 0);
            }

            float bank = PlayerEgo_getRocketBanking(player) * kBombScale;
            F<float>(this, 0x20) = bank;
            FModSound_setParamValue(*BombGun_sound_param, 0, 0x45c, bank * 0.5f);
        }
    }
    goto after_transforms;

update_transforms:
    {
        void **canvas = BombGun_update_canvas_a;
        uint64_t transform = PaintCanvas_TransformGetTransform(*canvas, F<uint32_t>(this, 0x10));
        Transform_Update(transform, (int64_t)elapsed, 0);
        if (F<int>(this, 0xf4) != -1) {
            transform = PaintCanvas_TransformGetTransform(*canvas, F<uint32_t>(this, 0xf4));
            Transform_Update(transform, (int64_t)elapsed, 0);
        }
    }

after_transforms:

    RocketGun_update(this, elapsed);
    gun = F<Gun *>(this, 0x8);
    if (F<uint8_t>(gun, 0x88) == 0)
        return;

    if (F<uint8_t>(this, 0x104) != 0) {
        if (F<uint8_t>(this, 0x24) != 0) {
            player = F<PlayerEgo *>(this, 0xec);
            LevelScript_resetCamera(F<LevelScript *>(player, 0x10), F<Level *>(player, 0xc));
            PlayerEgo_setRocketControl(player, 0, F<AEGeometry *>(this, 0xe8));
            FModSound_stop(*BombGun_sound_stop, 0x45c);
        }

        F<int>(this, 0x108) = 0;
        player = F<PlayerEgo *>(this, 0xec);
        PlayerEgo_getPosition(work, player);
        Vector_sub(position, (char *)this + 0xf8, work);
        float distance = VectorLength(position);
        float magnitude = (float)Gun_getMagnitude(F<Gun *>(this, 0x8));
        float force = (((magnitude * 0.5f) - distance) / (magnitude * 0.5f)) * 0.5f;
        if (force > 1.0f)
            force = 1.0f;
        if (force < kZero)
            force = kZero;
        if (F<int>(this, 0x128) == 0x2a)
            force *= kBombScale;
        if (Status_hardCoreMode(*BombGun_status) != 0) {
            Gun *damageGun = F<Gun *>(this, 0x8);
            Player_damage(F<Player *>(player, 0x0), (int)(force * (float)F<int>(damageGun, 0x60)),
                          0, -1);
        }
        PlayerEgo_addNukeVolatileForce(player, force);

        float capped = kRumbleDistance;
        if (distance < kRumbleDistance)
            capped = distance;
        F<float>(this, 0x10c) = 1.0f - capped / kRumbleDistance;
        TargetFollowCamera_setRumblePercentage(PlayerEgo_getTargetFollowCamera(player),
                                               F<float>(this, 0x10c), 0x32);

        Explosion *explosion = F<Explosion *>(this, 0xf0);
        if (F<int>(this, 0x128) == 0x2a) {
            PlayerEgo_GetDirVector(position, player);
        } else {
            *(uint64_t *)position = 0;
            *(uint32_t *)(position + 8) = 0;
        }
        Explosion_start(explosion, (char *)this + 0xf8, position);
        F<uint8_t>(this, 0x104) = 0;
    }

    Explosion_update(F<Explosion *>(this, 0xf0), elapsed, 0);
    int timer = F<int>(this, 0x108) + elapsed;
    if (timer > 2000)
        timer = 2000;
    F<int>(this, 0x108) = timer;

    player = F<PlayerEgo *>(this, 0xec);
    float rumble = F<float>(this, 0x10c) * ((float)timer / kRumbleTime + 1.0f);
    TargetFollowCamera_setRumblePercentage(PlayerEgo_getTargetFollowCamera(player), rumble, 0x32);

    if (Explosion_isPlaying(F<Explosion *>(this, 0xf0)) == 0) {
        TargetFollowCamera_setRumblePercentage(PlayerEgo_getTargetFollowCamera(player), 0.0f, 0);
        F<int>(this, 0x108) = 0;
        F<uint8_t>(F<void *>(this, 0x8), 0x88) = 0;
        F<uint8_t>(this, 0x104) = 1;
        Explosion_reset(F<Explosion *>(this, 0xf0));
    }
}

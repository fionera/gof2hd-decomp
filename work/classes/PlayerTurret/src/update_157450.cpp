#include "class.h"

using AbyssEngine::AEMath::Matrix;
using AbyssEngine::AEMath::Vector;

extern "C" int Player_isActive(Player *self);
extern "C" void Player_setVulnerable(Player *self, bool value);
extern "C" int Player_getHitpoints(Player *self);
extern "C" void Player_setActive(Player *self, bool value);
extern "C" int Player_turnedEnemy(Player *self);
extern "C" int Player_isAlwaysFriend(Player *self);
extern "C" int Level_getPlayer(Level *self);
extern "C" int Status_getStanding();
extern "C" int Standing_isEnemy(int standing, int value);
extern "C" int Standing_isFriend(int standing, int value);
extern "C" void *AEGeometry_getMatrix(AEGeometry *self);
extern "C" void AEGeometry_setMatrix(AEGeometry *self, const void *matrix);
extern "C" void AEGeometry_translate(AEGeometry *self, const Vector *position);
extern "C" void AEGeometry_getPosition(Vector *out, AEGeometry *self);
extern "C" void Matrix_assign(void *dst, const void *src);
extern "C" void MatrixRotateVector(Vector *out, const void *matrix, const Vector *vector);
extern "C" Vector *Vector_assign(Vector *dst, const Vector *src);
extern "C" void *FModSound_play(int sound, int id, Vector *pos, float volume);
extern "C" void ParticleSystemManager_emitManual(int manager, int settings, Vector *pos,
                                                 Vector *velocity, float value);
extern "C" void ParticleSystemManager_enableSystemEmit(int manager, int system, int enable);
extern "C" void Explosion_start(Explosion *self, Vector *position, Vector *velocity);
extern "C" void Explosion_update(Explosion *self, int delta, TargetFollowCamera *camera);
extern "C" int AERandom_nextInt(int rng, int max);
extern "C" void Array_int_ctor(IntArray *array);
extern "C" void ArrayAdd_int(int value, IntArray *array);
extern "C" void KIPlayer_createCrate(PlayerTurret *self, int kind);

extern int *gPlayerTurretSound __attribute__((visibility("hidden")));
extern int *gPlayerTurretRandom __attribute__((visibility("hidden")));
extern int *gPlayerTurretStanding __attribute__((visibility("hidden")));

struct PlayerTurret {
    // @portable-fields
    void* f_0; // 0x0
    unsigned char _pad_4[4];
    void* f_8; // 0x8
    void* f_c; // 0xc
    unsigned char _pad_10[64];
    void* f_50; // 0x50
    void* f_54; // 0x54
    unsigned char _pad_58[32];
    void* f_78; // 0x78
    unsigned char _pad_7c[192];
    void* f_13c; // 0x13c
    void* f_140; // 0x140
    void* f_144; // 0x144
    void* f_148; // 0x148
    void* f_14c; // 0x14c
    void* f_150; // 0x150
    void* f_154; // 0x154
    void* f_158; // 0x158
    void* f_15c; // 0x15c

    void update(int delta);
    void handleSentryGun(int delta);
    void handleTurret(int delta);
};

void PlayerTurret::update(int delta)
{
    char matrixBytes[60];
    char vectorBytes[12];
    I(this, 0x124) = delta;

    Player *player = TP<Player>(this, 0x4);
    if (Player_isActive(player) == 0) {
        return;
    }

    if (UC(this, 0x3f) != 0 && I(this, 0x128) < 3000) {
        Player_setVulnerable(player, false);
        int time = I(this, 0x128) + delta;
        I(this, 0x128) = time;
        if (time > 2999) {
            Player_setVulnerable(player, true);
        }
    }

    if (this->f_154 != 0) {
        __aeabi_memcpy(matrixBytes, B(P(TP<KIPlayer>(this, 0x154), 0x4), 0x4), 0x3c);
        MatrixRotateVector((Vector *)vectorBytes, matrixBytes, (Vector *)B(this, 0x158));
        Vector_assign((Vector *)B(this, 0x90), (Vector *)vectorBytes);
        AEGeometry_setMatrix(TP<AEGeometry>(this, 0x8), matrixBytes);
        AEGeometry_translate(TP<AEGeometry>(this, 0x8), (Vector *)B(this, 0x90));
    }

    void *matrix = AEGeometry_getMatrix(TP<AEGeometry>(this, 0x8));
    Matrix_assign(B(player, 0x4), matrix);
    AEGeometry_getPosition((Vector *)vectorBytes, TP<AEGeometry>(this, 0x8));
    Vector_assign((Vector *)B(this, 0x2c), (Vector *)vectorBytes);

    int hp = Player_getHitpoints(player);
    int state = I(this, 0x88);
    if (hp < 1 && (uint32_t)(state - 3) >= 2) {
        I(this, 0x12c) = 0;
        I(this, 0x88) = 3;
        float value = (float)(uint32_t)FModSound_play(**(int **)gPlayerTurretSound, 0x16, 0, 0.0f);
        Vector zero = {0.0f, 0.0f, 0.0f};
        ParticleSystemManager_emitManual(I(this->f_54, 0x74), I(this->f_54, 0x3c),
                                         (Vector *)B(this, 0x2c), &zero, value);
        ParticleSystemManager_enableSystemEmit(I(this->f_54, 0x74), I(this, 0x138), 1);
        Explosion_start(TP<Explosion>(this, 0x13c), (Vector *)B(this, 0x2c), &zero);

        int random = AERandom_nextInt(*gPlayerTurretRandom, 100);
        if (random < 0) {
            UC(this, 0x4c) = 1;
            IntArray *array = (IntArray *)operator new(0xc);
            Array_int_ctor(array);
            this->f_50 = array;
            ArrayAdd_int(99, array);
            ArrayAdd_int(AERandom_nextInt(*gPlayerTurretRandom, 10) + 1, (IntArray *)this->f_50);
            KIPlayer_createCrate(this, 3);
            UC(this, 0x4c) = 1;
        } else {
            int levelPlayer = Level_getPlayer((Level *)this->f_54);
            if (levelPlayer != 0) {
                levelPlayer = Level_getPlayer((Level *)this->f_54);
                if (I((void *)levelPlayer, 0x14) != 0) {
                    levelPlayer = Level_getPlayer((Level *)this->f_54);
                    if (P(P((void *)levelPlayer, 0x14), 0x1c) == this) {
                        levelPlayer = Level_getPlayer((Level *)this->f_54);
                        P(P((void *)levelPlayer, 0x14), 0x1c) = 0;
                    }
                }
            }
        }
        state = I(this, 0x88);
    }

    if (state == 4) {
        return;
    }
    if (state == 3) {
        I(this, 0x12c) = I(this, 0x12c) + delta;
        Explosion_update(TP<Explosion>(this, 0x13c), delta, 0);
        if (I(this, 0x12c) > 4500) {
            ParticleSystemManager_enableSystemEmit(I(this->f_54, 0x74), I(this, 0x138), 0);
            I(this, 0x12c) = 0;
            I(this, 0x88) = 4;
            Player_setActive(player, false);
            if (UC(this, 0x3f) != 0) {
                I(this->f_54, 0x6c) = I(this->f_54, 0x6c) - 1;
            }
        }
        return;
    }

    uint32_t standing = U(this, 0x28);
    if ((standing & 0xfffffffeU) == 8) {
        UC(player, 0x5c) = 1;
        UC(player, 0x5d) = 0;
    } else {
        int s = Status_getStanding();
        UC(player, 0x5c) = Standing_isEnemy(s, standing);
        if ((standing & 0xfffffffeU) == 8) {
            UC(player, 0x5d) = 0;
        } else {
            UC(player, 0x5d) = Standing_isFriend(Status_getStanding(), standing);
        }
    }
    if (Player_turnedEnemy(player) != 0) {
        US(player, 0x5c) = 1;
    }
    if (Player_isAlwaysFriend(player) != 0) {
        US(player, 0x5c) = 0x100;
    }

    if (UC(this, 0x3f) != 0) {
        handleSentryGun(delta);
    } else if (UC(this, 0x25) != 0 && this->f_144 != 0) {
        handleTurret(delta);
    }
}

#include "class.h"

extern "C" int Player_getHitpoints(Player *self);
extern "C" Matrix *AEGeometry_getMatrix(AEGeometry *self);
extern "C" void AEGeometry_setMatrix(AEGeometry *self, Matrix *matrix);
extern "C" void AEGeometry_moveForward(AEGeometry *self, float distance);
extern "C" void Matrix_multiply(Matrix *out, Matrix *lhs, Matrix *rhs);
extern "C" void Matrix_setRotation(Matrix *out, Matrix *matrix, float x, float y, float z);
extern "C" void Matrix_assign(Matrix *dst, Matrix *src);
extern "C" int AERandom_nextInt(int max, int keep);
extern "C" int *PlayerCreature_randomMax __attribute__((visibility("hidden")));
extern "C" FModSound **PlayerCreature_sound __attribute__((visibility("hidden")));
extern "C" void FModSound_play(FModSound *self, int id, Vector *a, Vector *b, float volume);
extern "C" void Player_setActive(Player *self, bool active);
extern "C" void *Level_getPlayer(void *level);
extern "C" void ParticleSystemManager_emitManual(int manager, Vector *emitter, Vector *position,
                                                  int unused, Vector *zero, float value);

struct PlayerCreature {
    void update(int elapsed);
};

void PlayerCreature::update(int elapsed)
{
    Vector zero;
    char workBytes[0x3c];

    int lastHitpoints = F<int>(this, 0x13c);
    F<int>(this, 0x124) = elapsed;
    int hitpoints = Player_getHitpoints(F<Player *>(this, 0x4));
    if (lastHitpoints > hitpoints && F<int>(this, 0x88) != 4) {
        F<uint8_t>(this, 0x128) = 1;
    }

    F<int>(this, 0x13c) = Player_getHitpoints(F<Player *>(this, 0x4));

    if (F<uint8_t>(this, 0x128) != 0) {
        float elapsedFloat = (float)elapsed;
        int rageTime = F<int>(this, 0x130) + elapsed;
        F<int>(this, 0x130) = rageTime;
        int endurance = F<int>(this, 0x138) - elapsed;
        float rageScale = F<float>(this, 0x12c) + elapsedFloat * 0.0007f;
        elapsed = (int)(rageScale * elapsedFloat);
        F<int>(this, 0x138) = endurance;
        F<float>(this, 0x12c) = rageScale;

        if (rageTime < 4000) {
            AEGeometry *geometry = F<AEGeometry *>(this, 0x8);
            Matrix *matrix = AEGeometry_getMatrix(geometry);
            Matrix_multiply((Matrix *)workBytes, matrix, (Matrix *)((char *)this + 0x144));
            AEGeometry_setMatrix(geometry, (Matrix *)workBytes);
        } else {
            int half = elapsed >> 1;
            float negativeHalf = (float)half * -0.5f;
            int *randomMax = PlayerCreature_randomMax;
            int first = AERandom_nextInt(*randomMax, half);
            float firstFloat = (float)first;
            int second = AERandom_nextInt(*randomMax, half);
            float secondFloat = (float)second;
            float x = ((negativeHalf + firstFloat) * 0.000244140625f) * 2.0f * 3.1415927f;
            float z = ((negativeHalf + secondFloat) * 0.000244140625f) * 2.0f * 3.1415927f;
            Matrix_setRotation((Matrix *)workBytes, (Matrix *)((char *)this + 0x144), x, z, 0.0f);
            F<int>(this, 0x130) = 0;
        }

        if (F<int>(this, 0x138) < 1) {
            if (F<uint8_t>(this, 0x129) == 0) {
                int maxEndurance = F<int>(this, 0x134);
                F<int>(this, 0x130) = 0;
                F<uint16_t>(this, 0x128) = 0;
                F<int>(this, 0x138) = maxEndurance;
            } else {
                *(volatile uint8_t *)((char *)this + 0x128) = 0;
                *(volatile uint8_t *)((char *)this + 0x12a) = 1;
            }
        }
    }

    if (Player_getHitpoints(F<Player *>(this, 0x4)) < 1) {
        int state = F<int>(this, 0x88);
        if ((uint32_t)(state - 3) >= 2) {
            F<int>(this, 0x88) = 3;
            FModSound_play(*PlayerCreature_sound, 0x16, 0, 0, 0.0f);
            Player_setActive(F<Player *>(this, 0x4), false);

            void *level = F<void *>(this, 0x54);
            void *player = Level_getPlayer(level);
            if (F<PlayerCreature *>(F<void *>(player, 0x14), 0x1c) == this) {
                player = Level_getPlayer(level);
                F<PlayerCreature *>(F<void *>(player, 0x14), 0x1c) = 0;
            }

            zero.x = 0.0f;
            zero.y = 0.0f;
            zero.z = 0.0f;
            Vector *position = (Vector *)workBytes;
            position->x = F<float>(this, 0x58);
            position->y = F<float>(this, 0x5c);
            position->z = F<float>(this, 0x60);
            void *levelObject = F<void *>(this, 0x54);
            ParticleSystemManager_emitManual(F<int>(levelObject, 0x74),
                                             F<Vector *>(levelObject, 0x34),
                                             position, 0, &zero, -1.0f);
        }
    }

    int state = F<int>(this, 0x88);
    if (state == 3) {
        F<int>(this, 0x88) = 4;
    } else if (state == 0 && F<uint8_t>(this, 0x12a) == 0) {
        AEGeometry_moveForward(F<AEGeometry *>(this, 0x8), (float)elapsed);
    }

    Matrix *matrix = AEGeometry_getMatrix(F<AEGeometry *>(this, 0x8));
    Matrix_assign((Matrix *)((char *)F<Player *>(this, 0x4) + 4), matrix);
}

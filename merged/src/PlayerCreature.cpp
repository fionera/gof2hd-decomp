#include "PlayerCreature.h"


extern "C" void AEGeometry_render(AEGeometry *self);
extern "C" void PlayerCreature_render_tail(PlayerCreature *self);
extern "C" PlayerCreature *KIPlayer_dtor(KIPlayer *self);
extern "C" void PlayerCreature_dtor_tail(PlayerCreature *self);
extern "C" int PlayerCreature_weightTable[] __attribute__((visibility("hidden")));
extern "C" int PlayerCreature_rageTable[] __attribute__((visibility("hidden")));
extern "C" char PlayerCreature_vtable;
extern "C" void Matrix_ctor(Matrix *self);
extern "C" int Player_getHitpoints(Player *self, int keepOne, int keepMax);
extern "C" int PlayerCreature_enduranceTable[] __attribute__((visibility("hidden")));
extern "C" void PlayerCreature_hook_tail(PlayerCreature *self, int value);
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
extern "C" void KIPlayer_reset(KIPlayer *self);
extern "C" void KIPlayer_setActive(KIPlayer *self, bool active);
extern "C" void Matrix_assign(Matrix *dst, const float *src);

// ---- isHooked_11cd38.cpp ----
struct PlayerCreature {
    uint8_t isHooked();
};

uint8_t PlayerCreature::isHooked()
{
    return F<uint8_t>(this, 0x129);
}

// ---- calmDown_11cc28.cpp ----
struct PlayerCreature {
    void calmDown();
};

void PlayerCreature::calmDown()
{
    int maxEndurance = F<int>(this, 0x134);
    *(volatile int *)((char *)this + 0x130) = 0;
    *(volatile uint16_t *)((char *)this + 0x128) = 0;
    F<int>(this, 0x138) = maxEndurance;
}

// ---- unhook_11cd4e.cpp ----
struct PlayerCreature {
    void unhook();
};

void PlayerCreature::unhook()
{
    int maxEndurance = F<int>(this, 0x134);
    *(volatile uint8_t *)((char *)this + 0x12a) = 0;
    *(volatile int *)((char *)this + 0x130) = 0;
    *(volatile uint16_t *)((char *)this + 0x128) = 0;
    *(volatile int *)((char *)this + 0x138) = maxEndurance;
}

// ---- render_11cfb8.cpp ----
struct PlayerCreature {
    void render();
};

void PlayerCreature::render()
{
    AEGeometry *geometry = F<AEGeometry *>(this, 0x78);
    if (geometry != 0) {
        AEGeometry_render(geometry);
    }
    if ((uint32_t)(F<int>(this, 0x88) - 3) >= 2) {
        return PlayerCreature_render_tail(this);
    }
}

// ---- getEndurance_11d004.cpp ----
struct PlayerCreature {
    int getEndurance();
};

int PlayerCreature::getEndurance()
{
    return F<int>(this, 0x138);
}

// ---- _PlayerCreature_11ccd8.cpp ----
extern "C" void _ZN14PlayerCreatureD1Ev(PlayerCreature *self)
{
    PlayerCreature_dtor_tail(KIPlayer_dtor((KIPlayer *)self));
}

// ---- getWeight_11cfe4.cpp ----
struct PlayerCreature {
    int getWeight();
};

int PlayerCreature::getWeight()
{
    return PlayerCreature_weightTable[F<int>(this, 0xac)];
}

// ---- isCaught_11cd3e.cpp ----
struct PlayerCreature {
    uint8_t isCaught();
};

uint8_t PlayerCreature::isCaught()
{
    return F<uint8_t>(this, 0x12a);
}

// ---- rage_11cce8.cpp ----
struct PlayerCreature {
    void rage(int amount);
};

void PlayerCreature::rage(int amount)
{
    float amountScale = (float)amount / -100.0f;
    float typeScale = (float)PlayerCreature_rageTable[F<int>(this, 0xac)];
    F<int>(this, 0x130) = 0;
    F<uint16_t>(this, 0x128) = 0x101;
    F<float>(this, 0x12c) = (amountScale + 1.0f) * (typeScale + 1.0f);
}

// ---- getMaxEndurance_11cffe.cpp ----
struct PlayerCreature {
    int getMaxEndurance();
};

int PlayerCreature::getMaxEndurance()
{
    return F<int>(this, 0x134);
}

// ---- getItemIndex_11cff8.cpp ----
struct PlayerCreature {
    int getItemIndex();
};

int PlayerCreature::getItemIndex()
{
    return F<int>(this, 0x140);
}

// ---- PlayerCreature_11cb5c.cpp ----
extern "C" void KIPlayer_ctor(KIPlayer *self, int kind, int route, Player *player,
                              AEGeometry *geometry, float x, float y, float z, bool active);

struct PlayerCreature {
    PlayerCreature(int kind, int itemIndex, Player *player, AEGeometry *geometry,
                   float x, float y, float z);
    void reset();
};

PlayerCreature::PlayerCreature(int kind, int itemIndex, Player *player, AEGeometry *geometry,
                               float x, float y, float z)
{
    KIPlayer_ctor((KIPlayer *)this, kind, -1, player, geometry, x, y, z, false);
    F<char *>(this, 0x0) = &PlayerCreature_vtable + 8;
    Matrix_ctor((Matrix *)((char *)this + 0x144));

    Player *playerObject = F<Player *>(this, 0x4);
    int oneBits = 0x3f800000;
    F<uint8_t>(this, 0x12a) = 0;
    F<int>(this, 0x140) = itemIndex;
    F<int>(this, 0x12c) = oneBits;
    F<int>(this, 0x130) = 0;
    F<uint16_t>(this, 0x128) = 0;
    int previousMaxEndurance = F<int>(this, 0x134);
    F<int>(this, 0x138) = previousMaxEndurance;

    int hitpoints = Player_getHitpoints(playerObject, oneBits, previousMaxEndurance);
    int endurance = (int)(((float)PlayerCreature_enduranceTable[kind] / 10.0f) * 8000.0f) + 8000;
    F<int>(this, 0x134) = endurance;
    F<int>(this, 0x138) = endurance;
    F<int>(this, 0x13c) = hitpoints;
    reset();
}

// ---- hook_11cd44.cpp ----
struct PlayerCreature {
    void hook(int);
};

void PlayerCreature::hook(int value)
{
    F<uint8_t>(this, 0x129) = 1;
    return PlayerCreature_hook_tail(this, value);
}

// ---- update_11cd68.cpp ----
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

// ---- reset_11cc40.cpp ----
struct PlayerCreature {
    void reset();
};

void PlayerCreature::reset()
{
    KIPlayer_reset((KIPlayer *)this);
    F<int>(this, 0x88) = 0;
    KIPlayer_setActive((KIPlayer *)this, true);
    F<int>(this, 0x138) = F<int>(this, 0x134);
    F<int>(this, 0x13c) = Player_getHitpoints(F<Player *>(this, 0x4));

    char matrixBytes[0x3c];
    float *matrix = (float *)matrixBytes;
    matrix[0] = 1.0f;
    matrix[1] = 0.0f;
    matrix[2] = 0.0f;
    matrix[3] = 0.0f;
    matrix[4] = 0.0f;
    matrix[5] = 1.0f;
    matrix[6] = 0.0f;
    matrix[7] = 0.0f;
    matrix[8] = 0.0f;
    matrix[9] = 0.0f;
    matrix[10] = 1.0f;
    matrix[11] = 0.0f;
    matrix[12] = 1.0f;
    matrix[13] = 1.0f;
    matrix[14] = 1.0f;
    Matrix_assign((Matrix *)((char *)this + 0x144), matrix);
}

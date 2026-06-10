#include "gof2/PlayerCreature.h"
#include "gof2/KIPlayer.h"
#include "gof2/Player.h"


extern "C" void AEGeometry_render(AEGeometry *self);
extern "C" void PlayerCreature_render_tail(PlayerCreature *self);
extern "C" PlayerCreature *KIPlayer_dtor(KIPlayer *self);
extern "C" void PlayerCreature_dtor_tail(PlayerCreature *self);
extern "C" int PlayerCreature_weightTable[] __attribute__((visibility("hidden")));
extern "C" int PlayerCreature_rageTable[] __attribute__((visibility("hidden")));
extern "C" char PlayerCreature_vtable;
extern "C" void Matrix_ctor(Matrix *self);
extern "C" int PlayerCreature_enduranceTable[] __attribute__((visibility("hidden")));
extern "C" void PlayerCreature_hook_tail(PlayerCreature *self, int value);
extern "C" Matrix *AEGeometry_getMatrix(AEGeometry *self);
extern "C" void AEGeometry_setMatrix(AEGeometry *self, Matrix *matrix);
extern "C" void AEGeometry_moveForward(AEGeometry *self, float distance);
extern "C" void Matrix_multiply(Matrix *out, Matrix *lhs, Matrix *rhs);
extern "C" void Matrix_setRotation(Matrix *out, Matrix *matrix, float x, float y, float z);
extern "C" void Matrix_assign(Matrix *dst, const void *src);
extern "C" int AERandom_nextInt(int max, int keep);
extern "C" int *PlayerCreature_randomMax __attribute__((visibility("hidden")));
extern "C" FModSound **PlayerCreature_sound __attribute__((visibility("hidden")));
extern "C" void FModSound_play(FModSound *self, int id, Vector *a, Vector *b, float volume);
extern "C" void *Level_getPlayer(void *level);

// ---- isHooked_11cd38.cpp ----

uint8_t PlayerCreature::isHooked()
{
    return this->field_0x129;
}

// ---- calmDown_11cc28.cpp ----

void PlayerCreature::calmDown()
{
    int maxEndurance = this->field_0x134;
    this->field_0x130 = 0;
    this->field_0x128 = 0;
    this->field_0x138 = maxEndurance;
}

// ---- unhook_11cd4e.cpp ----

void PlayerCreature::unhook()
{
    int maxEndurance = this->field_0x134;
    this->field_0x12a = 0;
    this->field_0x130 = 0;
    this->field_0x128 = 0;
    this->field_0x138 = maxEndurance;
}

// ---- render_11cfb8.cpp ----

void PlayerCreature::render()
{
    AEGeometry *geometry = this->field_0x78;
    if (geometry != 0) {
        AEGeometry_render(geometry);
    }
    if ((uint32_t)(this->field_0x88 - 3) >= 2) {
        return PlayerCreature_render_tail(this);
    }
}

// ---- getEndurance_11d004.cpp ----

int PlayerCreature::getEndurance()
{
    return this->field_0x138;
}

// ---- _PlayerCreature_11ccd8.cpp ----
extern "C" void _ZN14PlayerCreatureD1Ev(PlayerCreature *self)
{
    PlayerCreature_dtor_tail(KIPlayer_dtor((KIPlayer *)self));
}

// ---- getWeight_11cfe4.cpp ----

int PlayerCreature::getWeight()
{
    return PlayerCreature_weightTable[this->field_0xac];
}

// ---- isCaught_11cd3e.cpp ----

uint8_t PlayerCreature::isCaught()
{
    return this->field_0x12a;
}

// ---- rage_11cce8.cpp ----

void PlayerCreature::rage(int amount)
{
    float amountScale = (float)amount / -100.0f;
    float typeScale = (float)PlayerCreature_rageTable[this->field_0xac];
    this->field_0x130 = 0;
    this->field_0x128 = 0x101;
    this->field_0x12c = (amountScale + 1.0f) * (typeScale + 1.0f);
}

// ---- getMaxEndurance_11cffe.cpp ----

int PlayerCreature::getMaxEndurance()
{
    return this->field_0x134;
}

// ---- getItemIndex_11cff8.cpp ----

int PlayerCreature::getItemIndex()
{
    return this->field_0x140;
}

// ---- PlayerCreature_11cb5c.cpp ----


PlayerCreature::PlayerCreature(int kind, int itemIndex, Player *player, AEGeometry *geometry,
                               float x, float y, float z)
{
    this->field_0x0 = &PlayerCreature_vtable + 8;
    Matrix_ctor((Matrix *)((char *)this + 0x144));

    Player *playerObject = this->field_0x4;
    int oneBits = 0x3f800000;
    this->field_0x12a = 0;
    this->field_0x140 = itemIndex;
    this->field_0x12c = oneBits;
    this->field_0x130 = 0;
    this->field_0x128 = 0;
    int previousMaxEndurance = this->field_0x134;
    this->field_0x138 = previousMaxEndurance;

    int hitpoints = ((Player *)(playerObject))->getHitpoints();
    int endurance = (int)(((float)PlayerCreature_enduranceTable[kind] / 10.0f) * 8000.0f) + 8000;
    this->field_0x134 = endurance;
    this->field_0x138 = endurance;
    this->field_0x13c = hitpoints;
    reset();
}

// ---- hook_11cd44.cpp ----

void PlayerCreature::hook(int value)
{
    this->field_0x129 = 1;
    return PlayerCreature_hook_tail(this, value);
}

// ---- update_11cd68.cpp ----
extern "C" void ParticleSystemManager_emitManual(int manager, Vector *emitter, Vector *position,
                                                  int unused, Vector *zero, float value);


void PlayerCreature::update(int elapsed)
{
    Vector zero;
    char workBytes[0x3c];

    int lastHitpoints = this->field_0x13c;
    this->field_0x124 = elapsed;
    int hitpoints = ((Player *)(this->field_0x4))->getHitpoints();
    if (lastHitpoints > hitpoints && this->field_0x88 != 4) {
        this->field_0x128 = 1;
    }

    this->field_0x13c = ((Player *)(this->field_0x4))->getHitpoints();

    if (this->field_0x128 != 0) {
        float elapsedFloat = (float)elapsed;
        int rageTime = this->field_0x130 + elapsed;
        this->field_0x130 = rageTime;
        int endurance = this->field_0x138 - elapsed;
        float rageScale = this->field_0x12c + elapsedFloat * 0.0007f;
        elapsed = (int)(rageScale * elapsedFloat);
        this->field_0x138 = endurance;
        this->field_0x12c = rageScale;

        if (rageTime < 4000) {
            AEGeometry *geometry = this->field_0x8;
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
            this->field_0x130 = 0;
        }

        if (this->field_0x138 < 1) {
            if (this->field_0x129 == 0) {
                int maxEndurance = this->field_0x134;
                this->field_0x130 = 0;
                this->field_0x128 = 0;
                this->field_0x138 = maxEndurance;
            } else {
                this->field_0x128 = 0;
                this->field_0x12a = 1;
            }
        }
    }

    if (((Player *)(this->field_0x4))->getHitpoints() < 1) {
        int state = this->field_0x88;
        if ((uint32_t)(state - 3) >= 2) {
            this->field_0x88 = 3;
            FModSound_play(*PlayerCreature_sound, 0x16, 0, 0, 0.0f);
            ((Player *)(this->field_0x4))->setActive(false);

            void *level = this->field_0x54;
            void *player = Level_getPlayer(level);
            if (*(PlayerCreature **)((char *)(*(void **)((char *)player + 0x14)) + 0x1c) == this) {
                player = Level_getPlayer(level);
                *(PlayerCreature **)((char *)(*(void **)((char *)player + 0x14)) + 0x1c) = 0;
            }

            zero.x = 0.0f;
            zero.y = 0.0f;
            zero.z = 0.0f;
            Vector *position = (Vector *)workBytes;
            position->x = this->field_0x58;
            position->y = this->field_0x5c;
            position->z = this->field_0x60;
            void *levelObject = this->field_0x54;
            ParticleSystemManager_emitManual(*(int *)((char *)levelObject + 0x74),
                                             *(Vector **)((char *)levelObject + 0x34),
                                             position, 0, &zero, -1.0f);
        }
    }

    int state = this->field_0x88;
    if (state == 3) {
        this->field_0x88 = 4;
    } else if (state == 0 && this->field_0x12a == 0) {
        AEGeometry_moveForward(this->field_0x8, (float)elapsed);
    }

    Matrix *matrix = AEGeometry_getMatrix(this->field_0x8);
    Matrix_assign((Matrix *)((char *)this->field_0x4 + 4), matrix);
}

// ---- reset_11cc40.cpp ----

void PlayerCreature::reset()
{
    ((KIPlayer *)((KIPlayer *)this))->reset();
    this->field_0x88 = 0;
    this->field_0x138 = this->field_0x134;
    this->field_0x13c = ((Player *)(this->field_0x4))->getHitpoints();

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

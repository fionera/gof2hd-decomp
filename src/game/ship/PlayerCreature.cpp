#include "gof2/PlayerCreature.h"
#include "gof2/AEGeometry.h"
#include "gof2/FModSound.h"
#include "gof2/Level.h"
#include "gof2/ParticleSystemManager.h"
#include "gof2/KIPlayer.h"
#include "gof2/Player.h"
#include "gof2/PlayerJunk.h"


extern "C" PlayerCreature *KIPlayer_dtor(KIPlayer *self);
extern "C" int PlayerCreature_weightTable[] __attribute__((visibility("hidden")));
extern "C" int PlayerCreature_rageTable[] __attribute__((visibility("hidden")));
extern "C" char PlayerCreature_vtable;
extern "C" int PlayerCreature_enduranceTable[] __attribute__((visibility("hidden")));
void _ZN10PlayerJunk6renderEv(PlayerJunk *self);   // PlayerJunk::render() (base render)
void *ParticleSystemManager_emitManual_v(
    void *self, int handle, const float *pos, void *ret, const float *vel, float p5);
namespace AbyssEngine { namespace AEMath {
Matrix operator*(const Matrix &, const Matrix &);
Matrix MatrixSetRotation(Matrix &, float, float, float);
} }
namespace AbyssEngine { namespace AERandom { int nextInt(int rng, int bound); } }
extern "C" int *PlayerCreature_randomMax __attribute__((visibility("hidden")));
extern "C" FModSound **PlayerCreature_sound __attribute__((visibility("hidden")));

// ---- isHooked_11cd38.cpp ----

uint8_t PlayerCreature::isHooked()
{
    return this->hooked;
}

// ---- calmDown_11cc28.cpp ----

void PlayerCreature::calmDown()
{
    int maxEndurance = this->maxEndurance;
    this->rageTimer = 0;
    this->raging = 0;
    this->endurance = maxEndurance;
}

// ---- unhook_11cd4e.cpp ----

void PlayerCreature::unhook()
{
    int maxEndurance = this->maxEndurance;
    this->caught = 0;
    this->rageTimer = 0;
    this->raging = 0;
    this->endurance = maxEndurance;
}

// ---- render_11cfb8.cpp ----

void PlayerCreature::render()
{
    AEGeometry *geometry = this->renderGeometry;
    if (geometry != 0) {
        ((AEGeometry *)(geometry))->render();
    }
    if ((uint32_t)(this->state - 3) >= 2) {
        return ((PlayerCreature *)(this))->render_tail();
    }
}

// ---- recovered tail-call fragments -----------------------------------------
// PlayerCreature derives from PlayerJunk (-> KIPlayer). render()/hook()/the
// deleting destructor each peel their final tail-branch out into one of these
// helpers; the branch targets were resolved from the PLT veneers in the binary.

// render() tail: once the creature's own geometry has been drawn (and it is not
// in the dying states 3/4), render through the PlayerJunk base.
void PlayerCreature::render_tail()
{
    _ZN10PlayerJunk6renderEv((PlayerJunk *)this);
}

// hook() tail: hooking the creature also enrages it, so forward the hook amount
// into rage().
void PlayerCreature::hook_tail(int value)
{
    this->rage(value);
}

// deleting-destructor tail: the KIPlayer base subobject has already been torn
// down, so just release the storage.
void PlayerCreature::dtor_tail()
{
    ::operator delete(this);
}

// ---- getEndurance_11d004.cpp ----

int PlayerCreature::getEndurance()
{
    return this->endurance;
}

// ---- _PlayerCreature_11ccd8.cpp ----
void _ZN14PlayerCreatureD1Ev(PlayerCreature *self)
{
    ((PlayerCreature *)(KIPlayer_dtor((KIPlayer *)self)))->dtor_tail();
}

// ---- getWeight_11cfe4.cpp ----

int PlayerCreature::getWeight()
{
    return PlayerCreature_weightTable[this->creatureType];
}

// ---- isCaught_11cd3e.cpp ----

uint8_t PlayerCreature::isCaught()
{
    return this->caught;
}

// ---- rage_11cce8.cpp ----

void PlayerCreature::rage(int amount)
{
    float amountScale = (float)amount / -100.0f;
    float typeScale = (float)PlayerCreature_rageTable[this->creatureType];
    this->rageTimer = 0;
    this->raging = 0x101;
    this->rageScale = (amountScale + 1.0f) * (typeScale + 1.0f);
}

// ---- getMaxEndurance_11cffe.cpp ----

int PlayerCreature::getMaxEndurance()
{
    return this->maxEndurance;
}

// ---- getItemIndex_11cff8.cpp ----

int PlayerCreature::getItemIndex()
{
    return this->itemIndex;
}

// ---- PlayerCreature_11cb5c.cpp ----


PlayerCreature::PlayerCreature(int kind, int itemIndex, Player *player, AEGeometry *geometry,
                               float x, float y, float z)
{
    this->vtable = &PlayerCreature_vtable + 8;
    ((Matrix *)((char *)this + 0x144))->initIdentity();

    Player *playerObject = this->player;
    int oneBits = 0x3f800000;
    this->caught = 0;
    this->itemIndex = itemIndex;
    this->rageScale = oneBits;
    this->rageTimer = 0;
    this->raging = 0;
    int previousMaxEndurance = this->maxEndurance;
    this->endurance = previousMaxEndurance;

    int hitpoints = ((Player *)(playerObject))->getHitpoints();
    int endurance = (int)(((float)PlayerCreature_enduranceTable[kind] / 10.0f) * 8000.0f) + 8000;
    this->maxEndurance = endurance;
    this->endurance = endurance;
    this->lastHitpoints = hitpoints;
    reset();
}

// ---- hook_11cd44.cpp ----

void PlayerCreature::hook(int value)
{
    this->hooked = 1;
    return ((PlayerCreature *)(this))->hook_tail(value);
}

// ---- update_11cd68.cpp ----


void PlayerCreature::update(int elapsed)
{
    Vector zero;
    char workBytes[0x3c];

    int lastHitpoints = this->lastHitpoints;
    this->lastElapsed = elapsed;
    int hitpoints = ((Player *)(this->player))->getHitpoints();
    if (lastHitpoints > hitpoints && this->state != 4) {
        this->raging = 1;
    }

    this->lastHitpoints = ((Player *)(this->player))->getHitpoints();

    if (this->raging != 0) {
        float elapsedFloat = (float)elapsed;
        int rageTime = this->rageTimer + elapsed;
        this->rageTimer = rageTime;
        int endurance = this->endurance - elapsed;
        float rageScale = this->rageScale + elapsedFloat * 0.0007f;
        elapsed = (int)(rageScale * elapsedFloat);
        this->endurance = endurance;
        this->rageScale = rageScale;

        if (rageTime < 4000) {
            AEGeometry *geometry = this->geometry;
            Matrix *matrix = &((AEGeometry *)(geometry))->getMatrix();
            *(Matrix *)(workBytes) = *(const Matrix *)(matrix) * *(const Matrix *)((char *)this + 0x144);
            ((AEGeometry *)geometry)->setMatrix(*(const AbyssEngine::AEMath::Matrix *)((Matrix *)workBytes));
        } else {
            int half = elapsed >> 1;
            float negativeHalf = (float)half * -0.5f;
            int *randomMax = PlayerCreature_randomMax;
            int first = AbyssEngine::AERandom::nextInt(*randomMax, half);
            float firstFloat = (float)first;
            int second = AbyssEngine::AERandom::nextInt(*randomMax, half);
            float secondFloat = (float)second;
            float x = ((negativeHalf + firstFloat) * 0.000244140625f) * 2.0f * 3.1415927f;
            float z = ((negativeHalf + secondFloat) * 0.000244140625f) * 2.0f * 3.1415927f;
            *(Matrix *)(workBytes) = AbyssEngine::AEMath::MatrixSetRotation(*(Matrix *)((char *)this + 0x144), (float)(x), (float)(z), (float)0.0f);
            this->rageTimer = 0;
        }

        if (this->endurance < 1) {
            if (this->hooked == 0) {
                int maxEndurance = this->maxEndurance;
                this->rageTimer = 0;
                this->raging = 0;
                this->endurance = maxEndurance;
            } else {
                this->raging = 0;
                this->caught = 1;
            }
        }
    }

    if (((Player *)(this->player))->getHitpoints() < 1) {
        int state = this->state;
        if ((uint32_t)(state - 3) >= 2) {
            this->state = 3;
            ((FModSound *)(*PlayerCreature_sound))->play(0x16, 0, 0, 0.0f);
            ((Player *)(this->player))->setActive(false);

            void *level = this->level;
            void *player = (void *)(intptr_t)((Level *)(level))->getPlayer();
            if (*(PlayerCreature **)((char *)(*(void **)((char *)player + 0x14)) + 0x1c) == this) {
                player = (void *)(intptr_t)((Level *)(level))->getPlayer();
                *(PlayerCreature **)((char *)(*(void **)((char *)player + 0x14)) + 0x1c) = 0;
            }

            zero.x = 0.0f;
            zero.y = 0.0f;
            zero.z = 0.0f;
            Vector *position = (Vector *)workBytes;
            position->x = this->posX;
            position->y = this->posY;
            position->z = this->posZ;
            void *levelObject = this->level;
            ParticleSystemManager_emitManual_v((void *)(intptr_t)(*(int *)((char *)levelObject + 0x74)), (int)(intptr_t)(*(Vector **)((char *)levelObject + 0x34)), (const float *)position, 0, (const float *)&zero, 0.0f);
        }
    }

    int state = this->state;
    if (state == 3) {
        this->state = 4;
    } else if (state == 0 && this->caught == 0) {
        ((AEGeometry *)(this->geometry))->moveForward((float)elapsed);
    }

    Matrix *matrix = &((AEGeometry *)(this->geometry))->getMatrix();
    *(Matrix *)((char *)this->player + 4) = *(const Matrix *)(matrix);
}

// ---- reset_11cc40.cpp ----

void PlayerCreature::reset()
{
    ((KIPlayer *)((KIPlayer *)this))->reset();
    this->state = 0;
    this->endurance = this->maxEndurance;
    this->lastHitpoints = ((Player *)(this->player))->getHitpoints();

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
    *(Matrix *)((char *)this + 0x144) = *(const Matrix *)(matrix);
}

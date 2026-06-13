#include <new>
#include <cstdint>
#include "gof2/game/mission/Objective.h"
#include "gof2/game/world/Level.h"
#include "gof2/game/world/Route.h"
#include "gof2/game/ship/KIPlayer.h"
#include "gof2/game/ship/Player.h"
// RadioMessage.h re-declares incompatible mini Level/Player/Objective structs,
// so it cannot be pulled in here; forward-declare just the predicate we call.
struct RadioMessage { uint8_t isOver(); };
// NOTE: gof2/Level.h is intentionally NOT included: its 32-bit-target static_asserts
// do not hold on a 64-bit host. Level is opaque here; the few direct field reads on a
// Level* (offsets 0x20/0x24) use byte-offset casts, and everything else goes through the
// extern "C" Level_* accessors declared in Objective.h.

Objective::Objective(int type, int value, Level *level)
{
    this->type = type;
    this->value = value;
    this->calcValue = 0;
    this->level = level;
    this->children = 0;
    this->achievedText = 0;
    this->field_0x18 = 0;
}

Objective *Objective::addObjective(Objective *objective)
{
    Array<Objective *> *children = this->children;
    if (children == 0) {
        children = new Array<Objective *>();
        this->children = children;
    }
    children->push_back(objective);
    return this;
}

Objective::Objective(int type, int value, int calcValue, Level *level)
{
    this->type = type;
    this->value = value;
    this->calcValue = calcValue;
    this->level = level;
    this->children = 0;
    this->achievedText = 0;
    if (type == 0xd)
        this->field_0x18 = calcValue;
}

bool Objective::isSurvivalObjective()
{
    return this->type == 3;
}

bool Objective::getCalcValue()
{
    return this->type == 3;
}

Objective *_ZN9ObjectiveD2Ev(Objective *self)
{
    Array<Objective *> *children = self->children;
    if (children != 0) {
        for (Objective *child : *children)
            delete child;
        children->clear();
        delete children;
    }
    self->children = 0;

    String *text = self->achievedText;
    if (text != 0) {
        typedef void (*ReleaseFn)(void *);
        ReleaseFn release = *(ReleaseFn *)((char *)*(void **)text + 4);
        release(text);
    }
    self->achievedText = 0;
    return self;
}

// ---- Objective::~Objective() ----
// Real destructor (the "Objective_dtor" target): releases the child-objective array and
// the achieved-text String, mirroring the D2Ev teardown above.
Objective::~Objective()
{
    Array<Objective *> *children = this->children;
    if (children != 0) {
        for (Objective *child : *children)
            delete child;
        children->clear();
        delete children;
    }
    this->children = 0;

    String *text = this->achievedText;
    if (text != 0) {
        typedef void (*ReleaseFn)(void *);
        ReleaseFn release = *(ReleaseFn *)((char *)*(void **)text + 4);
        release(text);
    }
    this->achievedText = 0;
}

void _ZN9Objective15setAchievedTextEPN11AbyssEngine6StringE(Objective *self,
                                                                        String *text)
{
    String *copy = (String *)::operator new(sizeof(String));
    ((String *)(copy))->ctor_copy(text, false);
    self->achievedText = copy;
}

unsigned int Objective::achieved(int value)
{
    Array<KIPlayer *> *enemies = ((Array<KIPlayer *> *)((Level *)(this->level))->getEnemies());
    unsigned int result = 0;

    switch (this->type) {
    case 0:
        result = ((Level *)(this->level))->getEnemiesLeft() == 0;
        break;
    case 1:
        return Objective_tail_enemy(
            ((Array<KIPlayer *> *)((Level *)(this->level))->getEnemies())->data()[this->value]);
    case 2:
        result = *(uint8_t *)((char *)((Route *)(((Level *)(this->level))->getPlayerRoute()))->getLastWaypoint() + 0x130) != 0;
        break;
    case 3:
        result = this->value < value;
        break;
    case 4:
        return Objective_tail_message(((Array<void *> *)((Level *)(this->level))->getMessages())->data()[this->value]);
    case 5:
        result = ((Level *)(this->level))->getFriendsLeft() == 0;
        break;
    case 7: {
        result = 0;
        int i = 0;
        while (i < this->value) {
            result += ((KIPlayer *)(enemies->data()[i]))->isDead();
            i++;
        }
        result = (int)result == this->value;
        break;
    }
    case 8: {
        Array<KIPlayer *> *asteroids = ((Array<KIPlayer *> *)((Level *)(this->level))->getAsteroids());
        result = 0;
        uint32_t i = 0;
        while (i < asteroids->size()) {
            result += ((KIPlayer *)(asteroids->data()[i]))->isDead();
            i++;
        }
        result = (int)result > this->value;
        break;
    }
    case 9: {
        Array<KIPlayer *> *asteroids = ((Array<KIPlayer *> *)((Level *)(this->level))->getAsteroids());
        uint32_t i = 0;
        while (i < asteroids->size()) {
            int limit = this->value;
            if ((int)i >= limit)
                return limit < 1;
            i++;
        }
    }
        [[fallthrough]];
    case 10: {
        Array<KIPlayer *> *asteroids = ((Array<KIPlayer *> *)((Level *)(this->level))->getAsteroids());
        uint32_t i = 0;
        while (i < asteroids->size()) {
            if ((int)i >= this->value)
                return 0;
            i++;
        }
    }
        [[fallthrough]];
    case 11:
        result = ((Array<KIPlayer *> *)((Level *)(this->level))->getEnemies())->data()[this->value]->field_0x69 != 0;
        break;
    case 12:
        result = ((Array<KIPlayer *> *)((Level *)(this->level))->getEnemies())->data()[this->value]->field_0x68 != 0;
        break;
    case 15:
        return Objective_tail_enemy_payload(
            ((Array<KIPlayer *> *)((Level *)(this->level))->getEnemies())->data()[this->value]->player);
    case 16: {
        uint32_t i = 0;
        while (i < enemies->size()) {
            KIPlayer *enemy = enemies->data()[i++];
            result = 0;
            if (enemy->field_0x69 == 0)
                return result;
        }
        return 1;
    }
    case 17: {
        uint32_t i = 0;
        while (i < enemies->size()) {
            KIPlayer *enemy = enemies->data()[i++];
            result = 1;
            if (enemy->field_0x68 != 0)
                return result;
        }
        return 0;
    }
    case 18: {
        result = 0;
        int i = this->value;
        while (i < this->calcValue) {
            result += ((KIPlayer *)(enemies->data()[i]))->isDead();
            i++;
        }
        result = (int)result == this->calcValue - this->value;
        break;
    }
    case 19:
        return Objective_tail_level(this->level);
    case 20:
    case 21: {
        result = 0;
        int i = this->value;
        while (i < this->calcValue) {
            KIPlayer *enemy = enemies->data()[i];
            if (((KIPlayer *)(enemy))->isDead() != 0 && enemy->shipGroup == 8)
                result++;
            i++;
        }
        int count = this->calcValue - this->value;
        if ((int)result != count)
            return 0;
        Level *level = this->level;
        int field20 = *(int *)((char *)level + 0x20);
        int field24 = *(int *)((char *)level + 0x24);
        if (this->type != 0x14)
            return field24 <= field20;
        result = field24 > field20;
        break;
    }
    case 22: {
        Array<void *> *messages = ((Array<void *> *)((Level *)(this->level))->getMessages());
        Array<void *> *messages2 = ((Array<void *> *)((Level *)(this->level))->getMessages());
        return Objective_tail_message(messages->data()[messages2->size() - 1]);
    }
    case 23:
        result = (uint8_t)((Array<KIPlayer *> *)((Level *)(this->level))->getEnemies())->data()[this->value]->field_0x24 != 0;
        break;
    case 25:
        result = *(float *)&((Array<KIPlayer *> *)((Level *)(this->level))->getEnemies())->data()[this->value]->field_0x64 == 0.0f;
        break;
    case 26: {
        int i = this->value;
        while (i < this->calcValue) {
            int dead = ((KIPlayer *)(enemies->data()[i]))->isDead();
            i++;
            result = 1;
            if (dead != 0)
                return result;
        }
        return 0;
    }
    case 27:
        result = ((Level *)(this->level))->getEnemiesLeft() < this->value;
        break;
    case 28:
        result = ((Level *)(this->level))->getNumDeliveredOre() >= this->value;
        break;
    case 29:
        result = ((Level *)(this->level))->getNumDeliveredPassengers() >= this->value;
        break;
    case 30:
        return Objective_tail_enemy_final(
            ((Array<KIPlayer *> *)((Level *)(this->level))->getEnemies())->data()[this->value]);
    default:
        break;
    }
    return result;
}

// ---- achieved() tail-call helpers ----
// Recovered from the indirect tail-calls in Objective::achieved: each loads a
// single target object and forwards to one virtual predicate on it. The returned
// boolean predicate becomes achieved()'s result for that objective type.

unsigned int Objective::tailEnemyIsDead(KIPlayer *enemy)
{
    return enemy->isDead();
}

unsigned int Objective::tailEnemyPayloadActive(void *player)
{
    return ((Player *)player)->isActive();
}

unsigned int Objective::tailFriendCargoStolen(Level *level)
{
    return ((Level *)level)->friendCargoWasStolen();
}

unsigned int Objective::tailMessageOver(void *message)
{
    return ((RadioMessage *)message)->isOver();
}

unsigned int Objective::tailEnemyIsDying(KIPlayer *enemy)
{
    return enemy->isDying();
}

// Free-function entries: callers that hold the Objective storage opaquely
// placement-construct via the (type, value, calcValue, level) ctor, and free it
// through the in-place destructor (which returns the pointer for ::operator delete).
//   o    : storage to construct the Objective into
//   type : objective type, value/calcValue : its parameters, level : owning Level
extern "C" void Objective_ctor_akw(int o, int type, int value, int calcValue, Level *level)
{
    new ((void *)(intptr_t)o) Objective(type, value, calcValue, level);
}

extern "C" void *Objective_dtor(Objective *o)
{
    o->~Objective();
    return o;
}

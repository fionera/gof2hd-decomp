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
    this->field_0x0 = type;
    this->field_0x4 = value;
    this->field_0x8 = 0;
    this->field_0xc = level;
    this->field_0x10 = 0;
    this->field_0x14 = 0;
    this->field_0x18 = 0;
}

Objective *Objective::addObjective(Objective *objective)
{
    Array<Objective *> *children = this->field_0x10;
    if (children == 0) {
        children = new Array<Objective *>();
        this->field_0x10 = children;
    }
    children->push_back(objective);
    return this;
}

Objective::Objective(int type, int value, int calcValue, Level *level)
{
    this->field_0x0 = type;
    this->field_0x4 = value;
    this->field_0x8 = calcValue;
    this->field_0xc = level;
    this->field_0x10 = 0;
    this->field_0x14 = 0;
    if (type == 0xd)
        this->field_0x18 = calcValue;
}

bool Objective::isSurvivalObjective()
{
    return this->field_0x0 == 3;
}

bool Objective::getCalcValue()
{
    return this->field_0x0 == 3;
}

Objective *_ZN9ObjectiveD2Ev(Objective *self)
{
    Array<Objective *> *children = self->field_0x10;
    if (children != 0) {
        for (Objective *child : *children)
            delete child;
        children->clear();
        delete children;
    }
    self->field_0x10 = 0;

    void *text = self->field_0x14;
    if (text != 0) {
        typedef void (*ReleaseFn)(void *);
        ReleaseFn release = *(ReleaseFn *)((char *)*(void **)text + 4);
        release(text);
    }
    self->field_0x14 = 0;
    return self;
}

// ---- Objective::~Objective() ----
// Real destructor (the "Objective_dtor" target): releases the child-objective array and
// the achieved-text String, mirroring the D2Ev teardown above.
Objective::~Objective()
{
    Array<Objective *> *children = this->field_0x10;
    if (children != 0) {
        for (Objective *child : *children)
            delete child;
        children->clear();
        delete children;
    }
    this->field_0x10 = 0;

    void *text = this->field_0x14;
    if (text != 0) {
        typedef void (*ReleaseFn)(void *);
        ReleaseFn release = *(ReleaseFn *)((char *)*(void **)text + 4);
        release(text);
    }
    this->field_0x14 = 0;
}

void _ZN9Objective15setAchievedTextEPN11AbyssEngine6StringE(Objective *self,
                                                                        String *text)
{
    String *copy = (String *)::operator new(sizeof(String));
    ((String *)(copy))->ctor_copy(text, false);
    self->field_0x14 = copy;
}

unsigned int Objective::achieved(int value)
{
    Array<KIPlayer *> *enemies = ((Array<KIPlayer *> *)((Level *)(this->field_0xc))->getEnemies());
    unsigned int result = 0;

    switch (this->field_0x0) {
    case 0:
        result = ((Level *)(this->field_0xc))->getEnemiesLeft() == 0;
        break;
    case 1:
        return Objective_tail_enemy(
            ((Array<KIPlayer *> *)((Level *)(this->field_0xc))->getEnemies())->data()[this->field_0x4]);
    case 2:
        result = *(uint8_t *)((char *)((Route *)(((Level *)(this->field_0xc))->getPlayerRoute()))->getLastWaypoint() + 0x130) != 0;
        break;
    case 3:
        result = this->field_0x4 < value;
        break;
    case 4:
        return Objective_tail_message(((Array<void *> *)((Level *)(this->field_0xc))->getMessages())->data()[this->field_0x4]);
    case 5:
        result = ((Level *)(this->field_0xc))->getFriendsLeft() == 0;
        break;
    case 7: {
        result = 0;
        int i = 0;
        while (i < this->field_0x4) {
            result += ((KIPlayer *)(enemies->data()[i]))->isDead();
            i++;
        }
        result = (int)result == this->field_0x4;
        break;
    }
    case 8: {
        Array<KIPlayer *> *asteroids = ((Array<KIPlayer *> *)((Level *)(this->field_0xc))->getAsteroids());
        result = 0;
        uint32_t i = 0;
        while (i < asteroids->size()) {
            result += ((KIPlayer *)(asteroids->data()[i]))->isDead();
            i++;
        }
        result = (int)result > this->field_0x4;
        break;
    }
    case 9: {
        Array<KIPlayer *> *asteroids = ((Array<KIPlayer *> *)((Level *)(this->field_0xc))->getAsteroids());
        uint32_t i = 0;
        while (i < asteroids->size()) {
            int limit = this->field_0x4;
            if ((int)i >= limit)
                return limit < 1;
            i++;
        }
    }
        [[fallthrough]];
    case 10: {
        Array<KIPlayer *> *asteroids = ((Array<KIPlayer *> *)((Level *)(this->field_0xc))->getAsteroids());
        uint32_t i = 0;
        while (i < asteroids->size()) {
            if ((int)i >= this->field_0x4)
                return 0;
            i++;
        }
    }
        [[fallthrough]];
    case 11:
        result = ((Array<KIPlayer *> *)((Level *)(this->field_0xc))->getEnemies())->data()[this->field_0x4]->field_0x69 != 0;
        break;
    case 12:
        result = ((Array<KIPlayer *> *)((Level *)(this->field_0xc))->getEnemies())->data()[this->field_0x4]->field_0x68 != 0;
        break;
    case 15:
        return Objective_tail_enemy_payload(
            ((Array<KIPlayer *> *)((Level *)(this->field_0xc))->getEnemies())->data()[this->field_0x4]->player);
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
        int i = this->field_0x4;
        while (i < this->field_0x8) {
            result += ((KIPlayer *)(enemies->data()[i]))->isDead();
            i++;
        }
        result = (int)result == this->field_0x8 - this->field_0x4;
        break;
    }
    case 19:
        return Objective_tail_level(this->field_0xc);
    case 20:
    case 21: {
        result = 0;
        int i = this->field_0x4;
        while (i < this->field_0x8) {
            KIPlayer *enemy = enemies->data()[i];
            if (((KIPlayer *)(enemy))->isDead() != 0 && enemy->shipGroup == 8)
                result++;
            i++;
        }
        int count = this->field_0x8 - this->field_0x4;
        if ((int)result != count)
            return 0;
        Level *level = this->field_0xc;
        int field20 = *(int *)((char *)level + 0x20);
        int field24 = *(int *)((char *)level + 0x24);
        if (this->field_0x0 != 0x14)
            return field24 <= field20;
        result = field24 > field20;
        break;
    }
    case 22: {
        Array<void *> *messages = ((Array<void *> *)((Level *)(this->field_0xc))->getMessages());
        Array<void *> *messages2 = ((Array<void *> *)((Level *)(this->field_0xc))->getMessages());
        return Objective_tail_message(messages->data()[messages2->size() - 1]);
    }
    case 23:
        result = *(uint8_t *)((char *)((Array<KIPlayer *> *)((Level *)(this->field_0xc))->getEnemies())->data()[this->field_0x4] + 0x24) != 0;
        break;
    case 25:
        result = *(float *)((char *)((Array<KIPlayer *> *)((Level *)(this->field_0xc))->getEnemies())->data()[this->field_0x4] + 0x64) == 0.0f;
        break;
    case 26: {
        int i = this->field_0x4;
        while (i < this->field_0x8) {
            int dead = ((KIPlayer *)(enemies->data()[i]))->isDead();
            i++;
            result = 1;
            if (dead != 0)
                return result;
        }
        return 0;
    }
    case 27:
        result = ((Level *)(this->field_0xc))->getEnemiesLeft() < this->field_0x4;
        break;
    case 28:
        result = ((Level *)(this->field_0xc))->getNumDeliveredOre() >= this->field_0x4;
        break;
    case 29:
        result = ((Level *)(this->field_0xc))->getNumDeliveredPassengers() >= this->field_0x4;
        break;
    case 30:
        return Objective_tail_enemy_final(
            ((Array<KIPlayer *> *)((Level *)(this->field_0xc))->getEnemies())->data()[this->field_0x4]);
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

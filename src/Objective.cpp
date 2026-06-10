#include "gof2/Objective.h"
#include "gof2/KIPlayer.h"
// NOTE: gof2/Level.h is intentionally NOT included: its 32-bit-target static_asserts
// do not hold on a 64-bit host. Level is opaque here; the few direct field reads on a
// Level* (offsets 0x20/0x24) use byte-offset casts, and everything else goes through the
// extern "C" Level_* accessors declared in Objective.h.

extern "C" void *operator_new(unsigned int size);
extern "C" String *String_ctor_string(String *self, String *text, bool copy);

// ---- Objective_97bfc.cpp ----
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

// ---- addObjective_97cc2.cpp ----
Objective *Objective::addObjective(Objective *objective)
{
    Array<Objective *> *children = this->field_0x10;
    if (children == 0) {
        children = (Array<Objective *> *)operator new(sizeof(Array<Objective *>));
        Array_ObjectivePtr_ctor(children);
        this->field_0x10 = children;
    }
    ArrayAdd_Objective(objective, children);
    return this;
}

// ---- Objective_97c0e.cpp ----
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

// ---- isSurvivalObjective_980c0.cpp ----
bool Objective::isSurvivalObjective()
{
    return this->field_0x0 == 3;
}

// ---- getCalcValue_980b4.cpp ----
bool Objective::getCalcValue()
{
    return this->field_0x0 == 3;
}

// ---- _Objective_97c2a.cpp ----
extern "C" Objective *_ZN9ObjectiveD2Ev(Objective *self)
{
    Array<Objective *> *children = self->field_0x10;
    if (children != 0) {
        ArrayReleaseClasses_Objective(children);
        children = self->field_0x10;
        if (children != 0)
            operator_delete(Array_ObjectivePtr_dtor(children));
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

// ---- setAchievedText_980cc.cpp ----
extern "C" void _ZN9Objective15setAchievedTextEPN11AbyssEngine6StringE(Objective *self,
                                                                        String *text)
{
    String *copy = (String *)operator_new(sizeof(String));
    String_ctor_string(copy, text, false);
    self->field_0x14 = copy;
}

// ---- achieved_97d40.cpp ----
unsigned int Objective::achieved(int value)
{
    Array<KIPlayer *> *enemies = Level_getEnemies(this->field_0xc);
    unsigned int result = 0;

    switch (this->field_0x0) {
    case 0:
        result = Level_getEnemiesLeft(this->field_0xc) == 0;
        break;
    case 1:
        return Objective_tail_enemy(
            Level_getEnemies(this->field_0xc)->data()[this->field_0x4]);
    case 2:
        result = *(uint8_t *)((char *)Route_getLastWaypoint(Level_getPlayerRoute(this->field_0xc)) + 0x130) != 0;
        break;
    case 3:
        result = this->field_0x4 < value;
        break;
    case 4:
        return Objective_tail_message(Level_getMessages(this->field_0xc)->data()[this->field_0x4]);
    case 5:
        result = Level_getFriendsLeft(this->field_0xc) == 0;
        break;
    case 7: {
        result = 0;
        int i = 0;
        while (i < this->field_0x4) {
            result += KIPlayer_isDead(enemies->data()[i]);
            i++;
        }
        result = (int)result == this->field_0x4;
        break;
    }
    case 8: {
        Array<KIPlayer *> *asteroids = Level_getAsteroids(this->field_0xc);
        result = 0;
        uint32_t i = 0;
        while (i < asteroids->size()) {
            result += KIPlayer_isDead(asteroids->data()[i]);
            i++;
        }
        result = (int)result > this->field_0x4;
        break;
    }
    case 9: {
        Array<KIPlayer *> *asteroids = Level_getAsteroids(this->field_0xc);
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
        Array<KIPlayer *> *asteroids = Level_getAsteroids(this->field_0xc);
        uint32_t i = 0;
        while (i < asteroids->size()) {
            if ((int)i >= this->field_0x4)
                return 0;
            i++;
        }
    }
        [[fallthrough]];
    case 11:
        result = Level_getEnemies(this->field_0xc)->data()[this->field_0x4]->field_0x69 != 0;
        break;
    case 12:
        result = Level_getEnemies(this->field_0xc)->data()[this->field_0x4]->field_0x68 != 0;
        break;
    case 15:
        return Objective_tail_enemy_payload(
            Level_getEnemies(this->field_0xc)->data()[this->field_0x4]->field_0x4);
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
            result += KIPlayer_isDead(enemies->data()[i]);
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
            if (KIPlayer_isDead(enemy) != 0 && enemy->field_0x28 == 8)
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
        Array<void *> *messages = Level_getMessages(this->field_0xc);
        Array<void *> *messages2 = Level_getMessages(this->field_0xc);
        return Objective_tail_message(messages->data()[messages2->size() - 1]);
    }
    case 23:
        result = *(uint8_t *)((char *)Level_getEnemies(this->field_0xc)->data()[this->field_0x4] + 0x24) != 0;
        break;
    case 25:
        result = *(float *)((char *)Level_getEnemies(this->field_0xc)->data()[this->field_0x4] + 0x64) == 0.0f;
        break;
    case 26: {
        int i = this->field_0x4;
        while (i < this->field_0x8) {
            int dead = KIPlayer_isDead(enemies->data()[i]);
            i++;
            result = 1;
            if (dead != 0)
                return result;
        }
        return 0;
    }
    case 27:
        result = Level_getEnemiesLeft(this->field_0xc) < this->field_0x4;
        break;
    case 28:
        result = Level_getNumDeliveredOre(this->field_0xc) >= this->field_0x4;
        break;
    case 29:
        result = Level_getNumDeliveredPassengers(this->field_0xc) >= this->field_0x4;
        break;
    case 30:
        return Objective_tail_enemy_final(
            Level_getEnemies(this->field_0xc)->data()[this->field_0x4]);
    default:
        break;
    }
    return result;
}

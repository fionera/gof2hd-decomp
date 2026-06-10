#include "gof2/RadioMessage.h"




// ---- finish_151682.cpp ----
void RadioMessage::finish()
{
    field<uint8_t>(this, 0x21) = 1;
}

// ---- isOver_15167c.cpp ----
uint8_t RadioMessage::isOver()
{
    return field<uint8_t>(this, 0x21);
}

// ---- reset_1515bc.cpp ----
void RadioMessage::reset()
{
    volatile int *timer = &field<int>(this, 0x24);
    volatile uint16_t *flags = &field<uint16_t>(this, 0x20);
    *timer = 0;
    *flags = 0;
}

// ---- RadioMessage_1515c4.cpp ----
RadioMessage::RadioMessage(int param_1, int param_2, int param_3, int param_4, int param_5)
{
    field<int>(this, 0x1c) = 0;
    field<int>(this, 0x00) = 0;
    field<int>(this, 0x04) = 0;
    field<int>(this, 0x08) = param_1;
    field<int>(this, 0x0c) = param_2;
    field<int>(this, 0x10) = param_3;
    field<int>(this, 0x14) = param_4;

    int *values = new int[param_5];
    field<int *>(this, 0x1c) = values;
    for (int i = 0; i < param_5; ++i) {
        values[i] = param_4 + i;
    }

    volatile int *count = &field<int>(this, 0x18);
    volatile uint16_t *flags = &field<uint16_t>(this, 0x20);
    volatile int *timer = &field<int>(this, 0x24);
    *count = param_5;
    *flags = 0;
    *timer = 0;
}

// ---- trigger_15168a.cpp ----
void RadioMessage::trigger()
{
    field<uint8_t>(this, 0x20) = 1;
}

// ---- RadioMessage_15162a.cpp ----
RadioMessage::RadioMessage(int param_1, int param_2, Objective *param_3)
{
    field<int>(this, 0x1c) = 0;
    field<int>(this, 0x00) = 0;
    field<Objective *>(this, 0x04) = param_3;
    field<int>(this, 0x08) = param_1;
    field<int>(this, 0x0c) = param_2;
    field<int>(this, 0x10) = 0xb;
    field<uint16_t>(this, 0x20) = 0;
    field<int>(this, 0x24) = 0;
}

// ---- isTriggered_151676.cpp ----
uint8_t RadioMessage::isTriggered()
{
    return field<uint8_t>(this, 0x20);
}

// ---- _RadioMessage_151650.cpp ----
RadioMessage::~RadioMessage()
{
    void *values = field<void *>(this, 0x1c);
    if (values != 0) {
        operator_delete__(values);
    }
    field<void *>(this, 0x1c) = 0;
}

// ---- triggered_151694.cpp ----
#define ALWAYS_INLINE __attribute__((always_inline)) inline

static ALWAYS_INLINE Radio *radio(RadioMessage *self)
{
    return field<Radio *>(self, 0x00);
}

static ALWAYS_INLINE Objective *objective(RadioMessage *self)
{
    return field<Objective *>(self, 0x04);
}

static ALWAYS_INLINE int type(RadioMessage *self)
{
    return field<int>(self, 0x10);
}

static ALWAYS_INLINE int param(RadioMessage *self)
{
    return field<int>(self, 0x14);
}

static ALWAYS_INLINE int count(RadioMessage *self)
{
    return field<int>(self, 0x18);
}

static ALWAYS_INLINE int *indices(RadioMessage *self)
{
    return field<int *>(self, 0x1c);
}

static ALWAYS_INLINE Array<Player *> *enemies(PlayerEgo *ego)
{
    return field<Player *>(ego, 0x00)->getEnemies();
}

static ALWAYS_INLINE Player *selected(Array<Player *> *list, RadioMessage *self, int i)
{
    return list->data()[indices(self)[i]];
}

static ALWAYS_INLINE int set_result(RadioMessage *self, int value)
{
    field<uint8_t>(self, 0x20) = static_cast<uint8_t>(value);
    if (value == 0) {
        return 0;
    }
    radio(self)->setCurrentMessage(self);
    return 1;
}

static ALWAYS_INLINE int clear_result(RadioMessage *self)
{
    field<uint8_t>(self, 0x20) = 0;
    return 0;
}

static ALWAYS_INLINE int trigger_result(RadioMessage *self)
{
    field<uint8_t>(self, 0x20) = 1;
    radio(self)->setCurrentMessage(self);
    return 1;
}

static ALWAYS_INLINE int low_hp(Player *player, int divisor, int multiplier)
{
    int max = player->getMaxHitpoints() / divisor;
    return player->getHitpoints() < max * multiplier;
}

static ALWAYS_INLINE float vector_z(Vector *vector)
{
    return *reinterpret_cast<float *>(reinterpret_cast<char *>(vector) + 8);
}

int RadioMessage::triggered(int64_t time, PlayerEgo *ego, LevelScript *script)
{
    if (field<uint8_t>(this, 0x20) != 0) {
        return 0;
    }

    switch (type(this)) {
    case 0: {
        if (ego->getRoute() == 0) {
            break;
        }
        int current = ego->getRoute()->getCurrent();
        int last = field<int>(this, 0x24);
        int value = 0;
        if (current > last) {
            value = last == param(this);
        }
        field<int>(this, 0x24) = ego->getRoute()->getCurrent();
        field<uint8_t>(this, 0x20) = static_cast<uint8_t>(value);
        if (value != 0) {
            radio(this)->setCurrentMessage(this);
            return 1;
        }
        return 0;
    }

    case 1: {
        Array<Player *> *list = enemies(ego);
        for (int i = 0; i < count(this); ++i) {
            if (selected(list, this, i)->isDead()) {
                return trigger_result(this);
            }
        }
        break;
    }

    case 2: {
        Array<Player *> *list = enemies(ego);
        for (int i = 0; i < count(this); ++i) {
            Player *player = selected(list, this, i);
            if (field<uint8_t>(player, 0x5d) != 0 && player->isDead()) {
                return trigger_result(this);
            }
        }
        break;
    }

    case 3:
        return set_result(this, field<Level *>(ego, 0x0c)->getEnemiesLeft() < 1);

    case 4:
        return set_result(this, field<Level *>(ego, 0x0c)->getFriendsLeft() < 1);

    case 5:
        return set_result(this, time >= static_cast<int64_t>(param(this)));

    case 6: {
        RadioMessage *message = radio(this)->getMessage(param(this));
        return set_result(this, field<uint8_t>(message, 0x20));
    }

    case 8: {
        Array<Player *> *list = enemies(ego);
        for (int i = 0; i < count(this); ++i) {
            Player *player = selected(list, this, i);
            if (!player->isAsteroid() && player->isActive()) {
                return trigger_result(this);
            }
        }
        break;
    }

    case 9: {
        Array<Player *> *list = enemies(ego);
        for (int i = 0; i < count(this); ++i) {
            if (!selected(list, this, i)->isDead()) {
                break;
            }
            if (i + 1 >= count(this)) {
                return trigger_result(this);
            }
        }
        break;
    }

    case 10: {
        Array<Player *> *list = enemies(ego);
        for (int i = 0; i < count(this); ++i) {
            Player *player = selected(list, this, i);
            if (!player->isAsteroid() && field<uint8_t>(player, 0x5d) != 0 && player->isActive()) {
                return trigger_result(this);
            }
        }
        break;
    }

    case 0x0b:
        return set_result(this, objective(this)->achieved(static_cast<int>(time)));

    case 0x0c: {
        Array<Player *> *list = enemies(ego);
        for (int i = 0; i < count(this); ++i) {
            if (low_hp(selected(list, this, i), 2, 1)) {
                return trigger_result(this);
            }
        }
        break;
    }

    case 0x0e: {
        Array<Player *> *list = enemies(ego);
        KIPlayer *ki = list->data()[param(this)]->getKIPlayer();
        return set_result(this, field<uint8_t>(ki, 0x69));
    }

    case 0x0f: {
        Array<Player *> *list = enemies(ego);
        for (uint32_t i = 0; i < list->size(); ++i) {
            Player *player = list->data()[i];
            if (!player->isAsteroid() && player->isDead()) {
                return trigger_result(this);
            }
        }
        break;
    }

    case 0x10: {
        Array<Player *> *list = enemies(ego);
        for (uint32_t i = 0; i < list->size(); ++i) {
            Player *player = list->data()[i];
            if (!player->isAsteroid() && player->isActive() && !player->isAlwaysFriend()) {
                return trigger_result(this);
            }
        }
        break;
    }

    case 0x11: {
        Array<Player *> *list = enemies(ego);
        for (uint32_t i = 0; i < list->size(); ++i) {
            if (i == static_cast<uint32_t>(param(this))) {
                continue;
            }
            Player *player = list->data()[i];
            if (player->isAsteroid()) {
                continue;
            }
            if (player->isDead()) {
                break;
            }
        }
        return trigger_result(this);
    }

    case 0x12: {
        Array<Player *> *list = enemies(ego);
        KIPlayer *ki = list->data()[param(this)]->getKIPlayer();
        if (field<uint8_t>(ki, 0x69) != 0) {
            return trigger_result(this);
        }
        ki = list->data()[param(this)]->getKIPlayer();
        return set_result(this, field<uint8_t>(ki, 0x6a));
    }

    case 0x13: {
        Array<Player *> *list = enemies(ego);
        for (int i = 0; i < count(this); ++i) {
            if (low_hp(selected(list, this, i), 4, 1)) {
                return trigger_result(this);
            }
        }
        break;
    }

    case 0x14: {
        Array<Player *> *list = enemies(ego);
        int dead = 0;
        for (uint32_t i = 0; i < list->size(); ++i) {
            Player *player = list->data()[i];
            if (!player->isAsteroid()) {
                dead += player->isDead();
            }
            if (dead >= param(this)) {
                return trigger_result(this);
            }
        }
        break;
    }

    case 0x15: {
        Array<Player *> *list = enemies(ego);
        KIPlayer *ki = list->data()[param(this)]->getKIPlayer();
        return set_result(this, field<uint8_t>(ki, 0x24));
    }

    case 0x16: {
        int value = field<int>(field<Level *>(ego, 0x0c), 0x1c);
        return set_result(this, value >= param(this));
    }

    case 0x17:
        return set_result(this, field<Radar *>(ego, 0x14)->stationLocked());

    case 0x18: {
        Array<Player *> *list = enemies(ego);
        Player *player = list->data()[param(this)];
        if (!player->isActive()) {
            int value = (!player->isDead()) & (time > 0xea5fLL);
            return set_result(this, value);
        }
        break;
    }

    case 0x19: {
        if (ego->getRoute() == 0) {
            break;
        }
        int current = ego->getRoute()->getCurrent();
        int last = field<int>(this, 0x24);
        field<int>(this, 0x24) = ego->getRoute()->getCurrent();
        Array<Player *> *list = enemies(ego);
        int active = 0;
        for (uint32_t i = 0; i < list->size(); ++i) {
            Player *player = list->data()[i];
            if (!player->isAsteroid()) {
                active += player->isDead() ^ 1;
            }
        }
        int value = (active >= param(this)) & (current > last) & (last == 0);
        return set_result(this, value);
    }

    case 0x1a: {
        Array<Player *> *list = enemies(ego);
        Player *player = list->data()[0];
        if (!player->isActive() || player->isDead()) {
            break;
        }
        Vector first;
        first = player->getPosition();
        if (vector_z(&first) - static_cast<float>(param(this)) > 0.0f) {
            Vector second;
            second = player->getPosition();
            return set_result(this, vector_z(&second) - static_cast<float>(param(this)) < 5000.0f);
        }
        Vector second;
        second = player->getPosition();
        return set_result(this, vector_z(&second) - static_cast<float>(param(this)) > -5000.0f);
    }

    case 0x1b: {
        int delta = script->getEvent() - param(this);
        return set_result(this, delta == 0);
    }

    case 0x1c:
        return set_result(this, field<Player *>(ego, 0x00)->getArmorHP() < 1);

    case 0x1e: {
        Array<Player *> *list = enemies(ego);
        int dead = 0;
        for (int i = 2; i != 6; ++i) {
            Player *player = list->data()[i];
            if (!player->isAsteroid()) {
                dead += player->isDead();
            }
        }
        return set_result(this, dead - param(this) == 0);
    }

    case 0x1f: {
        Array<Player *> *list = enemies(ego);
        for (int i = 0; i < count(this); ++i) {
            if (low_hp(selected(list, this, i), 4, 3)) {
                return trigger_result(this);
            }
        }
        break;
    }
    }

    return clear_result(this);
}

// ---- RadioMessage_15158c.cpp ----
RadioMessage::RadioMessage(int param_1, int param_2, int param_3, int param_4)
{
    field<int>(this, 0x1c) = 0;
    field<int>(this, 0x00) = 0;
    field<int>(this, 0x04) = 0;
    field<int>(this, 0x08) = param_1;
    field<int>(this, 0x0c) = param_2;
    field<int>(this, 0x10) = param_3;
    field<int>(this, 0x14) = param_4;

    int *values = new int[1];
    *values = param_4;
    struct CountValues {
        int count;
        int *values;
    };
    field<CountValues>(this, 0x18) = CountValues{1, values};

    volatile uint16_t *flags = &field<uint16_t>(this, 0x20);
    volatile int *timer = &field<int>(this, 0x24);
    *flags = 0;
    *timer = 0;
}

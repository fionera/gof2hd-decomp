#include "class.h"

unsigned int Objective::achieved(int value)
{
    Array<KIPlayer *> *enemies = Level_getEnemies(this->f_c);
    unsigned int result = 0;

    switch (this->f_0) {
    case 0:
        result = Level_getEnemiesLeft(this->f_c) == 0;
        break;
    case 1:
        return Objective_tail_enemy(
            Level_getEnemies(this->f_c)->data[this->f_4]);
    case 2:
        result = G<uint8_t>(Route_getLastWaypoint(Level_getPlayerRoute(this->f_c)), 0x130) != 0;
        break;
    case 3:
        result = this->f_4 < value;
        break;
    case 4:
        return Objective_tail_message(Level_getMessages(this->f_c)->data[this->f_4]);
    case 5:
        result = Level_getFriendsLeft(this->f_c) == 0;
        break;
    case 7: {
        result = 0;
        int i = 0;
        while (i < this->f_4) {
            result += KIPlayer_isDead(enemies->data[i]);
            i++;
        }
        result = (int)result == this->f_4;
        break;
    }
    case 8: {
        Array<KIPlayer *> *asteroids = Level_getAsteroids(this->f_c);
        result = 0;
        uint32_t i = 0;
        while (i < asteroids->length) {
            result += KIPlayer_isDead(asteroids->data[i]);
            i++;
        }
        result = (int)result > this->f_4;
        break;
    }
    case 9: {
        Array<KIPlayer *> *asteroids = Level_getAsteroids(this->f_c);
        uint32_t i = 0;
        while (i < asteroids->length) {
            int limit = this->f_4;
            if ((int)i >= limit)
                return limit < 1;
            i++;
        }
    }
        [[fallthrough]];
    case 10: {
        Array<KIPlayer *> *asteroids = Level_getAsteroids(this->f_c);
        uint32_t i = 0;
        while (i < asteroids->length) {
            if ((int)i >= this->f_4)
                return 0;
            i++;
        }
    }
        [[fallthrough]];
    case 11:
        result = G<uint8_t>(
                     Level_getEnemies(this->f_c)->data[this->f_4],
                     0x69) != 0;
        break;
    case 12:
        result = G<uint8_t>(
                     Level_getEnemies(this->f_c)->data[this->f_4],
                     0x68) != 0;
        break;
    case 15:
        return Objective_tail_enemy_payload(
            G<void *>(Level_getEnemies(this->f_c)->data[this->f_4],
                      0x4));
    case 16: {
        uint32_t i = 0;
        while (i < enemies->length) {
            KIPlayer *enemy = enemies->data[i++];
            result = 0;
            if (G<uint8_t>(enemy, 0x69) == 0)
                return result;
        }
        return 1;
    }
    case 17: {
        uint32_t i = 0;
        while (i < enemies->length) {
            KIPlayer *enemy = enemies->data[i++];
            result = 1;
            if (G<uint8_t>(enemy, 0x68) != 0)
                return result;
        }
        return 0;
    }
    case 18: {
        result = 0;
        int i = this->f_4;
        while (i < this->f_8) {
            result += KIPlayer_isDead(enemies->data[i]);
            i++;
        }
        result = (int)result == this->f_8 - this->f_4;
        break;
    }
    case 19:
        return Objective_tail_level(this->f_c);
    case 20:
    case 21: {
        result = 0;
        int i = this->f_4;
        while (i < this->f_8) {
            KIPlayer *enemy = enemies->data[i];
            if (KIPlayer_isDead(enemy) != 0 && G<int>(enemy, 0x28) == 8)
                result++;
            i++;
        }
        int count = this->f_8 - this->f_4;
        if ((int)result != count)
            return 0;
        Level *level = this->f_c;
        int field20 = G<int>(level, 0x20);
        int field24 = G<int>(level, 0x24);
        if (this->f_0 != 0x14)
            return field24 <= field20;
        result = field24 > field20;
        break;
    }
    case 22: {
        Array<void *> *messages = Level_getMessages(this->f_c);
        Array<void *> *messages2 = Level_getMessages(this->f_c);
        return Objective_tail_message(messages->data[messages2->length - 1]);
    }
    case 23:
        result = G<uint8_t>(
                     Level_getEnemies(this->f_c)->data[this->f_4],
                     0x24) != 0;
        break;
    case 25:
        result = G<float>(
                     Level_getEnemies(this->f_c)->data[this->f_4],
                     0x64) == 0.0f;
        break;
    case 26: {
        int i = this->f_4;
        while (i < this->f_8) {
            int dead = KIPlayer_isDead(enemies->data[i]);
            i++;
            result = 1;
            if (dead != 0)
                return result;
        }
        return 0;
    }
    case 27:
        result = Level_getEnemiesLeft(this->f_c) < this->f_4;
        break;
    case 28:
        result = Level_getNumDeliveredOre(this->f_c) >= this->f_4;
        break;
    case 29:
        result = Level_getNumDeliveredPassengers(this->f_c) >= this->f_4;
        break;
    case 30:
        return Objective_tail_enemy_final(
            Level_getEnemies(this->f_c)->data[this->f_4]);
    default:
        break;
    }
    return result;
}

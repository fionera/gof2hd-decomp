#include "class.h"

unsigned int Objective::achieved(int value)
{
    Array<KIPlayer *> *enemies = Level_getEnemies(F<Level *>(this, 0xc));
    unsigned int result = 0;

    switch (F<int>(this, 0x0)) {
    case 0:
        result = Level_getEnemiesLeft(F<Level *>(this, 0xc)) == 0;
        break;
    case 1:
        return Objective_tail_enemy(
            Level_getEnemies(F<Level *>(this, 0xc))->data[F<int>(this, 0x4)]);
    case 2:
        result = G<uint8_t>(Route_getLastWaypoint(Level_getPlayerRoute(F<Level *>(this, 0xc))), 0x130) != 0;
        break;
    case 3:
        result = F<int>(this, 0x4) < value;
        break;
    case 4:
        return Objective_tail_message(Level_getMessages(F<Level *>(this, 0xc))->data[F<int>(this, 0x4)]);
    case 5:
        result = Level_getFriendsLeft(F<Level *>(this, 0xc)) == 0;
        break;
    case 7: {
        result = 0;
        int i = 0;
        while (i < F<int>(this, 0x4)) {
            result += KIPlayer_isDead(enemies->data[i]);
            i++;
        }
        result = (int)result == F<int>(this, 0x4);
        break;
    }
    case 8: {
        Array<KIPlayer *> *asteroids = Level_getAsteroids(F<Level *>(this, 0xc));
        result = 0;
        uint32_t i = 0;
        while (i < asteroids->length) {
            result += KIPlayer_isDead(asteroids->data[i]);
            i++;
        }
        result = (int)result > F<int>(this, 0x4);
        break;
    }
    case 9: {
        Array<KIPlayer *> *asteroids = Level_getAsteroids(F<Level *>(this, 0xc));
        uint32_t i = 0;
        while (i < asteroids->length) {
            int limit = F<int>(this, 0x4);
            if ((int)i >= limit)
                return limit < 1;
            i++;
        }
    }
        [[fallthrough]];
    case 10: {
        Array<KIPlayer *> *asteroids = Level_getAsteroids(F<Level *>(this, 0xc));
        uint32_t i = 0;
        while (i < asteroids->length) {
            if ((int)i >= F<int>(this, 0x4))
                return 0;
            i++;
        }
    }
        [[fallthrough]];
    case 11:
        result = G<uint8_t>(
                     Level_getEnemies(F<Level *>(this, 0xc))->data[F<int>(this, 0x4)],
                     0x69) != 0;
        break;
    case 12:
        result = G<uint8_t>(
                     Level_getEnemies(F<Level *>(this, 0xc))->data[F<int>(this, 0x4)],
                     0x68) != 0;
        break;
    case 15:
        return Objective_tail_enemy_payload(
            G<void *>(Level_getEnemies(F<Level *>(this, 0xc))->data[F<int>(this, 0x4)],
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
        int i = F<int>(this, 0x4);
        while (i < F<int>(this, 0x8)) {
            result += KIPlayer_isDead(enemies->data[i]);
            i++;
        }
        result = (int)result == F<int>(this, 0x8) - F<int>(this, 0x4);
        break;
    }
    case 19:
        return Objective_tail_level(F<Level *>(this, 0xc));
    case 20:
    case 21: {
        result = 0;
        int i = F<int>(this, 0x4);
        while (i < F<int>(this, 0x8)) {
            KIPlayer *enemy = enemies->data[i];
            if (KIPlayer_isDead(enemy) != 0 && G<int>(enemy, 0x28) == 8)
                result++;
            i++;
        }
        int count = F<int>(this, 0x8) - F<int>(this, 0x4);
        if ((int)result != count)
            return 0;
        Level *level = F<Level *>(this, 0xc);
        int field20 = G<int>(level, 0x20);
        int field24 = G<int>(level, 0x24);
        if (F<int>(this, 0x0) != 0x14)
            return field24 <= field20;
        result = field24 > field20;
        break;
    }
    case 22: {
        Array<void *> *messages = Level_getMessages(F<Level *>(this, 0xc));
        Array<void *> *messages2 = Level_getMessages(F<Level *>(this, 0xc));
        return Objective_tail_message(messages->data[messages2->length - 1]);
    }
    case 23:
        result = G<uint8_t>(
                     Level_getEnemies(F<Level *>(this, 0xc))->data[F<int>(this, 0x4)],
                     0x24) != 0;
        break;
    case 25:
        result = G<float>(
                     Level_getEnemies(F<Level *>(this, 0xc))->data[F<int>(this, 0x4)],
                     0x64) == 0.0f;
        break;
    case 26: {
        int i = F<int>(this, 0x4);
        while (i < F<int>(this, 0x8)) {
            int dead = KIPlayer_isDead(enemies->data[i]);
            i++;
            result = 1;
            if (dead != 0)
                return result;
        }
        return 0;
    }
    case 27:
        result = Level_getEnemiesLeft(F<Level *>(this, 0xc)) < F<int>(this, 0x4);
        break;
    case 28:
        result = Level_getNumDeliveredOre(F<Level *>(this, 0xc)) >= F<int>(this, 0x4);
        break;
    case 29:
        result = Level_getNumDeliveredPassengers(F<Level *>(this, 0xc)) >= F<int>(this, 0x4);
        break;
    case 30:
        return Objective_tail_enemy_final(
            Level_getEnemies(F<Level *>(this, 0xc))->data[F<int>(this, 0x4)]);
    default:
        break;
    }
    return result;
}

#include "class.h"

struct Route {
    void reset();
};

struct KIPlayer {
    void reset();
    static int isWingMan();
};

struct RadioMessage {
    void reset();
};

struct PlayerEgo {
    void setRoute(int route);
};

void Level::reset() {
    if (playerRoute != 0) {
        ((Route *)playerRoute)->reset();
    }
    if (enemyRoute != 0) {
        ((Route *)enemyRoute)->reset();
    }
    if (friendRoute != 0) {
        ((Route *)friendRoute)->reset();
    }
    unsigned int *list = *(unsigned int **)((char *)this + 0xf8);
    if (list != 0) {
        for (unsigned int i = 0; i < *list; i = i + 1) {
            ((KIPlayer *)((int *)list[1])[i])->reset();
            list = *(unsigned int **)((char *)this + 0xf8);
        }
    }
    createPlayer();
    assignGuns();
    connectPlayers();
    list = *(unsigned int **)((char *)this + 0x114);
    if (list != 0) {
        for (unsigned int i = 0; i < *list; i = i + 1) {
            ((RadioMessage *)((int *)list[1])[i])->reset();
            list = *(unsigned int **)((char *)this + 0x114);
        }
    }
    ((PlayerEgo *)player)->setRoute(playerRoute);
    list = *(unsigned int **)((char *)this + 0xf8);
    int count;
    if (list != 0) {
        count = 0;
        for (unsigned int i = 0; i < *list; i = i + 1) {
            int e = ((int *)list[1])[i];
            if (*(char *)(e + 0x41) == 0 && *(char *)(e + 0x71) == 0 && *(char *)(e + 0x3f) == 0) {
                int wm = KIPlayer::isWingMan();
                list = *(unsigned int **)((char *)this + 0xf8);
                list = *(unsigned int **)((char *)this + 0xf8);
                if (wm == 0) {
                    e = ((int *)list[1])[i];
                    if (*(char *)(e + 0x44) == 0 && *(char *)(e + 0x3c) == 0) {
                        count = count + (*(unsigned char *)(e + 0x3d) ^ 1);
                    }
                }
            }
        }
        if (list != 0) {
            count = count - field_120;
            goto done;
        }
    }
    count = 0;
done:
    enemiesLeft = count;
    int ast;
    if (asteroids == 0) {
        ast = 0;
    } else {
        ast = *(int *)asteroids;
    }
    asteroidsLeft = ast;
    kills = 0;
}

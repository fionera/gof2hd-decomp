#include "class.h"

extern "C" void Level_setAlwaysEnemy(int obj, int flag);

struct SolarSystem {
    int getRace();
};

struct Station;

struct Status {
    int inBlackMarketSystem();
    SolarSystem *getSystem();
    Station *getStation();
};

extern "C" void Level_alarmAllFriends_tail(Station *station, int one);

__attribute__((visibility("hidden"))) extern Status **g_alarmAllFriends;

void Level::alarmAllFriends(int race, bool message) {
    unsigned int *list = *(unsigned int **)((char *)this + 0xf8);
    if (list != 0) {
        for (unsigned int i = 0; i < *list; i = i + 1) {
            int obj = ((int *)list[1])[i];
            if (*(int *)(obj + 0x28) == race) {
                Level_setAlwaysEnemy(*(int *)(obj + 4), 1);
                list = *(unsigned int **)((char *)this + 0xf8);
            }
        }
    }
    if (*(unsigned char *)((char *)this + 0x189) == 0 && message) {
        int type = 1;
        *(unsigned char *)((char *)this + 0x189) = type;
        Status **slot = g_alarmAllFriends;
        if ((*slot)->inBlackMarketSystem() != 0) {
            type = 0xc;
        }
        createRadioMessage(type, race);
        if ((*slot)->getSystem()->getRace() == race) {
            return Level_alarmAllFriends_tail((*slot)->getStation(), 1);
        }
    }
}

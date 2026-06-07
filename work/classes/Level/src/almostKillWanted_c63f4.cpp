#include "class.h"

void operator delete(void *);

struct Station {
    int getIndex();
};

struct Status {
    int isStorylineWanted(int index);
    Station *getStation();
    int getWanted();
    void setMission(int m);
    void setCampaignMission(int m);
};

extern "C" void *Level_opnew_akw(unsigned int size);
extern "C" void Mission_ctor_akw(int m, int a, int b, int idx);
extern "C" void Mission_setCampaign_akw(int m, int flag);
extern "C" void Mission_setWon_akw(int m, int flag);
extern "C" void *dtor_Objective_akw(void *p);
extern "C" void Objective_ctor_akw(int o, int a, int b, int c, Level *self);
extern "C" void Level_setAlwaysEnemy(int obj, int flag);
extern "C" void Player_resetDamageDoneByPlayer(int obj);
extern "C" void Level_almostKillWanted_tail(int target, int zero);

__attribute__((visibility("hidden"))) extern Status **g_almostKillWanted;

void Level::almostKillWanted(int index) {
    if (field_29e != 0) {
        return;
    }
    Status **slot = g_almostKillWanted;
    field_29e = 1;
    if ((*slot)->isStorylineWanted(index) == 0) {
        return;
    }
    int m = (int)Level_opnew_akw(0x78);
    Mission_ctor_akw(m, 4, 0, (*slot)->getStation()->getIndex());
    Mission_setCampaign_akw(m, 1);
    Mission_setWon_akw(m, 1);
    (*slot)->setMission(m);
    (*slot)->setCampaignMission(m);
    if (objectivesA != 0) {
        operator delete(dtor_Objective_akw(*(void **)((char *)this + 0x28)));
    }
    objectivesA = 0;
    int o = (int)Level_opnew_akw(0x1c);
    Objective_ctor_akw(o, 3, 0, 0, this);
    objectivesA = o;
    int e = *(int *)(*(int *)(*(int *)((char *)this + 0xf8) + 4));
    Level_setAlwaysEnemy(*(int *)(e + 4), 0);
    Player_resetDamageDoneByPlayer(*(int *)(*(int *)(*(int *)((char *)this + 0xf8) + 4) + 4));
    int e0 = *(int *)(*(int *)(*(int *)((char *)this + 0xf8) + 4));
    *(unsigned char *)(*(int *)(e0 + 4) + 0x5c) = 0;
    *(unsigned char *)(e0 + 0x43) = 1;
    int w = (*slot)->getWanted();
    return Level_almostKillWanted_tail(((int *)(*(int *)(w + 4)))[index], 0);
}

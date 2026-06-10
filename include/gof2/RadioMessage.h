#ifndef GOF2_RADIOMESSAGE_H
#define GOF2_RADIOMESSAGE_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
struct Objective;
struct PlayerEgo;
struct LevelScript;
struct Player;
struct KIPlayer;
struct Level;
struct Route;
struct Radar;
struct Radio;





template <class T>
static inline T &field(void *self, unsigned offset)
{
    return *reinterpret_cast<T *>(reinterpret_cast<char *>(self) + offset);
}

extern "C" void *operator_new__(uint32_t size);
extern "C" void operator_delete__(void *ptr);



struct PlayerEgo {
    Route *getRoute();
};

struct Route {
    int getCurrent();
};

struct Level {
    int getEnemiesLeft();
    int getFriendsLeft();
};

struct Player {
    Array<Player *> *getEnemies();
    bool isDead();
    bool isAsteroid();
    uint8_t isActive();
    int getHitpoints();
    int getMaxHitpoints();
    KIPlayer *getKIPlayer();
    uint8_t isAlwaysFriend();
    int getArmorHP();
    Vector getPosition();
};

struct Objective {
    uint8_t achieved(int time);
};

struct Radar {
    bool stationLocked();
};

struct Radio {
    RadioMessage *getMessage(int index);
    void setCurrentMessage(RadioMessage *message);
};

struct LevelScript {
    int getEvent();
};

struct RadioMessage { void* _opaque; };  // no offset accesses observed
#endif

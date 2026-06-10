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





// NOTE: the byte-offset accessor template `field<T>(self, off)` is provided by
// common.h; the duplicate definition that the conversion left here was removed to
// avoid a redefinition (and the resulting ambiguous-overload failures at call sites).

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

struct RadioMessage {
    RadioMessage(int param_1, int param_2, int param_3, int param_4, int param_5);
    RadioMessage(int param_1, int param_2, int param_3, int param_4);
    RadioMessage(int param_1, int param_2, Objective *param_3);
    ~RadioMessage();
    void finish();
    uint8_t isOver();
    void reset();
    void trigger();
    uint8_t isTriggered();
    int triggered(int64_t time, PlayerEgo *ego, LevelScript *script);
};
#endif

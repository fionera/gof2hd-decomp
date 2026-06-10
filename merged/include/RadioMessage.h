#ifndef WORK_CLASSES_RADIOMESSAGE_SRC_CLASS_H
#define WORK_CLASSES_RADIOMESSAGE_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef long long int64_t;

struct Objective;
struct PlayerEgo;
struct LevelScript;
struct Player;
struct KIPlayer;
struct Level;
struct Route;
struct Radar;
struct Radio;

struct Vector {
    char raw[12];
};

template <class T>
struct Array {
    uint32_t size;
    T *data;
    uint32_t capacity;
};

template <class T>
static inline T &field(void *self, unsigned offset)
{
    return *reinterpret_cast<T *>(reinterpret_cast<char *>(self) + offset);
}

extern "C" void *operator_new__(uint32_t size);
extern "C" void operator_delete__(void *ptr);

struct RadioMessage {
    RadioMessage(int param_1, int param_2, int param_3, int param_4);
    RadioMessage(int param_1, int param_2, int param_3, int param_4, int param_5);
    RadioMessage(int param_1, int param_2, Objective *param_3);
    ~RadioMessage();

    uint8_t isTriggered();
    uint8_t isOver();
    void reset();
    void finish();
    void trigger();
    int triggered(int64_t param_1, PlayerEgo *param_2, LevelScript *param_3);
};

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

#endif

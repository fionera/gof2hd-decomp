#ifndef WORK_CLASSES_OBJECTIVE_SRC_CLASS_H
#define WORK_CLASSES_OBJECTIVE_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

struct Objective;
struct Level;
struct KIPlayer;
struct Route;
struct Waypoint;

template <class T>
struct Array {
    uint32_t length;
    T *data;
    uint32_t cap;
};

namespace AbyssEngine {
struct String {
    void *data;
    uint32_t field_04;
    uint32_t size;

    String(const String &other, bool copy);
};
}

using String = AbyssEngine::String;

struct Objective {
    Objective(int type, int value, Level *level);
    Objective(int type, int value, int calcValue, Level *level);
    ~Objective();

    Objective *addObjective(Objective *objective);
    unsigned int achieved(int value);
    bool getCalcValue();
    bool isSurvivalObjective();
    void setAchievedText(String *text);
};

template <class T>
static inline T &F(Objective *self, unsigned offset)
{
    return *(T *)((char *)self + offset);
}

template <class T>
static inline T &G(void *self, unsigned offset)
{
    return *(T *)((char *)self + offset);
}

void *operator new(__SIZE_TYPE__ size);
inline void *operator new(__SIZE_TYPE__, void *ptr) noexcept { return ptr; }
extern "C" void operator_delete(void *ptr);

extern "C" void ArrayReleaseClasses_Objective(Array<Objective *> *array);
extern "C" void *Array_ObjectivePtr_dtor(Array<Objective *> *array);
extern "C" void Array_ObjectivePtr_ctor(Array<Objective *> *array);
extern "C" void ArrayAdd_Objective(Objective *objective, Array<Objective *> *array);

extern "C" Array<KIPlayer *> *Level_getEnemies(Level *level);
extern "C" int Level_getEnemiesLeft(Level *level);
extern "C" Route *Level_getPlayerRoute(Level *level);
extern "C" Array<void *> *Level_getMessages(Level *level);
extern "C" int Level_getFriendsLeft(Level *level);
extern "C" Array<KIPlayer *> *Level_getAsteroids(Level *level);
extern "C" int Level_getNumDeliveredOre(Level *level);
extern "C" int Level_getNumDeliveredPassengers(Level *level);

extern "C" int KIPlayer_isDead(KIPlayer *player);
extern "C" Waypoint *Route_getLastWaypoint(Route *route);

extern "C" unsigned int Objective_tail_enemy(KIPlayer *player);
extern "C" unsigned int Objective_tail_enemy_payload(void *payload);
extern "C" unsigned int Objective_tail_level(Level *level);
extern "C" unsigned int Objective_tail_message(void *message);
extern "C" unsigned int Objective_tail_enemy_final(KIPlayer *player);

#endif

#ifndef GOF2_OBJECTIVE_H
#define GOF2_OBJECTIVE_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
struct Objective;
struct Level;
struct KIPlayer;
struct Route;
struct Waypoint;



namespace AbyssEngine {

}

using String = AbyssEngine::String;







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

struct Objective {
    int field_0x0;                      // +0x0
    int field_0x4;                      // +0x4
    int field_0x8;                      // +0x8
    Level* field_0xc;                   // +0xc
    int field_0x10;                     // +0x10
    int field_0x14;                     // +0x14
    int field_0x18;                     // +0x18
};
#endif

#ifndef GOF2_GENERATOR_H
#define GOF2_GENERATOR_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
struct Agent;
struct Generator;
struct ImageFactory;
struct Item;
struct Mission;
struct Ship;
struct SolarSystem;
struct Station;
struct Status;

namespace AbyssEngine {
struct AERandom;

}

static inline void *B(void *p, unsigned off) { return (char *)p + off; }

void *operator new(__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;

extern "C" int AERandom_nextInt(AbyssEngine::AERandom *self, int bound);

extern "C" int Agent_getCosts(Agent *self);
extern "C" int Agent_getOffer(Agent *self);
extern "C" int Agent_getRace(Agent *self);
extern "C" void Agent_getName(AbyssEngine::String *out, Agent *self);
extern "C" int *Agent_getImageParts(Agent *self);
extern "C" int Agent_getSystem(Agent *self);
extern "C" Agent *Agent_ctor(Agent *self, int kind, AbyssEngine::String *name,
                              int station, int system, int race, int male,
                              int a, int b, int c, int d);
extern "C" void Agent_setCosts(Agent *self, int costs);
extern "C" void Agent_setImageParts(Agent *self, int *parts);
extern "C" void Agent_setMission(Agent *self, Mission *mission);
extern "C" void Agent_setOffer(Agent *self, int offer);
extern "C" void Agent_setOfferAccepted(Agent *self, bool accepted);
extern "C" void Agent_setSellItemData(Agent *self, int index, int amount,
                                      int price);
extern "C" void Agent_setWingmanFriendNames(Agent *self,
                                             Array<AbyssEngine::String *> *names);

extern "C" void AbyssEngine_String_dtor(AbyssEngine::String *self);

void Globals_getRandomName(AbyssEngine::String *out, int names,
                                       int race, int male);

extern "C" int *ImageFactory_createChar(ImageFactory *self, int male, int race);

extern "C" int Item_getAmount(Item *self);
extern "C" int Item_getAttribute(Item *self, int which);
extern "C" int Item_getIndex(Item *self);
extern "C" int Item_getIngredients(Item *self);
extern "C" int Item_getMinPriceSystem(Item *self);
extern "C" int Item_getOccurence(Item *self);
extern "C" int Item_getSort(Item *self);
extern "C" int Item_getSinglePrice(Item *self);
extern "C" int Item_getTecLevel(Item *self);
extern "C" int Item_getType(Item *self);
extern "C" void Item_changeAmount(Item *self, int amount);
extern "C" bool Item_isWeapon(Item *self);
extern "C" Item *Item_makeItem(Item *self);
extern "C" Item *Item_setAmount(Item *self, int amount);

extern "C" Mission *Mission_ctor(Mission *self, int type, Agent *agent,
                                  int station);
extern "C" Mission *Mission_ctor_full(Mission *self, int type,
                                       AbyssEngine::String *name,
                                       int *imageParts, int race, int reward,
                                       int targetStation, int difficulty);
extern "C" int Mission_getReward(Mission *self);
extern "C" void Mission_setBonus(Mission *self, int bonus);
extern "C" void Mission_setCosts(Mission *self, int costs);
extern "C" void Mission_setProductionGoods(Mission *self, int item,
                                            int amount);
extern "C" void Mission_setReward(Mission *self, int reward);
extern "C" void Mission_setTargetName(Mission *self,
                                       AbyssEngine::String *name);
extern "C" void Mission_setTargetSystemName(Mission *self,
                                             AbyssEngine::String *name);

extern "C" int Ship_hasCargo(Ship *self, int item, int amount);
extern "C" int Ship_hasJumpDrive(Ship *self);
extern "C" int Ship_hasJumpDriveIntegrated(Ship *self);
extern "C" int Ship_getIndex(Ship *self);
extern "C" int Ship_getPrice(Ship *self);
extern "C" Ship *Ship_makeShip(Ship *self, int owner);
extern "C" void Ship_setRace(Ship *self, int race);
extern "C" void Ship_adjustPrice(Ship *self);
extern "C" int Globals_getRandomEnemyFighter(void *globals, int race);
extern "C" int Achievements_gotAllGoldMedals(void *achievements);
extern "C" int Achievements_gotAllSupernovaMedals(void *achievements);
extern "C" int Wanted_isTerminated(void *wanted);

extern "C" int SolarSystem_getIndex(SolarSystem *self);
extern "C" int SolarSystem_getRace(SolarSystem *self);
extern "C" Array<int> *SolarSystem_getRoutes(SolarSystem *self);
extern "C" Array<int> *SolarSystem_getStations(SolarSystem *self);
extern "C" int SolarSystem_stationIsInSystem(SolarSystem *self, int station);
extern "C" int SolarSystem_getX(SolarSystem *self);
extern "C" int SolarSystem_getY(SolarSystem *self);

extern "C" int Station_getIndex(Station *self);
extern "C" Array<Item *> *Station_getItems(Station *self);
extern "C" Array<Ship *> *Station_getShips(Station *self);
extern "C" int Station_hasShip(Station *self, int ship);
extern "C" int Station_getSystem(Station *self);
extern "C" int Station_getTecLevel(Station *self);

extern "C" Array<Agent *> *Status_getAgents(Status *self);
extern "C" int Status_getCurrentCampaignMission(Status *self);
extern "C" Ship *Status_getShip(Status *self);
extern "C" Station *Status_getStation(Status *self);
extern "C" SolarSystem *Status_getSystem(Status *self);
extern "C" int Status_getLevel(Status *self);
extern "C" Array<uint8_t> *Status_getSystemVisibilities(Status *self);
extern "C" int Status_dlc1Won(Status *self);
extern "C" void *Status_getStanding(Status *self);
extern "C" int Status_hardCoreMode(Status *self);
extern "C" int Status_inBlackMarketSystem(Status *self);
extern "C" int Status_inSupernovaSystem(Status *self);

extern "C" Array<SolarSystem *> *Galaxy_getSystems(void *galaxy);
extern "C" int Galaxy_invDistancePercent(void *galaxy, int x1, int y1, int x2,
                                          int y2);
extern "C" Station *Galaxy_getStation(void *galaxy);
int Galaxy_distance(void *galaxy, SolarSystem *from,
                                SolarSystem *to);
extern "C" int *Galaxy_getAsteroidProbabilities(void *galaxy,
                                                 Station *station);
extern "C" int Standing_isEnemy(void *standing, int race);
extern "C" int Standing_getMissionBonus(void *standing, int race);
extern "C" void SolarSystem_getName(AbyssEngine::String *out,
                                     SolarSystem *self);
extern "C" int Agent_getStation(Agent *self);
extern "C" bool Agent_isGenericAgent(Agent *self);
extern "C" bool Agent_isStoryAgent(Agent *self);
extern "C" Mission *Agent_getMission(Agent *self);

// Generator is a stateless helper class: all methods operate on globals and the
// arguments passed in (no instance fields are accessed).
class Generator {
public:
    // Generator holds no state: construction and destruction are trivial.
    Generator();
    ~Generator();

    void computerTradeGoods(Station *station);
    int generateStationIndex(Array<SolarSystem *> *systems, int station);
    Array<Agent *> *createAgents(Station *station);
    Mission *createMission(Agent *agent, Array<SolarSystem *> *systems);
    Agent *createAgent(Station *station);
    Array<Ship *> *getShipBuyList(Station *station);
    Array<Item *> *getItemBuyList(Station *station);
    Array<int> *getLootList(int itemIndex, int amount);
    bool isKaamoSpecialItem(int item);
};

// Byte-offset accessor for EXTERNAL opaque objects whose layouts are not part of
// this class (e.g. the mission Status struct).
#endif

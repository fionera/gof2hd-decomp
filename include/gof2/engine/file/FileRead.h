#ifndef GOF2_FILEREAD_H
#define GOF2_FILEREAD_H
#include "gof2/common.h"
#include "gof2/engine/file/AEFile.h"

namespace AbyssEngine {
class AERandom;
} // namespace AbyssEngine

class Station;
class SolarSystem;
class Agent;
class Wanted;
class Item;
class Ship;
class NewsItem;
class SpacePoint;

// AbyssEngine::FileRead — loads the game's *.bin data tables (stations, systems, ships, items, ...).
// FileRead is a stateless loader: every loadXxx() opens, reads and closes its file locally, so it
// owns no members and its construction/destruction are trivial.
class FileRead {
public:
    FileRead();
    ~FileRead();

    int32_t loadStation(int32_t id);
    int32_t loadStationsBinary();
    Array<Array<Vector *> *> *loadWeaponPositions(int32_t id);
    Array<SpacePoint *> *loadSpacePoints(int32_t id, int32_t group);
    Array<SolarSystem *> *loadSystemsBinary();
    Array<Wanted *> *loadWanted();
    Array<NewsItem *> *loadTicker();
    Array<Station *> *loadStationsBinary(int16_t *ids, int32_t count);
    Array<String *> *loadNamesBinary(int32_t type, bool first, bool second);
    Array<Station *> *loadStationsBinary(SolarSystem *system);
    Array<Agent *> *loadAgents();
    Array<int32_t> *loadWreckCollision(int32_t id);
    Array<int32_t> *loadStationCollision(int32_t id);
    Array<int32_t> *loadStaticCollision(int32_t id);
    Array<int32_t> *loadStationParts(int32_t id, int32_t special);
    Array<int32_t> *loadShipParts(int32_t id);
    Array<Item *> *loadItemsBinary();
    Array<Ship *> *loadShipsBinary();
};
#endif

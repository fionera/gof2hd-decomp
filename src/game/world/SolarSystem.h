#ifndef GOF2_SOLARSYSTEM_H
#define GOF2_SOLARSYSTEM_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

// Galaxy on Fire 2 — a single star system on the galaxy map. Owns its station-id,
// forbidden-goods and linked-system lists, and an embedded display name.

class Station;

class SolarSystem {
public:
    int starR;   // +0x0
    int starG;   // +0x4
    int starB;   // +0x8
    AbyssEngine::String name;
    int systemId;
    int securityLevel;
    uint32_t faction;
    int mapX;
    int mapY;
    int mapZ;
    int jumpgateStationId;
    int textureIndex;
    Array<int>* stationIds;
    Array<int>* forbiddenGoods;
    Array<int>* linkedSystemIds;
    uint8_t visible;

    SolarSystem(int unk0, AbyssEngine::String displayName, int security,
                bool isVisible, int factionId, int x, int y, int z,
                int jumpgateId, int texture, int* starRGB,
                Array<int>* stations, Array<int>* linkedSystems, Array<int>* forbidden);
    ~SolarSystem();

    bool currentOrbitHasWarpGate();
    int getAttackRace();
    int getIndex();
    int getRace();
    int getSecurityLevel();
    int getTextureIndex();
    int getX();
    int getY();
    int getZ();
    int getWarpGateIndex();
    uint32_t* getStations();
    // Array<int>*-typed view of the station-id list (same member as getStations());
    // callers read the embedded data pointer.
    uint32_t* getStations_i();
    uint32_t* getRoutes();
    AbyssEngine::String getName();
    uint32_t getStationEnumIndex(int idx);
    int getWarpGateEnumIndex();
    // Resolve the warp-gate's enum index — the same table lookup as an ordinary
    // station, so it forwards to getStationEnumIndex().
    int warpGateLookup(int idx);
    int hasHiddenBlueprint();
    uint32_t hasNoOwner();
    int hasPirateBase();
    int isFullyDiscovered();
    uint8_t isVisible();
    void setCoords(int x, int y);
    void setVisible(bool v);
    int stationIsInSystem(Station* station);
    int stationIsInSystem(int idx);
    int systemIsInSystemRoutes(int sys);
};
#endif

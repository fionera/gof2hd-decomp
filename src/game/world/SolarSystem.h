#ifndef GOF2_SOLARSYSTEM_H
#define GOF2_SOLARSYSTEM_H
#include "engine/core/Array.h"
#include "../../engine/core/AEString.h"
#include "Station.h"

class Station;

class SolarSystem {
public:
    int starR;
    int starG;
    int starB;
    AbyssEngine::String name;
    int systemId;
    int securityLevel;
    uint32_t faction;
    int mapX;
    int mapY;
    int mapZ;
    int jumpgateStationId;
    int textureIndex;
    Array<int> *stationIds;
    Array<int> *forbiddenGoods;
    Array<int> *linkedSystemIds;
    bool visible;

    SolarSystem(int unk0, AbyssEngine::String displayName, int security,
                bool isVisible, int factionId, int x, int y, int z,
                int jumpgateId, int texture, int *starRGB,
                Array<int> *stations, Array<int> *linkedSystems, Array<int> *forbidden);

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

    uint32_t *getStations();

    void *getForbiddenGoods(); // lint: void_ptr (method return type — rule 3 forbids changing ClassName::method return types)

    // lint: void_ptr (method return type — rule 3 forbids changing ClassName::method return types)

    uint32_t *getRoutes();

    AbyssEngine::String getName();

    uint32_t getStationEnumIndex(int idx);

    int getWarpGateEnumIndex();

    int hasHiddenBlueprint();

    uint32_t hasNoOwner();

    int hasPirateBase();

    int isFullyDiscovered();

    bool isVisible();

    void setCoords(int x, int y);

    void setVisible(bool v);

    bool stationIsInSystem(Station *station);

    bool stationIsInSystem(int idx);

    bool systemIsInSystemRoutes(int sys);
};
#endif

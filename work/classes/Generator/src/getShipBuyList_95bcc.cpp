#include "class.h"

extern "C" __attribute__((visibility("hidden"))) Status **volatile
    g_Generator_shipStatus;
extern "C" __attribute__((visibility("hidden"))) Array<Ship *> **volatile
    g_Generator_allShips;
extern "C" __attribute__((visibility("hidden"))) AbyssEngine::AERandom **volatile
    g_Generator_shipRandom;
extern "C" __attribute__((visibility("hidden"))) int volatile
    g_Generator_shipRaces[0x40];
extern "C" __attribute__((visibility("hidden"))) uint8_t *volatile
    g_Generator_shipFlags;
extern "C" __attribute__((visibility("hidden"))) void **volatile
    g_Generator_achievements;
extern "C" __attribute__((visibility("hidden"))) void **volatile
    g_Generator_globals;
extern "C" __attribute__((visibility("hidden"))) void **volatile
    g_Generator_wantedList;

static void addShip(Array<Ship *> *list, Ship *base, int race) {
    Ship *ship = Ship_makeShip(base, -1);
    ArrayAdd_ship_ptr(ship, list);
    Ship *added = list->data[list->length - 1];
    Ship_setRace(added, race);
    Ship_adjustPrice(added);
}

Array<Ship *> *Generator::getShipBuyList(Station *station) {
    Status *status = *g_Generator_shipStatus;
    if ((Station_getSystem(station) == 0xf &&
         Status_getCurrentCampaignMission(status) < 0x10) ||
        Station_getIndex(station) == 0x65 ||
        Station_getIndex(station) == 0x6c ||
        Status_inSupernovaSystem(status) != 0) {
        return 0;
    }

    Array<Ship *> *allShips = *g_Generator_allShips;

    if (Station_getIndex(station) == 100 && Status_dlc1Won(status) != 0) {
        Array<Ship *> *result = (Array<Ship *> *)operator new(0xc);
        Array_ship_ptr_ctor(result);
        for (int i = 0; i != 0x40; ++i) {
            Ship *base = allShips->data[i];
            if (Ship_hasJumpDriveIntegrated(base) &&
                Ship_getIndex(base) != 0x25) {
                addShip(result, base, g_Generator_shipRaces[i]);
            }
        }
        return result;
    }

    if (Station_getIndex(station) == 0x6b) {
        Array<Ship *> *result = (Array<Ship *> *)operator new(0xc);
        Array_ship_ptr_ctor(result);
        for (int i = 0; i != 0x32; ++i) {
            if (g_Generator_shipRaces[i] == 8) {
                addShip(result, allShips->data[i], 8);
            }
        }
        if (g_Generator_shipFlags[0x37] != 0) {
            Array<void *> *wanted = *(Array<void *> **)*g_Generator_wantedList;
            if (wanted != 0 && wanted->length != 0) {
                const int offsets[4] = {0x18, 0x30, 0x48, 0x60};
                const int ships[4] = {0x2d, 0x2e, 0x2f, 0x30};
                for (int i = 0; i != 4; ++i) {
                    void *w = *(void **)((char *)wanted->data + offsets[i]);
                    if (Wanted_isTerminated(w)) {
                        addShip(result, allShips->data[ships[i]], 8);
                    }
                }
            }
        }
        return result;
    }

    int race = SolarSystem_getRace(Status_getSystem(status));
    bool gold = Station_getIndex(station) == 10 &&
                Achievements_gotAllGoldMedals(*g_Generator_achievements);
    int stationIndex = Station_getIndex(station);
    int count;
    if (gold) {
        count = 1;
    } else {
        count = AERandom_nextInt(*g_Generator_shipRandom, 6);
        if (stationIndex == 0x29) {
            ++count;
        }
        if (count == 0) {
            return 0;
        }
    }

    Array<Ship *> *result = (Array<Ship *> *)operator new(0xc);
    Array_ship_ptr_ctor(result);
    ArraySetLength_ship_ptr(count, result);

    int forcedShip = gold ? 8 : 10;
    for (int i = 0; i < count; ++i) {
        bool first = i == 0;
        bool forced = gold || (first && stationIndex == 0x29);
        int shipIndex = forced ? forcedShip : 0;
        bool unique = false;
        while (!unique) {
            if (!forced && (stationIndex != 0x4e || !first)) {
                shipIndex =
                    Globals_getRandomEnemyFighter(*g_Generator_globals, race);
            }
            if (count > 1 &&
                AERandom_nextInt(*g_Generator_shipRandom, 100) < 0x16) {
                int roll = AERandom_nextInt(*g_Generator_shipRandom, 5);
                if (!forced && (stationIndex != 0x4e || !first)) {
                    int enemyRace = roll;
                    if (enemyRace == race || enemyRace == 4) {
                        enemyRace = 8;
                    }
                    shipIndex = Globals_getRandomEnemyFighter(
                        *g_Generator_globals, enemyRace);
                }
            }
            unique = true;
            for (int j = 0; j < count; ++j) {
                if (result->data[j] != 0 &&
                    Ship_getIndex(result->data[j]) == shipIndex) {
                    unique = false;
                    break;
                }
            }
        }
        result->data[i] = Ship_makeShip(allShips->data[shipIndex], -1);
        Ship_setRace(result->data[i], g_Generator_shipRaces[shipIndex]);
        Ship_adjustPrice(result->data[i]);
    }

    bool terranBonus = false;
    int raceFlag = 0;
    if (race == 0) {
        if (AERandom_nextInt(*g_Generator_shipRandom, 7) == 0) {
            addShip(result, allShips->data[0x3e], 3);
        }
        terranBonus = true;
    } else if (race == 1) {
        raceFlag = 1;
        if (AERandom_nextInt(*g_Generator_shipRandom, 5) == 0) {
            addShip(result, allShips->data[0x3f], 1);
        }
    } else if (race == 2 &&
               AERandom_nextInt(*g_Generator_shipRandom, 8) == 0) {
        addShip(result, allShips->data[0x3d], 1);
    }

    if (g_Generator_shipFlags[0x35] && Status_dlc1Won(status) && raceFlag) {
        if (AERandom_nextInt(*g_Generator_shipRandom, 2) == 0) {
            addShip(result, allShips->data[0x27], 1);
        }
        if (AERandom_nextInt(*g_Generator_shipRandom, 2) == 0) {
            addShip(result, allShips->data[0x29], 1);
        }
    }

    if (g_Generator_shipFlags[0x37] != 0) {
        if (raceFlag &&
            AERandom_nextInt(*g_Generator_shipRandom, 4) == 0) {
            addShip(result, allShips->data[0x36], 1);
        }
        if (Station_getIndex(station) == 0x78 &&
            Status_getCurrentCampaignMission(status) > 0x9e &&
            (Status_hardCoreMode(status) ||
             Achievements_gotAllSupernovaMedals(*g_Generator_achievements))) {
            addShip(result, allShips->data[0x2c], 1);
        }
        if (Station_getIndex(station) == 0x78 &&
            Status_getCurrentCampaignMission(status) > 0x9e) {
            addShip(result, allShips->data[0x31], 1);
        }
        if (terranBonus &&
            AERandom_nextInt(*g_Generator_shipRandom, 8) == 0) {
            addShip(result, allShips->data[0x33], 0);
        }
    }

    if (Station_getSystem(station) == 0x11 &&
        AERandom_nextInt(*g_Generator_shipRandom, 3) == 0) {
        addShip(result, allShips->data[0x2a], 1);
    }
    if (Station_getSystem(station) == 0x11 &&
        AERandom_nextInt(*g_Generator_shipRandom, 3) == 0) {
        addShip(result, allShips->data[0x2b], 2);
    }
    if (Station_getSystem(station) == 0x11 &&
        AERandom_nextInt(*g_Generator_shipRandom, 3) == 0) {
        addShip(result, allShips->data[0x34], 0);
    }

    return result;
}

#include "class.h"

extern "C" __attribute__((visibility("hidden"))) AbyssEngine::AERandom **volatile
    g_Generator_stationRandom;
extern "C" __attribute__((visibility("hidden"))) Status **volatile
    g_Generator_stationStatus;
extern "C" __attribute__((visibility("hidden"))) int volatile
    g_Generator_stationBlockList[0x34];

int Generator::generateStationIndex(Array<SolarSystem *> *systems, int station) {
    AbyssEngine::AERandom **randomPtr = g_Generator_stationRandom;
    Status **statusPtr = g_Generator_stationStatus;
    bool accepted = false;
    int selected = 0;

    do {
        if (accepted) {
            return selected;
        }

        int roll = AERandom_nextInt(*randomPtr, 100);
        selected = station;
        if (roll >= 20) {
            roll = AERandom_nextInt(*randomPtr, 100);
            if (roll < 40) {
                SolarSystem *system = Status_getSystem(*statusPtr);
                int currentIndex = SolarSystem_getIndex(system);
                Array<int> *stations =
                    SolarSystem_getStations(systems->data[currentIndex]);
                int pick = AERandom_nextInt(*randomPtr, stations->length);
                selected = stations->data[pick];
            } else {
                selected = AERandom_nextInt(*randomPtr, 0x87);
            }
        }

        SolarSystem *currentSystem = Status_getSystem(*statusPtr);
        if (SolarSystem_getIndex(currentSystem) == 0xf) {
            Array<int> *stations =
                SolarSystem_getStations(Status_getSystem(*statusPtr));
            SolarSystem *system = Status_getSystem(*statusPtr);
            Array<int> *stations2 = SolarSystem_getStations(system);
            int pick = AERandom_nextInt(*randomPtr, stations2->length);
            selected = stations->data[pick];
        }

        uint32_t blocked = 0;
        while (blocked <= 0x33) {
            if (selected == g_Generator_stationBlockList[blocked]) {
                break;
            }
            ++blocked;
        }
        accepted = blocked > 0x33;

        uint32_t systemIndex = 0;
        while (systemIndex < systems->length) {
            if (SolarSystem_stationIsInSystem(systems->data[systemIndex],
                                              selected) != 0) {
                break;
            }
            ++systemIndex;
        }
        if (systemIndex >= systems->length) {
            systemIndex = 0;
        }

        if ((uint32_t)(selected - 0x6d) < 5) {
            accepted = false;
        }

        Array<uint8_t> *visibilities =
            Status_getSystemVisibilities(*statusPtr);
        if (visibilities != 0 && systemIndex < visibilities->length) {
            accepted = visibilities->data[systemIndex];
        }

        if (SolarSystem_getRoutes(systems->data[systemIndex]) == 0) {
            int current = SolarSystem_getIndex(Status_getSystem(*statusPtr));
            if (systemIndex != (uint32_t)current) {
                accepted = false;
            }
        }
        accepted = accepted & true;
    } while (true);
}

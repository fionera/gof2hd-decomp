#include "class.h"

extern "C" void *gProgrammedStation;
extern "C" void *gStatus;

extern "C" void *Status_getStation(void *status);
extern "C" void *Status_getSystem(void *status);
extern "C" int Station_equals(void *station, void *other);
extern "C" int SolarSystem_stationIsInSystem(void *system, void *station);
extern "C" int SolarSystem_currentOrbitHasWarpGate(void *system);
extern "C" int SolarSystem_getWarpGateEnumIndex(void *system);
extern "C" int Station_getIndex(void *station);
extern "C" int SolarSystem_getStationEnumIndex(void *system, int stationIndex);
extern "C" void *Level_getPlayer(Level *level);
extern "C" void *Level_getStarSystem(Level *level);
extern "C" void *Level_getLandmarks(Level *level);
extern "C" void *StarSystem_getPlanetTargets(void *starSystem);
extern "C" void Player_setAutoPilotTarget(void *player, void *target);

void LevelScript::setAutoPilotToProgrammedStation()
{
    void **programmedStation = &gProgrammedStation;
    if (*programmedStation != 0) {
        void **status = &gStatus;
        if (Station_equals(Status_getStation(*status), *programmedStation) != 0) {
            *programmedStation = 0;
            return;
        }

        void *target;
        void *player;
        if (SolarSystem_stationIsInSystem(Status_getSystem(*status), *programmedStation) != 0) {
            player = Level_getPlayer((Level *)this->m_pLevel);
            void *targets = StarSystem_getPlanetTargets(Level_getStarSystem((Level *)this->m_pLevel));
            void *system = Status_getSystem(*status);
            int stationIndex = Station_getIndex(*programmedStation);
            int targetIndex = SolarSystem_getStationEnumIndex(system, stationIndex);
            target = ((void **)targets->f_4)[targetIndex];
        } else if (SolarSystem_currentOrbitHasWarpGate(Status_getSystem(*status)) != 0) {
            player = Level_getPlayer((Level *)this->m_pLevel);
            void *landmarks = Level_getLandmarks((Level *)this->m_pLevel);
            target = ((void **)landmarks->f_4)[1];
        } else {
            int warpGateIndex = SolarSystem_getWarpGateEnumIndex(Status_getSystem(*status));
            if (warpGateIndex < 0) {
                return;
            }
            player = Level_getPlayer((Level *)this->m_pLevel);
            void *targets = StarSystem_getPlanetTargets(Level_getStarSystem((Level *)this->m_pLevel));
            target = ((void **)targets->f_4)[warpGateIndex];
        }
        return Player_setAutoPilotTarget(player, target);
    }
}

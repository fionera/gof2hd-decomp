#include "class.h"

extern "C" void *Ship_dtor(Ship *s);
extern "C" void operator_delete(void *p);
extern "C" void ArrayReleaseClasses_Agent(Array<Agent *> *a);
extern "C" void *Array_Agent_dtor(Array<Agent *> *a);
extern "C" void ArrayReleaseClasses_Station(Array<Station *> *a);
extern "C" void *Array_Station_dtor(Array<Station *> *a);
extern "C" void ArrayReleaseClasses_Wanted(Array<Wanted *> *a);
extern "C" void *Array_Wanted_dtor(Array<Wanted *> *a);

// Releases the ship, campaign mission, agent/station-stack/wanted arrays, and the name string.
Status::~Status() {
    if (ship != 0) {
        operator_delete(Ship_dtor(ship));
    }
    ship = 0;
    if (mission != 0) {
        (*(void (**)(Mission *))(*(int *)mission + 4))(mission);
    }
    mission = 0;
    if (agents != 0) {
        ArrayReleaseClasses_Agent(agents);
        if (agents != 0) {
            operator_delete(Array_Agent_dtor(agents));
        }
    }
    agents = 0;
    if (stationStack != 0) {
        ArrayReleaseClasses_Station(stationStack);
        if (stationStack != 0) {
            operator_delete(Array_Station_dtor(stationStack));
        }
    }
    stationStack = 0;
    if (wanted != 0) {
        ArrayReleaseClasses_Wanted(wanted);
        if (wanted != 0) {
            operator_delete(Array_Wanted_dtor(wanted));
        }
    }
    wanted = 0;
}

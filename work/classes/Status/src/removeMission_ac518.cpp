#include "class.h"

__attribute__((visibility("hidden"))) extern int *g_missionSentinel;
extern "C" int Mission_getAgent(Mission *m);
extern "C" void Agent_setMission(Agent *a, Mission *m);

// Removes `mission` from the active list: clears the campaign slot, detaches its agent, and
// resets the slot to the sentinel value.
void Status::removeMission(Mission *mission) {
    int byteOff = 0;
    int *sentinel = g_missionSentinel;
    for (unsigned i = 0; i < missions->size; i = i + 1) {
        if (*(Mission **)((int)missions->data + byteOff) == mission) {
            Mission *target = mission;
            if (this->mission == mission) {
                mission = 0;
                this->mission = 0;
                target = *(Mission **)((int)missions->data + byteOff);
            }
            if (Mission_getAgent(target) != 0) {
                Agent *ag = (Agent *)Mission_getAgent(*(Mission **)((int)missions->data + byteOff));
                Agent_setMission(ag, 0);
            }
            int *slot = *(int **)((int)missions->data + byteOff);
            int *clr;
            if (slot == 0) {
                clr = (int *)((int)missions->data + i * 4);
            } else {
                (*(void (**)(int *))(*slot + 4))(slot);
                clr = (int *)((int)missions->data + byteOff);
            }
            *clr = 0;
            *(int *)((int)missions->data + byteOff) = *sentinel;
        }
        byteOff = byteOff + 4;
    }
}

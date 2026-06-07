#include "class.h"

extern "C" Agent *Mission_getAgent(Mission *self);
extern "C" void Mission_setWon(Mission *self, bool won);
extern "C" void Mission_setFailed(Mission *self, bool failed);
extern "C" int Agent_isGenericAgent(Agent *self);
extern "C" void Agent_setOfferAccepted(Agent *self, bool accepted);
extern "C" int Status_getCurrentCampaignMission(void *status);
extern "C" void DialogueWindow_loadContent(DialogueWindow *self);

__attribute__((visibility("hidden"))) extern void **g_dw_statusForSet;

extern "C" void DialogueWindow_set(DialogueWindow *self, Mission *mission, int kind, int campaign)
{
    if (kind == 1) {
        F<Mission *>(self, 0x4c) = mission;
        F<int>(self, 0x44) = kind;
        goto won;
    }
    F<Mission *>(self, 0x4c) = mission;
    F<int>(self, 0x44) = kind;
    if (kind == 2) {
        Agent *agent = Mission_getAgent(mission);
        if (agent != 0 && Agent_isGenericAgent(agent) == 0) {
            Agent_setOfferAccepted(agent, false);
        }
        Mission_setFailed(mission, true);
        goto finish;
    }
    goto finish;

won:
    Mission_getAgent(mission);
    Mission_setWon(mission, true);

finish:
    F<int>(self, 0x48) = 0;
    if (campaign == -1) {
        campaign = Status_getCurrentCampaignMission(*g_dw_statusForSet);
    }
    F<int>(self, 0x10) = campaign;
    DialogueWindow_loadContent(self);
}

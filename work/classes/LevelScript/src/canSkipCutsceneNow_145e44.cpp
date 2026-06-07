#include "class.h"

extern "C" void *gStatus;
extern "C" int Status_getCurrentCampaignMission(void *status);

uint32_t LevelScript::canSkipCutsceneNow()
{
    void **status = &gStatus;
    if (Status_getCurrentCampaignMission(*status) == 0x9a) {
        if (!((uint32_t)(I(this, 0x1c) - 1) < 9)) {
            return 0;
        }
    } else if (Status_getCurrentCampaignMission(*status) == 0x9d) {
        if (!((uint32_t)(I(this, 0x1c) - 2) < 3)) {
            return 0;
        }
    } else if (Status_getCurrentCampaignMission(*status) == 0x9e && I(this, 0x1c) > 1) {
        return 0;
    }
    return 1;
}

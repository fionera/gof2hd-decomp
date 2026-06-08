#include "class.h"

extern "C" bool Mission_isCampaignMission(Mission *self) {
    return F<int>(self, 0x64) != 0;
}

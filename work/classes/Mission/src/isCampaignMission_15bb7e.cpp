#include "class.h"

extern "C" bool Mission_isCampaignMission(Mission *self) {
    return self->f_64 != 0;
}

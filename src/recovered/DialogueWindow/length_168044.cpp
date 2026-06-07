#include "class.h"

extern "C" int Mission_isCampaignMission(Mission *self);
extern "C" int Mission_getTargetStation(Mission *self);

__attribute__((visibility("hidden"))) extern int g_dw_briefingCounts[];
__attribute__((visibility("hidden"))) extern int g_dw_successCounts[];
__attribute__((visibility("hidden"))) extern void **g_dw_status;

extern "C" int DialogueWindow_length(DialogueWindow *self)
{
    Mission *mission = F<Mission *>(self, 0x4c);
    if (mission != 0 && Mission_isCampaignMission(mission) != 0) {
        int kind = F<int>(self, 0x44);
        int *counts;
        if (kind == 1) {
            counts = g_dw_successCounts;
        } else {
            if (kind != 0) return 1;
            counts = g_dw_briefingCounts;
        }
        return counts[F<int>(self, 0x10)] / 2;
    }
    if (F<int>(self, 0x44) == 0 && F<void *>(self, 0x4c) != 0 &&
        Mission_getTargetStation(F<Mission *>(self, 0x4c)) == 0x6c) {
        int result = 6;
        if (F<int>(*g_dw_status, 0x114) == 2) result = 0x12;
        return result;
    }
    return 1;
}

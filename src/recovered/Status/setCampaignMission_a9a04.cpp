#include "class.h"
extern "C" void Mission_setCampaignMission(Mission *m, bool flag);
extern "C" int *g_campaignSentinel;
void Status::setCampaignMission(Mission *m) {
    Mission_setCampaignMission(m, true);
    int **slot = (int **)missions->data;
    int *cur = slot[0];
    if (cur != 0 && cur != g_campaignSentinel) {
        (*(void (**)(int *))(*cur + 4))(cur);
        ((int **)missions->data)[0] = 0;
        slot = (int **)missions->data;
    }
    slot[0] = (int *)m;
}

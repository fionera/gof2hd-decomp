#include "class.h"

extern "C" void *gStatus;
extern "C" int Status_getCurrentCampaignMission(void *status);
extern "C" void *Level_getPlayer(Level *level);
extern "C" void Player_setUnknown(void *player, bool enabled);

void LevelScript::skipSequence()
{
    if (I(this, 0x24) > 0 && Status_getCurrentCampaignMission(gStatus) > 0) {
        I(this, 0x24) = 0x1b59;
        void *player = Level_getPlayer((Level *)P(this, 0x18));
        return Player_setUnknown(P(player, 0), true);
    }
}

#include "class.h"

extern "C" void *Level_getMessages(Level *level);
extern "C" void *Level_getPlayer(Level *level);
extern "C" void *Level_getActiveMessages(Level *level);
extern "C" int Status_getCurrentCampaignMission(void *status);
extern "C" void TargetFollowCamera_update(TargetFollowCamera *camera, float delta, int a, int b);
extern "C" void *gStatus;

void LevelScript::process(int delta)
{
    Level *level = (Level *)P(this, 0x18);
    void *messages = Level_getMessages(level);
    void *player = Level_getPlayer(level);
    void *activeMessages = Level_getActiveMessages(level);
    int mission = Status_getCurrentCampaignMission(gStatus);

    (void)messages;
    (void)player;
    (void)activeMessages;

    if (mission != 0) {
        float frameDelta = (float)delta * 0.001f;
        TargetFollowCamera_update((TargetFollowCamera *)P(this, 0x14), frameDelta, 0, 0);
    }
}

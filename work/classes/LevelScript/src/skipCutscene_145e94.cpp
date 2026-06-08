#include "class.h"

struct StackVector {
    char bytes[12];
};

typedef void *(*LevelListProc)(Level *);
typedef void (*ReadWaypointProc)(StackVector *out, void *waypoint);
typedef void (*SetVectorProc)(void *self, StackVector *value);
typedef void (*VirtualCommandProc)(void *self, int a, int b, int c);

extern "C" void *gStatus;
extern "C" int Status_getCurrentCampaignMission(void *status);
extern "C" void *Level_getMessages(Level *level);
extern "C" void RadioMessage_trigger(void *message);
extern "C" void RadioMessage_finish(void *message);
extern "C" void PlayerFighter_setAIDisabled(void *fighter, bool disabled);
extern "C" void *KIPlayer_getRoute(void *player);
extern "C" void *Route_getWaypoint(void *route, int index);
extern "C" void KIPlayer_setVisible(void *player, bool visible);
extern "C" void KIPlayer_setActive(void *player, bool active);

static volatile LevelListProc gLevelListProc;

static inline void *firstListEntry(void *list)
{
    return ((void **)P(list, 4))[0];
}

void LevelScript::skipCutscene()
{
    StackVector position;
    void **status = &gStatus;

    int mission = Status_getCurrentCampaignMission(*status);
    if (mission == 0x9a) {
        if ((uint32_t)(I(this, 0x1c) - 1) < 9) {
            I(this, 0x1c) = 9;
            for (int i = 0; i != 8; ++i) {
                void *messages = Level_getMessages((Level *)P(this, 0x18));
                RadioMessage_trigger(((void **)P(messages, 4))[i]);
                messages = Level_getMessages((Level *)P(this, 0x18));
                RadioMessage_finish(((void **)P(messages, 4))[i]);
            }

            I(this, 0x90) = 0x7d1;
            I(this, 0x94) = 0;

            LevelListProc getList = gLevelListProc;
            void *list = getList((Level *)P(this, 0x18));
            PlayerFighter_setAIDisabled(firstListEntry(list), false);

            list = getList((Level *)P(this, 0x18));
            void *fighter = firstListEntry(list);

            list = getList((Level *)P(this, 0x18));
            void *route = KIPlayer_getRoute(firstListEntry(list));
            void *waypoint = Route_getWaypoint(route, 0);
            ReadWaypointProc readWaypoint = *(ReadWaypointProc *)((char *)P(waypoint, 0) + 0x28);
            readWaypoint(&position, waypoint);
            SetVectorProc setVector = *(SetVectorProc *)((char *)P(fighter, 0) + 0x44);
            setVector(fighter, &position);
        }
    } else if (Status_getCurrentCampaignMission(*status) == 0x9d) {
        if (I(this, 0x1c) <= 4) {
            for (int i = 0; i != 4; ++i) {
                void *messages = Level_getMessages((Level *)P(this, 0x18));
                RadioMessage_trigger(((void **)P(messages, 4))[i]);
                messages = Level_getMessages((Level *)P(this, 0x18));
                RadioMessage_finish(((void **)P(messages, 4))[i]);
            }
            I(this, 0x90) = 0x4651;
            I(this, 0x94) = 0;
            I(this, 0x1c) = 4;
        }
    } else if (Status_getCurrentCampaignMission(*status) == 0x9e) {
        I(this, 0x98) = 0x2ee1;
        I(this, 0x9c) = 0;
        for (int i = 0; i != 3; ++i) {
            void *messages = Level_getMessages((Level *)P(this, 0x18));
            RadioMessage_trigger(((void **)P(messages, 4))[i]);
            messages = Level_getMessages((Level *)P(this, 0x18));
            RadioMessage_finish(((void **)P(messages, 4))[i]);
        }

        LevelListProc getList = gLevelListProc;
        void *list = getList((Level *)P(this, 0x18));
        void *player = firstListEntry(list);
        VirtualCommandProc command = *(VirtualCommandProc *)((char *)P(player, 0) + 0x48);
        command(player, 0x000ba51e, 0, 0x000ba4b6);

        list = getList((Level *)P(this, 0x18));
        KIPlayer_setVisible(firstListEntry(list), true);

        list = getList((Level *)P(this, 0x18));
        KIPlayer_setActive(firstListEntry(list), true);
        I(this, 0x1c) = 2;
    }
}

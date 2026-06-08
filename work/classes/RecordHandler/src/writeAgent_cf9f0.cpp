#include "class.h"

extern "C" void *__stack_chk_guard;
extern "C" void __stack_chk_fail(unsigned diff);

extern "C" void AEFile_WriteInt(int v, unsigned int fd);
extern "C" void AEFile_WriteBool(int v, unsigned int fd);
extern "C" void AEFile_WriteString(void *s, unsigned int fd, int flag);
extern "C" void AEString_cstr_ctor(void *dst, const char *s, int copy);
extern "C" void AEString_dtor(void *s);

extern "C" int Agent_getCosts(void *a);
extern "C" int Agent_getSellSystemIndex(void *a);
extern "C" int Agent_getSellBlueprintIndex(void *a);
extern "C" int Agent_getEvent(void *a);
extern "C" int Agent_getIndex(void *a);
extern "C" int Agent_getOffer(void *a);
extern "C" int Agent_getRace(void *a);
extern "C" int Agent_getSellItemIndex(void *a);
extern "C" int Agent_getSellItemPrice(void *a);
extern "C" int Agent_getSellItemQuantity(void *a);
extern "C" int Agent_getStation(void *a);
extern "C" int Agent_getSystem(void *a);
extern "C" int Agent_getWingmanFriendsCount(void *a);
extern "C" int Agent_isMale(void *a);
extern "C" int Agent_hasReward(void *a);
extern "C" int Agent_hasAcceptedOffer(void *a);
extern "C" int *Agent_getImageParts(void *a);
extern "C" int Agent_getSellModIndex(void *a);
extern "C" void Agent_getMissionString(void *out, void *a);
extern "C" void Agent_getName(void *out, void *a);
extern "C" void Agent_getStationName(void *out, void *a);
extern "C" void Agent_getSystemName(void *out, void *a);
extern "C" void *Agent_getMission(void *a);
extern "C" void RecordHandler_writeMission(RecordHandler *self, void *m, unsigned int fd);

__attribute__((visibility("hidden"))) extern int *g_WA_guard;       // DAT_000dfc2c -> guard holder
__attribute__((visibility("hidden"))) extern const char g_WA_empty1[]; // DAT_000dfc38
__attribute__((visibility("hidden"))) extern const char g_WA_empty2[]; // DAT_000dfc3c

// RecordHandler::writeAgent(Agent*, unsigned int fd)
extern "C" void RecordHandler_writeAgent(RecordHandler *self, void *agent, unsigned int fd)
{
    int *guardP = g_WA_guard;
    volatile int saved = *guardP;

    AEFile_WriteInt(Agent_getCosts(agent), fd);
    AEFile_WriteInt(Agent_getSellSystemIndex(agent), fd);
    AEFile_WriteInt(Agent_getSellBlueprintIndex(agent), fd);
    AEFile_WriteInt(Agent_getEvent(agent), fd);
    AEFile_WriteInt(Agent_getIndex(agent), fd);
    AEFile_WriteInt(Agent_getOffer(agent), fd);
    AEFile_WriteInt(Agent_getRace(agent), fd);
    AEFile_WriteInt(Agent_getSellItemIndex(agent), fd);
    AEFile_WriteInt(Agent_getSellItemPrice(agent), fd);
    AEFile_WriteInt(Agent_getSellItemQuantity(agent), fd);
    AEFile_WriteInt(Agent_getStation(agent), fd);
    AEFile_WriteInt(Agent_getSystem(agent), fd);
    AEFile_WriteInt(Agent_getWingmanFriendsCount(agent), fd);
    AEFile_WriteBool(Agent_isMale(agent), fd);
    AEFile_WriteBool(Agent_hasReward(agent), fd);
    AEFile_WriteBool(Agent_hasAcceptedOffer(agent), fd);
    AEFile_WriteBool(*(unsigned char *)((char *)agent + 0x24), fd);
    AEFile_WriteBool(*(unsigned char *)((char *)agent + 0x25), fd);

    if (Agent_getImageParts(agent) == 0) {
        AEFile_WriteInt(-1, fd);
    } else {
        AEFile_WriteInt(5, fd);
        for (int i = 0; i != 5; i++) {
            int *img = Agent_getImageParts(agent);
            AEFile_WriteInt(img[i], fd);
        }
    }
    if (0x12 < Agent_getIndex(agent)) {
        AEFile_WriteInt(Agent_getSellModIndex(agent), fd);
    }

    char s[12];
    Agent_getMissionString(s, agent);
    AEFile_WriteString(s, fd, 1);
    AEString_dtor(s);
    Agent_getName(s, agent);
    AEFile_WriteString(s, fd, 1);
    AEString_dtor(s);
    Agent_getStationName(s, agent);
    AEFile_WriteString(s, fd, 1);
    AEString_dtor(s);
    Agent_getSystemName(s, agent);
    AEFile_WriteString(s, fd, 1);
    AEString_dtor(s);

    void *f0c = *(void **)((char *)agent + 0xc);
    if (f0c == 0) {
        AEString_cstr_ctor(s, g_WA_empty1, 0);
        AEFile_WriteString(s, fd, 1);
        AEString_dtor(s);
    } else {
        AEFile_WriteString(f0c, fd, 1);
    }
    void *f10 = *(void **)((char *)agent + 0x10);
    if (f10 == 0) {
        AEString_cstr_ctor(s, g_WA_empty2, 0);
        AEFile_WriteString(s, fd, 1);
        AEString_dtor(s);
    } else {
        AEFile_WriteString(f10, fd, 1);
    }

    *(void **)((char *)self + 4) = agent;
    void *mission = Agent_getMission(agent);
    if (mission == 0 || *(void **)self == mission) {
        AEFile_WriteInt(-1, fd);
    } else {
        AEFile_WriteInt(1, fd);
        RecordHandler_writeMission(self, mission, fd);
    }

    if (*guardP == saved) {
        return;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
}

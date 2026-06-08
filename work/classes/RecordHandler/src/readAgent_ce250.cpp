#include "class.h"

extern "C" void *__stack_chk_guard;
extern "C" void __stack_chk_fail(unsigned diff);

extern "C" void AEFile_ReadInt(void *out, unsigned int fd);
extern "C" void AEFile_ReadByte(void *out, unsigned int fd);
extern "C" void AEFile_ReadString(void *out, unsigned int fd, int flag);
extern "C" unsigned char *RH_op_new_arr(unsigned int n);
extern "C" void *RH_op_new(unsigned int sz);
extern "C" void AEString_default_ctor(void *s);
extern "C" void AEString_copy_ctor(void *dst, void *src, int copy);
extern "C" void AEString_dtor(void *s);
extern "C" void *RecordHandler_readMission(RecordHandler *self, unsigned int fd);
extern "C" void Agent_ctor(void *self, int idx, void *name, int station, int system, int race,
                           int male, int sellSys, int sellBp, int sellMod, int sellItemPrice);
extern "C" void Agent_setCosts(void *a, int v);
extern "C" void Agent_setEvent(void *a, int v);
extern "C" void Agent_setOffer(void *a, int v);
extern "C" void Agent_setSellItemData(void *a, int idx, int qty, int price);
extern "C" void Agent_setWingmanFriendNames(void *a, void *arr);
extern "C" void Agent_giveRewardAtNextChat(void *a, bool v);
extern "C" void Agent_setOfferAccepted(void *a, bool v);
extern "C" void Agent_setImageParts(void *a, int *parts);
extern "C" void Agent_setMissionString(void *a, void *s);
extern "C" void Agent_setStationName(void *a, void *s);
extern "C" void Agent_setSystemName(void *a, void *s);
extern "C" void Agent_setMission(void *a, void *m);
extern "C" void ArrayStr_ctor(void *a);
extern "C" void ArraySetLength_Str(unsigned n, void *a);

__attribute__((visibility("hidden"))) extern int *g_RA_guard;   // DAT_000de608 -> guard holder

// RecordHandler::readAgent(unsigned int fd) -> Agent* (in r0).
extern "C" void *RecordHandler_readAgent(RecordHandler *self, unsigned int fd)
{
    int *guardP = g_RA_guard;
    volatile int saved = *guardP;

    int costs = 0, sellSys = 0, sellBp = 0, event = 0, idx = 0, offer = 0, race = 0;
    int sellItemIdx = 0, sellItemPrice = 0, sellItemQty = 0, station = 0, system = 0;
    unsigned wingmen = 0;
    AEFile_ReadInt(&costs, fd);
    AEFile_ReadInt(&sellSys, fd);
    AEFile_ReadInt(&sellBp, fd);
    AEFile_ReadInt(&event, fd);
    AEFile_ReadInt(&idx, fd);
    AEFile_ReadInt(&offer, fd);
    AEFile_ReadInt(&race, fd);
    AEFile_ReadInt(&sellItemIdx, fd);
    AEFile_ReadInt(&sellItemPrice, fd);
    AEFile_ReadInt(&sellItemQty, fd);
    AEFile_ReadInt(&station, fd);
    AEFile_ReadInt(&system, fd);
    AEFile_ReadInt(&wingmen, fd);

    unsigned char male = 0;
    bool hasReward = false, accepted = false;
    unsigned char raw24 = 0, raw25 = 0;
    AEFile_ReadByte(&male, fd);
    AEFile_ReadByte(&hasReward, fd);
    AEFile_ReadByte(&accepted, fd);
    AEFile_ReadByte(&raw24, fd);
    AEFile_ReadByte(&raw25, fd);

    unsigned imgCount = 0;
    AEFile_ReadInt(&imgCount, fd);
    int *img = 0;
    if (0 < (int)imgCount) {
        unsigned long long bytes = (unsigned long long)imgCount * 4;
        unsigned sz = (int)(bytes >> 32) != 0 ? 0xffffffff : (unsigned)bytes;
        img = (int *)RH_op_new_arr(sz);
        int *p = img;
        for (int i = 0; i < (int)imgCount; i++) {
            AEFile_ReadInt(p, fd);
            p++;
        }
    }

    int sellMod = -1;
    if (0x12 < idx) {
        AEFile_ReadInt(&sellMod, fd);
    }

    char missionStr[12], name[12], stationName[12], systemName[12], strE[12], strF[12];
    AEString_default_ctor(missionStr);
    AEString_default_ctor(name);
    AEString_default_ctor(stationName);
    AEString_default_ctor(systemName);
    AEString_default_ctor(strE);
    AEString_default_ctor(strF);
    AEFile_ReadString(missionStr, fd, 1);
    AEFile_ReadString(name, fd, 1);
    AEFile_ReadString(stationName, fd, 1);
    AEFile_ReadString(systemName, fd, 1);
    AEFile_ReadString(strE, fd, 1);
    AEFile_ReadString(strF, fd, 1);

    int hasMission = 0;
    AEFile_ReadInt(&hasMission, fd);
    void *mission = (hasMission < 1) ? 0 : RecordHandler_readMission(self, fd);

    void *agent = RH_op_new(0x98);
    char nameCopy[12];
    AEString_copy_ctor(nameCopy, name, 0);
    Agent_ctor(agent, idx, nameCopy, station, system, race, male, sellSys, sellBp, sellMod,
               sellItemIdx);
    AEString_dtor(nameCopy);

    Agent_setCosts(agent, costs);
    Agent_setEvent(agent, event);
    Agent_setOffer(agent, offer);
    Agent_setSellItemData(agent, sellItemPrice, sellItemQty, sellItemIdx);

    if (*((int *)strE + 2) != 0) {
        void *s = RH_op_new(0xc);
        AEString_copy_ctor(s, strE, 0);
        *(void **)((char *)agent + 0xc) = s;
    }
    if (*((int *)strF + 2) != 0) {
        void *s = RH_op_new(0xc);
        AEString_copy_ctor(s, strF, 0);
        *(void **)((char *)agent + 0x10) = s;
    }
    *(unsigned *)((char *)agent + 0x14) = wingmen;

    void *arr = RH_op_new(0xc);
    ArrayStr_ctor(arr);
    ArraySetLength_Str(wingmen, arr);
    for (int i = 0; i < (int)wingmen; i++) {
        void *s = RH_op_new(0xc);
        void *src = (i == 0) ? (void *)strE : (void *)strF;
        AEString_copy_ctor(s, src, 0);
        *(void **)(*(char **)((char *)arr + 4) + i * 4) = s;
    }
    Agent_setWingmanFriendNames(agent, arr);
    Agent_giveRewardAtNextChat(agent, hasReward);
    Agent_setOfferAccepted(agent, accepted);
    Agent_setImageParts(agent, img);

    char tmp[12];
    AEString_copy_ctor(tmp, missionStr, 0);
    Agent_setMissionString(agent, tmp);
    AEString_dtor(tmp);
    AEString_copy_ctor(tmp, stationName, 0);
    Agent_setStationName(agent, tmp);
    AEString_dtor(tmp);
    AEString_copy_ctor(tmp, systemName, 0);
    Agent_setSystemName(agent, tmp);
    AEString_dtor(tmp);
    Agent_setMission(agent, mission);

    *(unsigned char *)((char *)agent + 0x24) = raw24;
    *(unsigned char *)((char *)agent + 0x25) = raw25;

    AEString_dtor(strF);
    AEString_dtor(strE);
    AEString_dtor(systemName);
    AEString_dtor(stationName);
    AEString_dtor(name);
    AEString_dtor(missionStr);

    if (*guardP == saved) {
        return agent;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
}

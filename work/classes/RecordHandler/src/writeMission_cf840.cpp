#include "class.h"

extern "C" void *__stack_chk_guard;
extern "C" void __stack_chk_fail(unsigned diff);

extern "C" void AEFile_WriteBool(int v, unsigned int fd);
extern "C" void AEFile_WriteInt(int v, unsigned int fd);
extern "C" void AEFile_WriteString(void *s, unsigned int fd, int flag);
extern "C" void AEString_dtor(void *s);

extern "C" int Mission_getType(void *m);
extern "C" int Mission_isEmpty(void *m);
extern "C" void Mission_getClientName(void *out, void *m);
extern "C" void Mission_getTargetName(void *out, void *m);
extern "C" void Mission_getTargetStationName(void *out, void *m);
extern "C" void Mission_getTargetSystemName(void *out, void *m);
extern "C" int Mission_isCampaignMission(void *m);
extern "C" int *Mission_getClientImage(void *m);
extern "C" int Mission_getClientRace(void *m);
extern "C" int Mission_getCosts(void *m);
extern "C" int Mission_getBonus(void *m);
extern "C" int Mission_getReward(void *m);
extern "C" int Mission_getTargetStation(void *m);
extern "C" int Mission_getDifficulty(void *m);
extern "C" int Mission_getProductionGoodIndex(void *m);
extern "C" int Mission_getProductionGoodAmount(void *m);
extern "C" int Mission_getStatusValue(void *m);
extern "C" int Mission_isVisible(void *m);
extern "C" void *Mission_getAgent(void *m);
extern "C" void RecordHandler_writeAgent(RecordHandler *self, void *agent, unsigned int fd);

__attribute__((visibility("hidden"))) extern int *g_WM_guard;   // DAT_000df9e8 -> guard holder

// RecordHandler::writeMission(Mission*, unsigned int fd)
extern "C" void RecordHandler_writeMission(RecordHandler *self, void *m, unsigned int fd)
{
    int *guardP = g_WM_guard;
    volatile int saved = *guardP;

    AEFile_WriteInt(Mission_getType(m), fd);
    if (Mission_isEmpty(m) == 0) {
        char s[12];
        Mission_getClientName(s, m);
        AEFile_WriteString(s, fd, 1);
        AEString_dtor(s);
        Mission_getTargetName(s, m);
        AEFile_WriteString(s, fd, 1);
        AEString_dtor(s);
        Mission_getTargetStationName(s, m);
        AEFile_WriteString(s, fd, 1);
        AEString_dtor(s);
        Mission_getTargetSystemName(s, m);
        AEFile_WriteString(s, fd, 1);
        AEString_dtor(s);

        AEFile_WriteBool(Mission_isCampaignMission(m), fd);
        if (Mission_getClientImage(m) == 0) {
            AEFile_WriteInt(-1, fd);
        } else {
            AEFile_WriteInt(5, fd);
            for (int i = 0; i != 5; i++) {
                int *img = Mission_getClientImage(m);
                AEFile_WriteInt(img[i], fd);
            }
        }
        AEFile_WriteInt(Mission_getClientRace(m), fd);
        AEFile_WriteInt(Mission_getCosts(m), fd);
        AEFile_WriteInt(Mission_getBonus(m), fd);
        AEFile_WriteInt(Mission_getReward(m), fd);
        AEFile_WriteInt(Mission_getTargetStation(m), fd);
        AEFile_WriteInt(Mission_getDifficulty(m), fd);
        AEFile_WriteInt(Mission_getProductionGoodIndex(m), fd);
        AEFile_WriteInt(Mission_getProductionGoodAmount(m), fd);
        AEFile_WriteInt(Mission_getStatusValue(m), fd);
        AEFile_WriteBool(Mission_isVisible(m), fd);

        *(void **)self = m;
        void *agent = Mission_getAgent(m);
        if (agent == 0 || *(void **)((char *)self + 4) == agent) {
            AEFile_WriteInt(-1, fd);
        } else {
            AEFile_WriteInt(1, fd);
            RecordHandler_writeAgent(self, agent, fd);
        }
    }

    if (*guardP == saved) {
        return;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
}

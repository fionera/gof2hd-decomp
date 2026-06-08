#include "class.h"

extern "C" void *__stack_chk_guard;
extern "C" void __stack_chk_fail(unsigned diff);

extern "C" void AEFile_ReadInt(void *out, unsigned int fd);
extern "C" void AEFile_ReadBool(void *out, unsigned int fd);
extern "C" void AEFile_ReadString(void *out, unsigned int fd, int flag);
extern "C" unsigned char *RH_op_new_arr(unsigned int n);
extern "C" void *RH_op_new(unsigned int sz);
extern "C" void AEString_default_ctor(void *s);
extern "C" void AEString_copy_ctor(void *dst, void *src, int copy);
extern "C" void AEString_dtor(void *s);
extern "C" void *RecordHandler_readAgent(RecordHandler *self, unsigned int fd);
extern "C" void Mission_ctorFull(void *self, int type, void *clientName, int *img, int clientRace,
                                 int reward, int targetStation, int difficulty);
extern "C" void Mission_ctorEmpty(void *self, int type, int reward, int targetStation);
extern "C" void Mission_setCosts(void *m, int v);
extern "C" void Mission_setBonus(void *m, int v);
extern "C" void Mission_setProductionGoods(void *m, int idx, int amt);
extern "C" void Mission_setStatusValue(void *m, int v);
extern "C" void Mission_setVisible(void *m, bool v);
extern "C" void Mission_setAgent(void *m, void *agent);
extern "C" void Mission_setTargetName(void *m, void *name);
extern "C" void Agent_setMission(void *agent, void *m);

__attribute__((visibility("hidden"))) extern int *g_RM_guard;   // DAT_000de244 -> guard holder

// RecordHandler::readMission(unsigned int fd) -> Mission* (in r0).
extern "C" void *RecordHandler_readMission(RecordHandler *self, unsigned int fd)
{
    int *guardP = g_RM_guard;
    volatile int saved = *guardP;
    void *mission = 0;

    int type = 0;
    AEFile_ReadInt(&type, fd);
    if (type != -1) {
        char clientName[12], targetName[12], targetStation[12], targetSystem[12];
        AEString_default_ctor(clientName);
        AEFile_ReadString(clientName, fd, 1);
        AEString_default_ctor(targetName);
        AEFile_ReadString(targetName, fd, 1);
        AEString_default_ctor(targetStation);
        AEFile_ReadString(targetStation, fd, 1);
        AEString_default_ctor(targetSystem);
        AEFile_ReadString(targetSystem, fd, 1);

        bool isEmpty = false;
        AEFile_ReadBool(&isEmpty, fd);

        unsigned imgCount = 0;
        AEFile_ReadInt(&imgCount, fd);
        int *img = 0;
        if ((int)imgCount >= 1) {
            unsigned long long bytes = (unsigned long long)imgCount * 4;
            unsigned sz = (int)(bytes >> 32) != 0 ? 0xffffffff : (unsigned)bytes;
            img = (int *)RH_op_new_arr(sz);
            int *p = img;
            for (int i = 0; i < (int)imgCount; i++) {
                AEFile_ReadInt(p, fd);
                p++;
            }
        }

        int clientRace = 0, costs = 0, bonus = 0, reward = 0, targetStationIdx = 0;
        int difficulty = 0, prodIdx = 0, prodAmt = 0, statusValue = 0;
        bool visible = false;
        AEFile_ReadInt(&clientRace, fd);
        AEFile_ReadInt(&costs, fd);
        AEFile_ReadInt(&bonus, fd);
        AEFile_ReadInt(&reward, fd);
        AEFile_ReadInt(&targetStationIdx, fd);
        AEFile_ReadInt(&difficulty, fd);
        AEFile_ReadInt(&prodIdx, fd);
        AEFile_ReadInt(&prodAmt, fd);
        AEFile_ReadInt(&statusValue, fd);
        AEFile_ReadBool(&visible, fd);

        int hasAgent = 0;
        AEFile_ReadInt(&hasAgent, fd);
        void *agent = (hasAgent < 1) ? 0 : RecordHandler_readAgent(self, fd);

        if (!isEmpty) {
            mission = RH_op_new(0x78);
            char nameCopy[12];
            AEString_copy_ctor(nameCopy, clientName, 0);
            Mission_ctorFull(mission, type, nameCopy, img, clientRace, reward, targetStationIdx,
                             difficulty);
            AEString_dtor(nameCopy);
        } else {
            mission = RH_op_new(0x78);
            Mission_ctorEmpty(mission, type, reward, targetStationIdx);
        }
        Mission_setCosts(mission, costs);
        Mission_setBonus(mission, bonus);
        Mission_setProductionGoods(mission, prodIdx, prodAmt);
        Mission_setStatusValue(mission, statusValue);
        Mission_setVisible(mission, visible);
        Mission_setAgent(mission, agent);

        char tgtNameCopy[12];
        AEString_copy_ctor(tgtNameCopy, targetName, 0);
        Mission_setTargetName(mission, tgtNameCopy);
        AEString_dtor(tgtNameCopy);

        if (agent != 0) {
            Agent_setMission(agent, mission);
        }

        AEString_dtor(targetSystem);
        AEString_dtor(targetStation);
        AEString_dtor(targetName);
        AEString_dtor(clientName);
    }

    if (*guardP == saved) {
        return mission;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
}

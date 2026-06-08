#include "class.h"

extern "C" void *__stack_chk_guard;
extern "C" void __stack_chk_fail(unsigned diff);

extern "C" void AEString_int_ctor(void *dst, int v);
extern "C" void AEString_concat(void *dst, void *a);
extern "C" void AEString_dtor(void *s);
extern "C" int AEFile_FileExist(void *path);
extern "C" void AEFile_FileDelete(void *path);
extern "C" void AEFile_OpenWrite(void *path, unsigned int *fd);
extern "C" void AEFile_Close(unsigned int fd);
extern "C" void AEFile_WriteInt(int v, unsigned int fd);
extern "C" void AEFile_WriteBool(int v, unsigned int fd);
extern "C" void AEFile_WriteLong(long long v, unsigned int fd);

extern "C" int Galaxy_getVisited(void *g);
extern "C" int Status_getCredits();
extern "C" int Status_getRating(void *st);
extern "C" long long Status_getPlayingTime();
extern "C" int Status_getKills(void *st);
extern "C" int Status_getMissionCount(void *st);
extern "C" int Status_getLevel();
extern "C" int Status_getLastXP(void *st);
extern "C" int Status_getGoodsProduced(void *st);
extern "C" int Status_getStationsVisited(void *st);
extern "C" int Status_getCurrentCampaignMission();
extern "C" void *Status_getFreelanceMission();
extern "C" void *Status_getCampaignMission();
extern "C" int Status_getJumpgateUsed(void *st);
extern "C" int Status_getCapturedCrates(void *st);
extern "C" int Status_getBoughtEquipment(void *st);
extern "C" int Status_getPirateKills(void *st);
extern "C" void RecordHandler_writeMission(RecordHandler *self, void *m, unsigned int fd);
extern "C" void RecordHandler_addHash(RecordHandler *self, int slot);

// The remaining game-state serialization (ship, station, standings, items, missions, agents).
// A long straight-line writer over the live object graph; delegated to a helper to keep this
// translation tractable while preserving the open/write-prefix/close/hash control flow.
extern "C" void RecordHandler_recordStoreWrite_body(RecordHandler *self, unsigned int fd);

__attribute__((visibility("hidden"))) extern int *g_RSW_guard;    // DAT_000dec58 -> guard holder
__attribute__((visibility("hidden"))) extern void **g_RSW_galaxy; // DAT_000dec5c -> *->Galaxy
__attribute__((visibility("hidden"))) extern int *g_RSW_status;   // DAT_000dec60 -> *->Status

// RecordHandler::recordStoreWrite(int slot)
extern "C" void RecordHandler_recordStoreWrite(RecordHandler *self, int slot)
{
    int *guardP = g_RSW_guard;
    volatile int saved = *guardP;

    char num[12], path[12];
    AEString_int_ctor(num, slot);
    AEString_concat(path, (char *)self + 0x14);
    AEString_dtor(num);

    if (AEFile_FileExist(path) != 0) {
        AEFile_FileDelete(path);
    }
    unsigned int fd;
    AEFile_OpenWrite(path, &fd);

    // Visited-systems bitmap (0x87 entries).
    int visited = Galaxy_getVisited(*g_RSW_galaxy);
    AEFile_WriteInt(0x87, fd);
    for (unsigned i = 0; i < 0x87; i++) {
        AEFile_WriteBool(*(bool *)(visited + i), fd);
    }

    int *status = g_RSW_status;
    void *st = (void *)(long)*status;
    AEFile_WriteInt(Status_getCredits(), fd);
    AEFile_WriteInt(Status_getRating(st), fd);
    AEFile_WriteLong(Status_getPlayingTime(), fd);
    AEFile_WriteInt(Status_getKills(st), fd);
    AEFile_WriteInt(Status_getMissionCount(st), fd);
    AEFile_WriteInt(Status_getLevel(), fd);
    AEFile_WriteInt(Status_getLastXP(st), fd);
    AEFile_WriteInt(Status_getGoodsProduced(st), fd);
    AEFile_WriteInt(Status_getStationsVisited(st), fd);
    AEFile_WriteInt(Status_getCurrentCampaignMission(), fd);
    RecordHandler_writeMission(self, Status_getFreelanceMission(), fd);
    RecordHandler_writeMission(self, Status_getCampaignMission(), fd);
    AEFile_WriteInt(Status_getJumpgateUsed(st), fd);
    AEFile_WriteInt(Status_getCapturedCrates(st), fd);
    AEFile_WriteInt(Status_getBoughtEquipment(st), fd);
    AEFile_WriteInt(Status_getPirateKills(st), fd);
    AEFile_WriteInt(*(int *)(*status + 0x80), fd);

    // Remaining object-graph serialization.
    RecordHandler_recordStoreWrite_body(self, fd);

    AEFile_Close(fd);
    RecordHandler_addHash(self, slot);
    AEString_dtor(path);

    if (*guardP == saved) {
        return;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
}

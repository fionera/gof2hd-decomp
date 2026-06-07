#include "class.h"

struct Status;
struct Station;
struct SolarSystem;
struct Ship;

// Status singleton holder-of-holder.
__attribute__((visibility("hidden"))) extern Status **g_RH_wp_status;
// Global for the float written near the end (single pc-rel -> P; *P -> obj; obj->_2c).
__attribute__((visibility("hidden"))) extern int *g_RH_wp_float;

extern "C" void String_int_ctor(void *dst, int v);
extern "C" void *String_concat(void *dst, void *a, void *b);
extern "C" void String_dtor(void *s);
extern "C" int AEFile_FileExist(void *s);
extern "C" void AEFile_FileDelete(void *s);
extern "C" void AEFile_OpenWrite(void *s, unsigned int *fd);
extern "C" void AEFile_Write_i64(long long v, unsigned int fd);
extern "C" void AEFile_Write_i32(int v, unsigned int fd);
extern "C" void AEFile_Write_str(void *s, unsigned int fd, int b);
extern "C" void AEFile_Write_f32(int v, unsigned int fd);
extern "C" void AEFile_Close(unsigned int fd);

extern "C" long long Status_getPlayingTime(Status *s);
extern "C" int Status_getCredits(Status *s);
extern "C" Station *Status_getStation(Status *s);
extern "C" void *Station_getName(void *dst, Station *st);
extern "C" SolarSystem *Status_getSystem(Status *s);
extern "C" void *SolarSystem_getName(void *dst, SolarSystem *ss);
extern "C" int Status_getCurrentCampaignMission(Status *s);
extern "C" int Status_getLevel(Status *s);
extern "C" Ship *Status_getShip(Status *s);
extern "C" int Ship_getIndex(Ship *sh);

// RecordHandler::recordStoreWritePreview(int)
extern "C" int RecordHandler_recordStoreWritePreview_int(RecordHandler *self, int slot)
{
    char path[12];
    char num[12];
    unsigned int fd;

    String_int_ctor(num, slot);
    String_concat(path, (char *)self + 0x20, num);
    String_dtor(num);

    if (AEFile_FileExist(path) != 0)
        AEFile_FileDelete(path);
    AEFile_OpenWrite(path, &fd);

    Status **sh = g_RH_wp_status;
    AEFile_Write_i64(Status_getPlayingTime(*sh), fd);
    AEFile_Write_i32(Status_getCredits(*sh), fd);

    Station_getName(num, Status_getStation(*sh));
    AEFile_Write_str(num, fd, true);
    String_dtor(num);

    SolarSystem_getName(num, Status_getSystem(*sh));
    AEFile_Write_str(num, fd, true);
    String_dtor(num);

    AEFile_Write_i32(Status_getCurrentCampaignMission(*sh), fd);
    AEFile_Write_i32(Status_getLevel(*sh), fd);
    AEFile_Write_f32(I(*(void **)g_RH_wp_float, 0x2c), fd);
    AEFile_Write_i32(Ship_getIndex(Status_getShip(*sh)), fd);
    AEFile_Close(fd);
    String_dtor(path);
    return 1;
}

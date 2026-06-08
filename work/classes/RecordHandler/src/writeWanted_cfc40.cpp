#include "class.h"

extern "C" void *__stack_chk_guard;
extern "C" void __stack_chk_fail(unsigned diff);

extern "C" void AEFile_WriteBool(int v, unsigned int fd);
extern "C" void AEFile_WriteInt(int v, unsigned int fd);
extern "C" void AEFile_WriteString(void *s, unsigned int fd, int flag);
extern "C" void AEString_dtor(void *s);

extern "C" int Wanted_isActive(void *w);
extern "C" int Wanted_isTerminated(void *w);
extern "C" int Wanted_getCurrentLocation(void *w);
extern "C" int Wanted_getTravelsTo(void *w);
extern "C" int Wanted_getLastSeen(void *w);
extern "C" void Wanted_getName(void *out, void *w);
extern "C" int Wanted_getIndex(void *w);
extern "C" int Wanted_getBoard(void *w);
extern "C" int Wanted_getRace(void *w);
extern "C" int Wanted_isMale(void *w);
extern "C" int Wanted_getShip(void *w);
extern "C" int Wanted_getWeapon(void *w);
extern "C" int Wanted_getHitpoints(void *w);
extern "C" int Wanted_getLoot(void *w);
extern "C" int Wanted_getLootAmount(void *w);
extern "C" int Wanted_getReward(void *w);
extern "C" int Wanted_getRequiredBounties(void *w);
extern "C" int Wanted_getRequiredMission(void *w);
extern "C" int Wanted_getNumWingmen(void *w);
extern "C" int *Wanted_getImageParts(void *w);

__attribute__((visibility("hidden"))) extern int *g_WW_guard;   // DAT_000dfd70 -> guard holder

// RecordHandler::writeWanted(Wanted*, unsigned int fd)
extern "C" void RecordHandler_writeWanted(RecordHandler *self, void *w, unsigned int fd)
{
    (void)self;
    int *guardP = g_WW_guard;
    volatile int saved = *guardP;

    AEFile_WriteBool(Wanted_isActive(w), fd);
    AEFile_WriteBool(Wanted_isTerminated(w), fd);
    AEFile_WriteInt(Wanted_getCurrentLocation(w), fd);
    AEFile_WriteInt(Wanted_getTravelsTo(w), fd);
    AEFile_WriteInt(Wanted_getLastSeen(w), fd);

    char name[12];
    Wanted_getName(name, w);
    AEFile_WriteString(name, fd, 1);
    AEString_dtor(name);

    AEFile_WriteInt(Wanted_getIndex(w), fd);
    AEFile_WriteInt(Wanted_getBoard(w), fd);
    AEFile_WriteInt(Wanted_getRace(w), fd);
    AEFile_WriteBool(Wanted_isMale(w), fd);
    AEFile_WriteInt(Wanted_getShip(w), fd);
    AEFile_WriteInt(Wanted_getWeapon(w), fd);
    AEFile_WriteInt(Wanted_getHitpoints(w), fd);
    AEFile_WriteInt(Wanted_getLoot(w), fd);
    AEFile_WriteInt(Wanted_getLootAmount(w), fd);
    AEFile_WriteInt(Wanted_getReward(w), fd);
    AEFile_WriteInt(Wanted_getRequiredBounties(w), fd);
    AEFile_WriteInt(Wanted_getRequiredMission(w), fd);
    AEFile_WriteInt(Wanted_getNumWingmen(w), fd);

    for (int i = 0; i != 5; i++) {
        int *parts = Wanted_getImageParts(w);
        AEFile_WriteInt(parts[i], fd);
    }

    if (*guardP == saved) {
        return;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
}

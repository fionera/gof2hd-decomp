#include "class.h"

extern "C" void *__stack_chk_guard;
extern "C" void __stack_chk_fail(unsigned diff);

extern "C" void AEFile_ReadBool(void *out, unsigned int fd);
extern "C" void AEFile_ReadInt(void *out, unsigned int fd);
extern "C" void AEFile_ReadString(void *out, unsigned int fd, int flag);
extern "C" unsigned char *RH_op_new_arr(unsigned int n);
extern "C" void *RH_op_new(unsigned int sz);
extern "C" void AEString_default_ctor(void *s);
extern "C" void AEString_copy_ctor(void *dst, void *src, int copy);
extern "C" void AEString_dtor(void *s);
extern "C" void Wanted_ctor(void *self, int idx, void *name, int board, int race, bool male,
                            int ship, int weapon, int hp, int loot, int lootAmt, int reward,
                            int reqBounties, int reqMission, int numWingmen);
extern "C" void Wanted_setImageParts(void *w, int *parts);
extern "C" void Wanted_setActive(void *w, bool v);
extern "C" void Wanted_setTerminated(void *w, bool v);
extern "C" void Wanted_setCurrentLocation(void *w, int v);
extern "C" void Wanted_setTravelsTo(void *w, int v);
extern "C" void Wanted_setLastSeen(void *w, int v);

__attribute__((visibility("hidden"))) extern int *g_RW_guard;   // DAT_000de8a4 -> guard holder

// RecordHandler::readWanted(unsigned int fd) -> Wanted* (in r0 on return).
extern "C" void *RecordHandler_readWanted(RecordHandler *self, unsigned int fd)
{
    (void)self;
    int *guardP = g_RW_guard;
    volatile int saved = *guardP;

    bool active = false;
    bool terminated = false;
    int currentLocation = -1;
    int travelsTo = -1;
    int lastSeen = -1;
    AEFile_ReadBool(&active, fd);
    AEFile_ReadBool(&terminated, fd);
    AEFile_ReadInt(&currentLocation, fd);
    AEFile_ReadInt(&travelsTo, fd);
    AEFile_ReadInt(&lastSeen, fd);

    char name[12];
    AEString_default_ctor(name);

    int idx = 0, board = 0, race = 0;
    bool male = true;
    int ship = 0, weapon = 0, hp = 0, loot = 0, lootAmt = 0;
    int reward = 0, reqBounties = 0, reqMission = 0, numWingmen = 0;

    AEFile_ReadString(name, fd, 1);
    AEFile_ReadInt(&idx, fd);
    AEFile_ReadInt(&board, fd);
    AEFile_ReadInt(&race, fd);
    AEFile_ReadBool(&male, fd);
    AEFile_ReadInt(&ship, fd);
    AEFile_ReadInt(&weapon, fd);
    AEFile_ReadInt(&hp, fd);
    AEFile_ReadInt(&loot, fd);
    AEFile_ReadInt(&lootAmt, fd);
    AEFile_ReadInt(&reward, fd);
    AEFile_ReadInt(&reqBounties, fd);
    AEFile_ReadInt(&reqMission, fd);
    AEFile_ReadInt(&numWingmen, fd);

    void *w = RH_op_new(0x54);
    char nameCopy[12];
    AEString_copy_ctor(nameCopy, name, 0);
    Wanted_ctor(w, idx, nameCopy, board, race, male, ship, weapon, hp, loot, lootAmt, reward,
                reqBounties, reqMission, numWingmen);
    AEString_dtor(nameCopy);

    int *parts = (int *)RH_op_new_arr(0x14);
    int *p = parts;
    for (unsigned i = 0; i < 5; i++) {
        AEFile_ReadInt(p, fd);
        p++;
    }
    Wanted_setImageParts(w, parts);
    Wanted_setActive(w, active);
    Wanted_setTerminated(w, terminated);
    Wanted_setCurrentLocation(w, currentLocation);
    Wanted_setTravelsTo(w, travelsTo);
    Wanted_setLastSeen(w, lastSeen);
    AEString_dtor(name);

    if (*guardP == saved) {
        return w;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
}

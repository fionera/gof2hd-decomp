#include "class.h"

extern "C" void *operator_new(unsigned int sz);
extern "C" void ArrayInt_ctor(Array<int> *a);
// hidden PC-relative global: the App singleton pointer, deref'd twice.
extern void *const gMissionCrateApp __attribute__((visibility("hidden")));
extern "C" int Status_getMission(unsigned status);
extern "C" int Mission_getType(int mission);
extern "C" void ArrayInt_add(int val, Array<int> *a);
extern "C" void PlayerFighter_setMissionCrate_tail(int one, Array<int> *a);

extern "C" void PlayerFighter_setMissionCrate(PlayerFighter *self, bool on)
{
    F<uint8_t>(self, 0xd0) = on;
    if (on) {
        F<Array<int> *>(self, 0x50) = 0;
        Array<int> *a = (Array<int> *)operator_new(0xc);
        ArrayInt_ctor(a);
        F<Array<int> *>(self, 0x50) = a;
        int mission = Status_getMission(*(unsigned *)gMissionCrateApp);
        int type = Mission_getType(mission);
        int item = (type == 5) ? 0x74 : 0x75;
        ArrayInt_add(item, F<Array<int> *>(self, 0x50));
        PlayerFighter_setMissionCrate_tail(1, F<Array<int> *>(self, 0x50));
    }
}

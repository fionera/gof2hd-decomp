#include "class.h"

extern "C" int HangarList_getCurrentTab(void *list);
extern "C" int BluePrint_isEmpty(void *bp);
extern "C" int BluePrint_getStationIndex(void *bp);
extern "C" int Station_getIndex(void *station);
// Status singleton (pc-rel deref -> holder; *holder is the Status object).
__attribute__((visibility("hidden"))) extern void **g_hw_status;
extern "C" void *Status_getStation(void *status);

extern "C" bool HangarWindow_readyToClose(HangarWindow *self)
{
    int tab = HangarList_getCurrentTab(F<void *>(self, 0x14));
    if (tab == 4 && F<uint8_t>(self, 0x88) != 0 && F<int>(self, 0x94) > 0 &&
        F<uint8_t>(self, 0x3c) == 0 && BluePrint_isEmpty(F<void *>(self, 0x80)) == 0) {
        int si = BluePrint_getStationIndex(F<void *>(self, 0x80));
        void *st = Status_getStation(*g_hw_status);
        if (si != Station_getIndex(st)) {
            return false;
        }
    }
    return F<uint8_t>(self, 0x3c) == 0;
}

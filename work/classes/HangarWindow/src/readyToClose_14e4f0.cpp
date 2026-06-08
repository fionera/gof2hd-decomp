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
    int tab = HangarList_getCurrentTab(self->f_14);
    if (tab == 4 && self->f_88 != 0 && self->f_94 > 0 &&
        self->f_3c == 0 && BluePrint_isEmpty(self->f_80) == 0) {
        int si = BluePrint_getStationIndex(self->f_80);
        void *st = Status_getStation(*g_hw_status);
        if (si != Station_getIndex(st)) {
            return false;
        }
    }
    return self->f_3c == 0;
}

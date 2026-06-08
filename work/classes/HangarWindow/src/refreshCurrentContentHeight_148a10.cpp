#include "class.h"

extern "C" int *HangarList_getCurrentTabItems(void *list);
// Globals singleton (pc-rel deref -> holder; *holder is the object), field +0x70 is the row height.
__attribute__((visibility("hidden"))) extern void **g_hw_globals;

extern "C" void HangarWindow_refreshCurrentContentHeight(HangarWindow *self)
{
    int *items = HangarList_getCurrentTabItems(F<void *>(self, 0x14));
    if (items != 0) {
        int n = *items;
        int rowH = G<int>(*g_hw_globals, 0x70);
        F<int>(self, 0xd4) = F<int>(self, 0x10c) * (n - 1) + n * rowH;
    }
}

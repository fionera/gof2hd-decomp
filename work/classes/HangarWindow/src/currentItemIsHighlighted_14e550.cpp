#include "class.h"

extern "C" void *HangarList_getCurrentItem(void *list);

extern "C" bool HangarWindow_currentItemIsHighlighted(HangarWindow *self)
{
    void *item = HangarList_getCurrentItem(F<void *>(self, 0x14));
    if (item == 0) return false;
    return item == F<void *>(self, 0x68);
}

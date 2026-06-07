#include "class.h"

extern "C" int HangarList_getCurrentTab(void *list);

extern "C" int HangarWindow_getCurrentTab(HangarWindow *self)
{
    return HangarList_getCurrentTab(F<void *>(self, 0x14));
}

#include "class.h"

extern "C" int HangarList_getCurrentTab(void *list);

extern "C" int HangarWindow_getCurrentTab(HangarWindow *self)
{
    return HangarList_getCurrentTab(self->f_14);
}

#include "class.h"

extern "C" void *HangarList_getCurrentItem(void *list);

extern "C" bool HangarWindow_currentItemIsHighlighted(HangarWindow *self)
{
    void *item = HangarList_getCurrentItem(self->f_14);
    if (item == 0) return false;
    return item == self->f_68;
}

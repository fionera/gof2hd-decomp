#include "class.h"

extern "C" void *HangarList_dtor(void *p);
extern "C" void *ListItemWindow_dtor(void *p);
extern "C" void *ChoiceWindow_dtor(void *p);
extern "C" void ArrayReleaseClasses_TouchButton(void *arr);
extern "C" void *Array_TouchButton_dtor(void *p);
extern "C" void operator_delete(void *p);
extern "C" void operator_delete_arr(void *p);

// HangarWindow::~HangarWindow() -> returns this. Tears down owned windows/lists/arrays.
extern "C" HangarWindow *_ZN12HangarWindowD2Ev(HangarWindow *self)
{
    void *p;
    p = F<void *>(self, 0x14);
    if (p != 0) operator_delete(HangarList_dtor(p));
    F<void *>(self, 0x14) = 0;
    p = F<void *>(self, 0x18);
    if (p != 0) operator_delete(ListItemWindow_dtor(p));
    F<void *>(self, 0x18) = 0;
    p = F<void *>(self, 0x1c);
    if (p != 0) operator_delete(ChoiceWindow_dtor(p));
    F<void *>(self, 0x1c) = 0;
    p = F<void *>(self, 0x20);
    if (p != 0) operator_delete(ChoiceWindow_dtor(p));
    F<void *>(self, 0x20) = 0;
    p = F<void *>(self, 0x4);
    if (p != 0) {
        ArrayReleaseClasses_TouchButton(p);
        void *q = F<void *>(self, 0x4);
        if (q != 0) operator_delete(Array_TouchButton_dtor(q));
    }
    F<void *>(self, 0x4) = 0;
    p = F<void *>(self, 0x24);
    if (p != 0) {
        ArrayReleaseClasses_TouchButton(p);
        void *q = F<void *>(self, 0x24);
        if (q != 0) operator_delete(Array_TouchButton_dtor(q));
    }
    p = F<void *>(self, 0x30);
    F<uint8_t>(self, 0xc) = 0;
    F<void *>(self, 0x24) = 0;
    if (p != 0) operator_delete_arr(p);
    F<void *>(self, 0x30) = 0;
    return self;
}

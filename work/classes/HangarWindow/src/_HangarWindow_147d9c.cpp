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
    p = self->f_14;
    if (p != 0) operator_delete(HangarList_dtor(p));
    self->f_14 = 0;
    p = self->f_18;
    if (p != 0) operator_delete(ListItemWindow_dtor(p));
    self->f_18 = 0;
    p = self->f_1c;
    if (p != 0) operator_delete(ChoiceWindow_dtor(p));
    self->f_1c = 0;
    p = self->f_20;
    if (p != 0) operator_delete(ChoiceWindow_dtor(p));
    self->f_20 = 0;
    p = self->f_4;
    if (p != 0) {
        ArrayReleaseClasses_TouchButton(p);
        void *q = self->f_4;
        if (q != 0) operator_delete(Array_TouchButton_dtor(q));
    }
    self->f_4 = 0;
    p = self->f_24;
    if (p != 0) {
        ArrayReleaseClasses_TouchButton(p);
        void *q = self->f_24;
        if (q != 0) operator_delete(Array_TouchButton_dtor(q));
    }
    p = self->f_30;
    self->f_c = 0;
    self->f_24 = 0;
    if (p != 0) operator_delete_arr(p);
    self->f_30 = 0;
    return self;
}

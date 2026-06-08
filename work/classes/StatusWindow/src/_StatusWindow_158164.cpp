#include "class.h"

extern "C" void ArrayReleaseClasses_TouchButton(void *arr);
extern "C" void *Array_TouchButton_dtor(void *p);
extern "C" void ArrayReleaseClasses_ImagePart(void *arr);
extern "C" void *Array_ImagePart_dtor(void *p);
extern "C" void ArrayReleaseClasses_String(void *arr);
extern "C" void *Array_String_dtor(void *p);
extern "C" void operator_delete(void *p);

// StatusWindow::~StatusWindow() -> returns this. Tears down the 4 owned Arrays.
extern "C" StatusWindow *_ZN12StatusWindowD2Ev(StatusWindow *self)
{
    void *p = self->f_4;
    if (p != 0) {
        ArrayReleaseClasses_TouchButton(p);
        void *q = self->f_4;
        if (q != 0) operator_delete(Array_TouchButton_dtor(q));
    }
    self->f_4 = 0;
    p = self->f_8;
    if (p != 0) {
        ArrayReleaseClasses_TouchButton(p);
        void *q = self->f_8;
        if (q != 0) operator_delete(Array_TouchButton_dtor(q));
    }
    self->f_8 = 0;
    p = self->f_c;
    if (p != 0) {
        ArrayReleaseClasses_ImagePart(p);
        void *q = self->f_c;
        if (q != 0) operator_delete(Array_ImagePart_dtor(q));
    }
    self->f_c = 0;
    p = self->f_10;
    if (p != 0) {
        ArrayReleaseClasses_String(p);
        void *q = self->f_10;
        if (q != 0) operator_delete(Array_String_dtor(q));
    }
    self->f_10 = 0;
    return self;
}

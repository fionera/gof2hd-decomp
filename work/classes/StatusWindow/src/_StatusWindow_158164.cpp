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
    void *p = F<void *>(self, 0x4);
    if (p != 0) {
        ArrayReleaseClasses_TouchButton(p);
        void *q = F<void *>(self, 0x4);
        if (q != 0) operator_delete(Array_TouchButton_dtor(q));
    }
    F<void *>(self, 0x4) = 0;
    p = F<void *>(self, 0x8);
    if (p != 0) {
        ArrayReleaseClasses_TouchButton(p);
        void *q = F<void *>(self, 0x8);
        if (q != 0) operator_delete(Array_TouchButton_dtor(q));
    }
    F<void *>(self, 0x8) = 0;
    p = F<void *>(self, 0xc);
    if (p != 0) {
        ArrayReleaseClasses_ImagePart(p);
        void *q = F<void *>(self, 0xc);
        if (q != 0) operator_delete(Array_ImagePart_dtor(q));
    }
    F<void *>(self, 0xc) = 0;
    p = F<void *>(self, 0x10);
    if (p != 0) {
        ArrayReleaseClasses_String(p);
        void *q = F<void *>(self, 0x10);
        if (q != 0) operator_delete(Array_String_dtor(q));
    }
    F<void *>(self, 0x10) = 0;
    return self;
}

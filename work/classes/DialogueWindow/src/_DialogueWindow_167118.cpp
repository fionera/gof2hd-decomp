#include "class.h"

extern "C" void ArrayReleaseClasses_ImagePartPtr(void *self);
extern "C" void *Array_ImagePartPtr_dtor(void *self);
extern "C" void *ScrollTouchWindow_dtor(void *self);
extern "C" void *TouchButton_dtor(void *self);
extern "C" void String_dtor(String *self);
extern "C" void operator_delete(void *self);
extern "C" void operator_delete_arr(void *self);

extern "C" DialogueWindow *_ZN14DialogueWindowD2Ev(DialogueWindow *self)
{
    void *p = F<void *>(self, 0x0c);
    if (p != 0) {
        ArrayReleaseClasses_ImagePartPtr(p);
        p = F<void *>(self, 0x0c);
        if (p != 0) {
            operator_delete(Array_ImagePartPtr_dtor(p));
        }
    }
    F<void *>(self, 0x0c) = 0;

    p = F<void *>(self, 0x5c);
    if (p != 0) operator_delete_arr(p);
    F<void *>(self, 0x5c) = 0;

    p = F<void *>(self, 0x60);
    if (p != 0) operator_delete_arr(p);
    F<void *>(self, 0x60) = 0;

    p = F<void *>(self, 0x40);
    if (p != 0) operator_delete(ScrollTouchWindow_dtor(p));
    F<void *>(self, 0x40) = 0;

    p = F<void *>(self, 0x00);
    if (p != 0) operator_delete(TouchButton_dtor(p));
    F<void *>(self, 0x00) = 0;

    p = F<void *>(self, 0x04);
    if (p != 0) operator_delete(TouchButton_dtor(p));
    F<void *>(self, 0x04) = 0;

    p = F<void *>(self, 0x08);
    if (p != 0) operator_delete(TouchButton_dtor(p));
    F<void *>(self, 0x08) = 0;

    String_dtor((String *)((char *)self + 0x34));
    String_dtor((String *)((char *)self + 0x28));
    return self;
}

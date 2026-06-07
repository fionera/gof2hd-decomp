#include "class.h"

extern "C" void ArrayReleaseClasses_ImagePart(void *arr);
extern "C" void *Array_ImagePart_dtor(void *arr);
extern "C" void ArrayReleaseClasses_TouchButton(void *arr);
extern "C" void *Array_TouchButton_dtor(void *arr);
extern "C" void *TouchButton_dtor(void *button);
extern "C" void *Array_Wanted_dtor(void *arr);
extern "C" void *ScrollTouchWindow_dtor(void *window);

__attribute__((visibility("hidden"))) extern void (*g_WantedWindow_string_dtor)(void *);

extern "C" WantedWindow *_ZN12WantedWindowD2Ev(WantedWindow *self)
{
    void *p = F<void *>(self, 0x8);
    if (p != 0) {
        ArrayReleaseClasses_ImagePart(p);
        p = F<void *>(self, 0x8);
        if (p != 0) {
            operator_delete(Array_ImagePart_dtor(p));
        }
    }
    F<void *>(self, 0x8) = 0;

    p = F<void *>(self, 0x0c);
    if (p != 0) {
        ArrayReleaseClasses_TouchButton(p);
        p = F<void *>(self, 0x0c);
        if (p != 0) {
            operator_delete(Array_TouchButton_dtor(p));
        }
    }
    F<void *>(self, 0x0c) = 0;

    p = F<void *>(self, 0x18);
    if (p != 0) {
        operator_delete(TouchButton_dtor(p));
    }
    F<void *>(self, 0x18) = 0;

    p = F<void *>(self, 0x38);
    if (p != 0) {
        operator_delete(Array_Wanted_dtor(p));
    }
    F<void *>(self, 0x38) = 0;

    p = F<void *>(self, 0xb0);
    if (p != 0) {
        (*(void (**)(void *))(*(int *)p + 4))(p);
    }
    F<void *>(self, 0xb0) = 0;

    p = F<void *>(self, 0x2c);
    if (p != 0) {
        operator_delete(ScrollTouchWindow_dtor(p));
    }
    F<void *>(self, 0x2c) = 0;

    void (*stringDtor)(void *) = g_WantedWindow_string_dtor;
    stringDtor((char *)self + 0x78);
    stringDtor((char *)self + 0x6c);
    stringDtor((char *)self + 0x60);
    stringDtor((char *)self + 0x54);
    stringDtor((char *)self + 0x48);
    stringDtor((char *)self + 0x3c);
    return self;
}

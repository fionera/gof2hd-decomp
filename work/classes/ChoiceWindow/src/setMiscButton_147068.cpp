#include "class.h"

void *operator new(__SIZE_TYPE__ size);

extern "C" int TouchButton_getWidth(void *self);
extern "C" void TouchButton_ctor(void *self, String const &text, int value, int x, int y,
                                  int width, int anchor, int mode);
extern "C" void TouchButton_setPosition(void *self, int x, int y, int anchor);
extern "C" void TouchButton_translate(void *self, int x, int y);
extern "C" void ScrollTouchWindow_setPosition(void *self, int y);

__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_layout_147068_a;
__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_layout_147068_b;
__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_layout_147068_c;
__attribute__((visibility("hidden"))) extern int *g_ChoiceWindow_screenHeight_147068;

void ChoiceWindow::setMiscButton(String const &text)
{
    void *layout;
    int padding = F<int>(this, 0x48);
    int buttonWidth;

    if (F<void *>(this, 0x10) == 0 || F<void *>(this, 0x14) == 0) {
        layout = *g_ChoiceWindow_layout_147068_b;
        buttonWidth = padding + F<int>(layout, 0x40) * 2;
    } else {
        int leftWidth = TouchButton_getWidth(F<void *>(this, 0x10));
        int rightWidth = TouchButton_getWidth(F<void *>(this, 0x14));
        layout = *g_ChoiceWindow_layout_147068_a;
        buttonWidth = leftWidth + rightWidth + padding;
    }

    void *button = operator new(0xc8);
    TouchButton_ctor(button, text, 0,
                     F<int>(this, 0x0) + F<int>(this, 0x8) / 2 - F<int>(layout, 0x4c) / 2,
                     F<int>(this, 0x4) + F<int>(this, 0xc) - padding,
                     buttonWidth, 0x22, 4);

    F<void *>(this, 0x18) = button;

    void **layoutHolder = g_ChoiceWindow_layout_147068_c;
    layout = *layoutHolder;
    setHeight((F<int>(this, 0xc) - F<int>(layout, 0x8)) + F<int>(this, 0x48) * 2 +
              F<int>(layout, 0x30));

    TouchButton_setPosition(F<void *>(this, 0x18),
                            F<int>(this, 0x0) + F<int>(this, 0x8) / 2,
                            F<int>(this, 0x4) + F<int>(this, 0xc) - F<int>(this, 0x48),
                            0x24);

    int delta = -(F<int>(*layoutHolder, 0x30) + F<int>(this, 0x48));
    if (F<void *>(this, 0x10) != 0)
        TouchButton_translate(F<void *>(this, 0x10), 0, delta);
    if (F<void *>(this, 0x14) != 0)
        TouchButton_translate(F<void *>(this, 0x14), 0, delta);

    ScrollTouchWindow_setPosition(F<void *>(this, 0x1c),
                                  *g_ChoiceWindow_screenHeight_147068 / 2 -
                                      F<int>(this, 0xc) / 2 + F<int>(*layoutHolder, 0x8));
}

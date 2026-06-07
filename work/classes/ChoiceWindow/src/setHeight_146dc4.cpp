#include "class.h"

extern "C" void TouchButton_setPosition(void *self, int x, int y, int anchor);

__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_layout_146dc4;
__attribute__((visibility("hidden"))) extern int *g_ChoiceWindow_screenHeight_146dc4;

void ChoiceWindow::setHeight(int height)
{
    void **layoutHolder = g_ChoiceWindow_layout_146dc4;
    void *layout = *layoutHolder;
    int top = F<int>(layout, 0x8);
    F<int>(this, 0xc) = top + height;

    void *button = F<void *>(this, 0x10);
    int yBase = *g_ChoiceWindow_screenHeight_146dc4 / 2 - height / 2;
    F<int>(this, 0x4) = yBase - top;

    if (F<uint8_t>(this, 0x58) == 0) {
        if (button == 0)
            return;
        TouchButton_setPosition(button,
                                F<int>(this, 0x0) + F<int>(this, 0x8) / 2,
                                height + yBase - F<int>(this, 0x48),
                                0x24);
        return;
    }

    if (button != 0) {
        TouchButton_setPosition(button,
                                F<int>(this, 0x0) + F<int>(this, 0x8) / 2 - F<int>(layout, 0x4c) / 2,
                                height + yBase - F<int>(this, 0x48),
                                0x22);
    }

    button = F<void *>(this, 0x14);
    if (button == 0)
        return;

    layout = *layoutHolder;
    TouchButton_setPosition(button,
                            F<int>(this, 0x0) + F<int>(this, 0x8) / 2 + F<int>(layout, 0x4c) / 2,
                            F<int>(this, 0x4) + F<int>(this, 0xc) - F<int>(this, 0x48),
                            0x21);
}

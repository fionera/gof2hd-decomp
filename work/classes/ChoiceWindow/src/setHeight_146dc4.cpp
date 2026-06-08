#include "class.h"

extern "C" void TouchButton_setPosition(void *self, int x, int y, int anchor);

__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_layout_146dc4;
__attribute__((visibility("hidden"))) extern int *g_ChoiceWindow_screenHeight_146dc4;

void ChoiceWindow::setHeight(int height)
{
    void **layoutHolder = g_ChoiceWindow_layout_146dc4;
    void *layout = *layoutHolder;
    int top = layout->f_8;
    this->f_c = top + height;

    void *button = this->f_10;
    int yBase = *g_ChoiceWindow_screenHeight_146dc4 / 2 - height / 2;
    this->f_4 = yBase - top;

    if (this->f_58 == 0) {
        if (button == 0)
            return;
        TouchButton_setPosition(button,
                                this->f_0 + this->f_8 / 2,
                                height + yBase - this->f_48,
                                0x24);
        return;
    }

    if (button != 0) {
        TouchButton_setPosition(button,
                                this->f_0 + this->f_8 / 2 - layout->f_4c / 2,
                                height + yBase - this->f_48,
                                0x22);
    }

    button = this->f_14;
    if (button == 0)
        return;

    layout = *layoutHolder;
    TouchButton_setPosition(button,
                            this->f_0 + this->f_8 / 2 + layout->f_4c / 2,
                            this->f_4 + this->f_c - this->f_48,
                            0x21);
}

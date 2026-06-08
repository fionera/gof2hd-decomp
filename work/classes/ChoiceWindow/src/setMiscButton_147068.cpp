#include "class.h"

void *operator new(unsigned int size);

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
    int padding = this->f_48;
    int buttonWidth;

    if (this->f_10 == 0 || this->f_14 == 0) {
        layout = *g_ChoiceWindow_layout_147068_b;
        buttonWidth = padding + layout->f_40 * 2;
    } else {
        int leftWidth = TouchButton_getWidth(this->f_10);
        int rightWidth = TouchButton_getWidth(this->f_14);
        layout = *g_ChoiceWindow_layout_147068_a;
        buttonWidth = leftWidth + rightWidth + padding;
    }

    void *button = operator new(0xc8);
    TouchButton_ctor(button, text, 0,
                     this->f_0 + this->f_8 / 2 - layout->f_4c / 2,
                     this->f_4 + this->f_c - padding,
                     buttonWidth, 0x22, 4);

    this->f_18 = button;

    void **layoutHolder = g_ChoiceWindow_layout_147068_c;
    layout = *layoutHolder;
    setHeight((this->f_c - layout->f_8) + this->f_48 * 2 +
              layout->f_30);

    TouchButton_setPosition(this->f_18,
                            this->f_0 + this->f_8 / 2,
                            this->f_4 + this->f_c - this->f_48,
                            0x24);

    int delta = -(F<int>(*layoutHolder, 0x30) + this->f_48);
    if (this->f_10 != 0)
        TouchButton_translate(this->f_10, 0, delta);
    if (this->f_14 != 0)
        TouchButton_translate(this->f_14, 0, delta);

    ScrollTouchWindow_setPosition(this->f_1c,
                                  *g_ChoiceWindow_screenHeight_147068 / 2 -
                                      this->f_c / 2 + F<int>(*layoutHolder, 0x8));
}

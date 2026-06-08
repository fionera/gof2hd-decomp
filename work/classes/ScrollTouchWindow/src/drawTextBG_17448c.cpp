#include "class.h"

extern "C" float ScrollTouchBox_getRelativeScrollHeight(void *self);
extern "C" void String_ctor_cstr(void *dst, const char *text, bool copy);
extern "C" void String_dtor(void *self);
extern "C" void Layout_drawBox(void *layout, int style, int x, int y, int w, int h,
                               AbyssEngine::String *text);

__attribute__((visibility("hidden"))) extern void **g_STW_layout_drawTextBG;
__attribute__((visibility("hidden"))) extern const char g_STW_empty_drawTextBG[];

void ScrollTouchWindow::drawTextBG()
{
    void **layoutHolder = g_STW_layout_drawTextBG;
    void *layout = *layoutHolder;
    int x = I(this, 0x14);
    int y = I(this, 0x18);
    int w = I(this, 0x1c);
    int pad = I(layout, 0x2c);
    float relHeight = ScrollTouchBox_getRelativeScrollHeight(P(this, 0));
    void *layoutNow = *layoutHolder;
    int widthInset;
    int heightInset;
    if (relHeight > 0.0f) {
        int p = I(layoutNow, 0x2c);
        widthInset = I(layoutNow, 0x48) + p * 2;
        heightInset = p * 2;
    } else {
        int p = I(layoutNow, 0x2c);
        widthInset = p;
        heightInset = p * 2;
    }
    int h = I(this, 0x20);
    char text[12];
    String_ctor_cstr(text, g_STW_empty_drawTextBG, false);
    Layout_drawBox(layout, 5, x, pad + y, w - widthInset, h - heightInset,
                   (AbyssEngine::String *)text);
    String_dtor(text);
}

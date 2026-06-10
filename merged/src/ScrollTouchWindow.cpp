#include "ScrollTouchWindow.h"


extern "C" void ScrollTouchBox_OnTouchEnd(void *self, int x, int y);
extern "C" void ScrollTouchBox_OnTouchMove(void *self, int x, int y);
extern "C" void ScrollTouchBox_setTextCentered(void *self, bool centered);
extern "C" void ScrollTouchBox_setYPosition(void *self, int y);
extern "C" void *ScrollTouchBox_dtor(void *self);
extern "C" void operator_delete(void *self);
extern "C" void String_dtor(void *self);
extern "C" void ScrollTouchBox_update(void *self, int dt);
extern "C" void ScrollTouchBox_OnTouchBegin(void *self, int x, int y);
extern "C" int PaintCanvas_GetColor(void *canvas);
extern "C" void PaintCanvas_SetColor(void *canvas, int color);
extern "C" void String_ctor_copy(void *dst, const AbyssEngine::String *src, bool copy);
extern "C" void ScrollTouchBox_draw(void *self);
extern "C" float ScrollTouchBox_getRelativeScrollStartPos(void *self);
extern "C" float ScrollTouchBox_getRelativeScrollHeight(void *self);
extern "C" void String_ctor_cstr(void *dst, const char *text, bool copy);
extern "C" void ScrollTouchBox_setText(void *self, AbyssEngine::String *text);
extern "C" void String_ctor_default(void *self);
extern "C" void *operator_new(unsigned int size);
extern "C" void ScrollTouchBox_ctor(void *self, int x, int y, int w, int h);
extern "C" void ScrollTouchBox_setTextColor(void *self, AbyssEngine::String *text, int color);

// ---- OnTouchEnd_17456a.cpp ----
void ScrollTouchWindow::OnTouchEnd(int x, int y)
{
    ScrollTouchBox_OnTouchEnd(P(this, 0), x, y);
    UC(this, 0x10) = 0;
}

// ---- OnTouchMove_174558.cpp ----
void ScrollTouchWindow::OnTouchMove(int x, int y)
{
    ScrollTouchBox_OnTouchMove(P(this, 0), x, y);
    UC(this, 0x10) = 1;
}

// ---- setTextCentered_174484.cpp ----
void ScrollTouchWindow::setTextCentered(bool centered)
{
    return ScrollTouchBox_setTextCentered(P(this, 0), centered);
}

// ---- setYPosition_174328.cpp ----
void ScrollTouchWindow::setYPosition(int y)
{
    return ScrollTouchBox_setYPosition(P(this, 0), y);
}

// ---- _ScrollTouchWindow_174244.cpp ----
ScrollTouchWindow::~ScrollTouchWindow()
{
    void *box = P(this, 0);
    if (box != 0) {
        operator_delete(ScrollTouchBox_dtor(box));
    }

    int *p = (int *)this;
    *p++ = 0;
    String_dtor(p);
}

// ---- update_17454c.cpp ----
void ScrollTouchWindow::update(int dt)
{
    return ScrollTouchBox_update(this->f_0, dt);
}

// ---- OnTouchBegin_174552.cpp ----
void ScrollTouchWindow::OnTouchBegin(int x, int y)
{
    return ScrollTouchBox_OnTouchBegin(this->f_0, x, y);
}

// ---- draw_174330.cpp ----
extern "C" void Layout_drawWindow(void *layout, AbyssEngine::String *title, int x, int y,
                                  int w, int h, int framed) __attribute__((nothrow));
extern "C" void Layout_drawScrollBar(void *layout, int x, int y, int h,
                                     int start, int size);

__attribute__((visibility("hidden"))) extern void **g_STW_canvas_draw;
__attribute__((visibility("hidden"))) extern void **g_STW_layout_draw_plain;
__attribute__((visibility("hidden"))) extern void **g_STW_layout_draw_window;
__attribute__((visibility("hidden"))) extern void **g_STW_layout_draw_scrollbar;

void ScrollTouchWindow::draw()
{
    void **canvasHolder = g_STW_canvas_draw;
    void *canvas = *canvasHolder;
    int color = PaintCanvas_GetColor(canvas);

    int scrollOffset;
    int contentHeight;
    if (UC(this, 0x11) == 0) {
        void *layout = *g_STW_layout_draw_plain;
        contentHeight = I(this, 0x20) - I(layout, 0x2c) * 2;
        scrollOffset = 0;
    } else {
        void **layoutHolder = g_STW_layout_draw_window;
        void *layout = *layoutHolder;
        {
            char title[12];
            String_ctor_copy(title, (AbyssEngine::String *)B(this, 4), false);
            Layout_drawWindow(layout, (AbyssEngine::String *)title, I(this, 0x14), I(this, 0x18),
                              I(this, 0x1c), I(this, 0x20), 1);
            String_dtor(title);
        }
        layout = *layoutHolder;
        contentHeight = I(this, 0x20) - I(layout, 0x2c) * 2;
        if (UC(this, 0x11) != 0) {
            scrollOffset = -I(layout, 8);
        } else {
            scrollOffset = 0;
        }
    }

    ScrollTouchBox_draw(this->f_0);
    int scrollHeight = scrollOffset + contentHeight;
    float scale = (float)scrollHeight;
    float start = ScrollTouchBox_getRelativeScrollStartPos(this->f_0);
    float height = ScrollTouchBox_getRelativeScrollHeight(this->f_0);
    int startPx = (int)(start * scale);
    int heightPx = (int)(height * scale);

    if (startPx > 0 || heightPx >= 1) {
        void *layout = *g_STW_layout_draw_scrollbar;
        int yOffset;
        if (UC(this, 0x11) == 0) {
            yOffset = 0;
        } else {
            yOffset = I(layout, 8);
        }
        Layout_drawScrollBar(layout,
                             (I(this, 0x14) + I(this, 0x1c)) - I(layout, 0x48) - I(layout, 0x2c),
                             I(this, 0x18) + I(layout, 0x2c) + yOffset,
                             scrollHeight, startPx, heightPx);
    }

    PaintCanvas_SetColor(*canvasHolder, color);
}

// ---- drawTextBG_17448c.cpp ----
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
    float relHeight = ScrollTouchBox_getRelativeScrollHeight(this->f_0);
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

// ---- setText_174264.cpp ----
void ScrollTouchWindow::setText(AbyssEngine::String title, AbyssEngine::String text)
{
    {
        void *box = this->f_0;
        AbyssEngine::String tmp(text, false);
        ScrollTouchBox_setText(box, &tmp);
    }
    *(AbyssEngine::String *)B(this, 4) = title;
}

// ---- ScrollTouchWindow_174128.cpp ----
__attribute__((visibility("hidden"))) extern void **g_STW_layout_174128;

ScrollTouchWindow::ScrollTouchWindow(int x, int y, int w, int h, bool hasFrame)
{
    String_ctor_default(B(this, 4));
    int *geom = (int *)B(this, 0x14);
    geom[0] = x;
    geom[1] = y;
    geom[2] = w;
    I(this, 0x20) = h;

    void *box = operator_new(0x40);
    void *layout = *g_STW_layout_174128;
    int border = I(layout, 0x4c);
    int extra;
    int boxY;
    if (hasFrame) {
        int top = I(layout, 8);
        boxY = border + y + top;
        extra = -top;
    } else {
        boxY = border + y;
        extra = 0;
    }
    ScrollTouchBox_ctor(box, border + x, boxY, w - border * 2,
                        extra + h - border * 2);
    *(unsigned char volatile *)B(this, 0x10) = 0;
    *(void *volatile *)B(this, 0) = box;
    *(unsigned char volatile *)B(this, 0x11) = hasFrame;
}

// ---- setText_1742c4.cpp ----
void ScrollTouchWindow::setText(AbyssEngine::String title, AbyssEngine::String text, int color)
{
    {
        void *box = this->f_0;
        AbyssEngine::String tmp(text, false);
        ScrollTouchBox_setTextColor(box, &tmp, color);
    }
    *(AbyssEngine::String *)B(this, 4) = title;
}

// ---- ScrollTouchWindow_1741c0.cpp ----
__attribute__((visibility("hidden"))) extern void **g_STW_layout_1741c0;

ScrollTouchWindow::ScrollTouchWindow(int x, int y, int w, int h)
{
    String_ctor_default(B(this, 4));
    I(this, 0x14) = x;
    I(this, 0x18) = y;
    I(this, 0x1c) = w;
    I(this, 0x20) = h;

    void *box = operator_new(0x40);
    void *layout = *g_STW_layout_1741c0;
    int top = I(layout, 8);
    int border = I(layout, 0x4c);
    ScrollTouchBox_ctor(box, border + x, top + border + y, w - border * 2,
                        (h - top) - border * 2);
    *(void *volatile *)B(this, 0) = box;
    *(unsigned short volatile *)B(this, 0x10) = 0x100;
}

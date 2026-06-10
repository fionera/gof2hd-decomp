#include "gof2/ScrollTouchWindow.h"
#include "gof2/Layout.h"
#include "gof2/String.h"


extern "C" void ScrollTouchBox_OnTouchEnd(void *self, int x, int y);
extern "C" void ScrollTouchBox_OnTouchMove(void *self, int x, int y);
extern "C" void ScrollTouchBox_setTextCentered(void *self, bool centered);
extern "C" void ScrollTouchBox_setYPosition(void *self, int y);
extern "C" void *ScrollTouchBox_dtor(void *self);
extern "C" void operator_delete(void *self);
extern "C" void ScrollTouchBox_update(void *self, int dt);
extern "C" void ScrollTouchBox_OnTouchBegin(void *self, int x, int y);
extern "C" int PaintCanvas_GetColor(void *canvas);
extern "C" void PaintCanvas_SetColor(void *canvas, int color);
extern "C" void ScrollTouchBox_draw(void *self);
extern "C" float ScrollTouchBox_getRelativeScrollStartPos(void *self);
extern "C" float ScrollTouchBox_getRelativeScrollHeight(void *self);
extern "C" void String_ctor_cstr(void *dst, const char *text, bool copy);
extern "C" void ScrollTouchBox_setText(void *self, AbyssEngine::String *text);
extern "C" void String_ctor_default(void *self);
extern "C" void *operator_new(unsigned int size);
extern "C" void ScrollTouchBox_ctor(void *self, int x, int y, int w, int h);
extern "C" void ScrollTouchBox_setTextColor(void *self, AbyssEngine::String *text, int color);

// Layout fields (a global UI metrics object) are still accessed by byte offset since the
// Layout class is not modeled here; helper kept local to this translation unit.
static inline int   &LayoutI(void *p, int off) { return *(int *)((char *)p + off); }

// ---- OnTouchEnd_17456a.cpp ----
void ScrollTouchWindow::OnTouchEnd(int x, int y)
{
    ScrollTouchBox_OnTouchEnd(this->field_0x0, x, y);
    this->field_0x10 = 0;
}

// ---- OnTouchMove_174558.cpp ----
void ScrollTouchWindow::OnTouchMove(int x, int y)
{
    ScrollTouchBox_OnTouchMove(this->field_0x0, x, y);
    this->field_0x10 = 1;
}

// ---- setTextCentered_174484.cpp ----
void ScrollTouchWindow::setTextCentered(bool centered)
{
    return ScrollTouchBox_setTextCentered(this->field_0x0, centered);
}

// ---- setYPosition_174328.cpp ----
void ScrollTouchWindow::setYPosition(int y)
{
    return ScrollTouchBox_setYPosition(this->field_0x0, y);
}

// ---- _ScrollTouchWindow_174244.cpp ----
ScrollTouchWindow::~ScrollTouchWindow()
{
    void *box = this->field_0x0;
    if (box != 0) {
        operator_delete(ScrollTouchBox_dtor(box));
    }
    this->field_0x0 = 0;
    ((String *)(&this->field_0x4))->dtor();
}

// ---- update_17454c.cpp ----
void ScrollTouchWindow::update(int dt)
{
    return ScrollTouchBox_update(this->field_0x0, dt);
}

// ---- OnTouchBegin_174552.cpp ----
void ScrollTouchWindow::OnTouchBegin(int x, int y)
{
    return ScrollTouchBox_OnTouchBegin(this->field_0x0, x, y);
}

// ---- draw_174330.cpp ----
extern "C" void Layout_drawWindow(void *layout, AbyssEngine::String *title, int x, int y,
                                  int w, int h, int framed) __attribute__((nothrow));

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
    if (this->field_0x11 == 0) {
        void *layout = *g_STW_layout_draw_plain;
        contentHeight = this->field_0x20 - LayoutI(layout, 0x2c) * 2;
        scrollOffset = 0;
    } else {
        void **layoutHolder = g_STW_layout_draw_window;
        void *layout = *layoutHolder;
        {
            char title[sizeof(AbyssEngine::String)];
            ((String *)(title))->ctor_copy(&this->field_0x4, false);
            Layout_drawWindow(layout, (AbyssEngine::String *)title, this->field_0x14, this->field_0x18,
                              this->field_0x1c, this->field_0x20, 1);
            ((String *)(title))->dtor();
        }
        layout = *layoutHolder;
        contentHeight = this->field_0x20 - LayoutI(layout, 0x2c) * 2;
        if (this->field_0x11 != 0) {
            scrollOffset = -LayoutI(layout, 8);
        } else {
            scrollOffset = 0;
        }
    }

    ScrollTouchBox_draw(this->field_0x0);
    int scrollHeight = scrollOffset + contentHeight;
    float scale = (float)scrollHeight;
    float start = ScrollTouchBox_getRelativeScrollStartPos(this->field_0x0);
    float height = ScrollTouchBox_getRelativeScrollHeight(this->field_0x0);
    int startPx = (int)(start * scale);
    int heightPx = (int)(height * scale);

    if (startPx > 0 || heightPx >= 1) {
        void *layout = *g_STW_layout_draw_scrollbar;
        int yOffset;
        if (this->field_0x11 == 0) {
            yOffset = 0;
        } else {
            yOffset = LayoutI(layout, 8);
        }
        ((Layout *)(layout))->drawScrollBar((this->field_0x14 + this->field_0x1c) - LayoutI(layout, 0x48) - LayoutI(layout, 0x2c), this->field_0x18 + LayoutI(layout, 0x2c) + yOffset, scrollHeight, startPx, heightPx);
    }

    PaintCanvas_SetColor(*canvasHolder, color);
}

// ---- drawTextBG_17448c.cpp ----

__attribute__((visibility("hidden"))) extern void **g_STW_layout_drawTextBG;
__attribute__((visibility("hidden"))) extern const char g_STW_empty_drawTextBG[];

void ScrollTouchWindow::drawTextBG()
{
    void **layoutHolder = g_STW_layout_drawTextBG;
    void *layout = *layoutHolder;
    int x = this->field_0x14;
    int y = this->field_0x18;
    int w = this->field_0x1c;
    int pad = LayoutI(layout, 0x2c);
    float relHeight = ScrollTouchBox_getRelativeScrollHeight(this->field_0x0);
    void *layoutNow = *layoutHolder;
    int widthInset;
    int heightInset;
    if (relHeight > 0.0f) {
        int p = LayoutI(layoutNow, 0x2c);
        widthInset = LayoutI(layoutNow, 0x48) + p * 2;
        heightInset = p * 2;
    } else {
        int p = LayoutI(layoutNow, 0x2c);
        widthInset = p;
        heightInset = p * 2;
    }
    int h = this->field_0x20;
    char text[sizeof(AbyssEngine::String)];
    String_ctor_cstr(text, g_STW_empty_drawTextBG, false);
    ((Layout *)(layout))->drawBox(5, x, pad + y, w - widthInset, h - heightInset, (AbyssEngine::String *)text);
    ((String *)(text))->dtor();
}

// ---- setText_174264.cpp ----
void ScrollTouchWindow::setText(AbyssEngine::String title, AbyssEngine::String text)
{
    {
        void *box = this->field_0x0;
        char tmp[sizeof(AbyssEngine::String)];
        ((String *)(tmp))->ctor_copy(&text, false);
        ScrollTouchBox_setText(box, (AbyssEngine::String *)tmp);
        ((String *)(tmp))->dtor();
    }
    this->field_0x4 = title;
}

// ---- ScrollTouchWindow_174128.cpp ----
__attribute__((visibility("hidden"))) extern void **g_STW_layout_174128;

ScrollTouchWindow::ScrollTouchWindow(int x, int y, int w, int h, bool hasFrame)
{
    String_ctor_default(&this->field_0x4);
    this->field_0x14 = x;
    this->field_0x18 = y;
    this->field_0x1c = w;
    this->field_0x20 = h;

    void *box = operator_new(0x40);
    void *layout = *g_STW_layout_174128;
    int border = LayoutI(layout, 0x4c);
    int extra;
    int boxY;
    if (hasFrame) {
        int top = LayoutI(layout, 8);
        boxY = border + y + top;
        extra = -top;
    } else {
        boxY = border + y;
        extra = 0;
    }
    ScrollTouchBox_ctor(box, border + x, boxY, w - border * 2,
                        extra + h - border * 2);
    this->field_0x10 = 0;
    this->field_0x0 = box;
    this->field_0x11 = hasFrame;
}

// ---- setText_1742c4.cpp ----
void ScrollTouchWindow::setText(AbyssEngine::String title, AbyssEngine::String text, int color)
{
    {
        void *box = this->field_0x0;
        char tmp[sizeof(AbyssEngine::String)];
        ((String *)(tmp))->ctor_copy(&text, false);
        ScrollTouchBox_setTextColor(box, (AbyssEngine::String *)tmp, color);
        ((String *)(tmp))->dtor();
    }
    this->field_0x4 = title;
}

// ---- ScrollTouchWindow_1741c0.cpp ----
__attribute__((visibility("hidden"))) extern void **g_STW_layout_1741c0;

ScrollTouchWindow::ScrollTouchWindow(int x, int y, int w, int h)
{
    String_ctor_default(&this->field_0x4);
    this->field_0x14 = x;
    this->field_0x18 = y;
    this->field_0x1c = w;
    this->field_0x20 = h;

    void *box = operator_new(0x40);
    void *layout = *g_STW_layout_1741c0;
    int top = LayoutI(layout, 8);
    int border = LayoutI(layout, 0x4c);
    ScrollTouchBox_ctor(box, border + x, top + border + y, w - border * 2,
                        (h - top) - border * 2);
    this->field_0x0 = box;
    this->field_0x10 = 0;     // low byte of the 0x100 short store
    this->field_0x11 = 1;     // high byte of the 0x100 short store
}

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
    ScrollTouchBox_OnTouchEnd(this->scrollBox, x, y);
    this->touchActive = 0;
}

// ---- OnTouchMove_174558.cpp ----
void ScrollTouchWindow::OnTouchMove(int x, int y)
{
    ScrollTouchBox_OnTouchMove(this->scrollBox, x, y);
    this->touchActive = 1;
}

// ---- setTextCentered_174484.cpp ----
void ScrollTouchWindow::setTextCentered(bool centered)
{
    return ScrollTouchBox_setTextCentered(this->scrollBox, centered);
}

// ---- setYPosition_174328.cpp ----
void ScrollTouchWindow::setYPosition(int y)
{
    return ScrollTouchBox_setYPosition(this->scrollBox, y);
}

// ---- _ScrollTouchWindow_174244.cpp ----
ScrollTouchWindow::~ScrollTouchWindow()
{
    void *box = this->scrollBox;
    if (box != 0) {
        operator_delete(ScrollTouchBox_dtor(box));
    }
    this->scrollBox = 0;
    ((String *)(&this->title))->dtor();
}

// ---- update_17454c.cpp ----
void ScrollTouchWindow::update(int dt)
{
    return ScrollTouchBox_update(this->scrollBox, dt);
}

// ---- OnTouchBegin_174552.cpp ----
void ScrollTouchWindow::OnTouchBegin(int x, int y)
{
    return ScrollTouchBox_OnTouchBegin(this->scrollBox, x, y);
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
    if (this->hasFrame == 0) {
        void *layout = *g_STW_layout_draw_plain;
        contentHeight = this->height - LayoutI(layout, 0x2c) * 2;
        scrollOffset = 0;
    } else {
        void **layoutHolder = g_STW_layout_draw_window;
        void *layout = *layoutHolder;
        {
            char title[sizeof(AbyssEngine::String)];
            ((String *)(title))->ctor_copy(&this->title, false);
            Layout_drawWindow(layout, (AbyssEngine::String *)title, this->x, this->y,
                              this->width, this->height, 1);
            ((String *)(title))->dtor();
        }
        layout = *layoutHolder;
        contentHeight = this->height - LayoutI(layout, 0x2c) * 2;
        if (this->hasFrame != 0) {
            scrollOffset = -LayoutI(layout, 8);
        } else {
            scrollOffset = 0;
        }
    }

    ScrollTouchBox_draw(this->scrollBox);
    int scrollHeight = scrollOffset + contentHeight;
    float scale = (float)scrollHeight;
    float start = ScrollTouchBox_getRelativeScrollStartPos(this->scrollBox);
    float height = ScrollTouchBox_getRelativeScrollHeight(this->scrollBox);
    int startPx = (int)(start * scale);
    int heightPx = (int)(height * scale);

    if (startPx > 0 || heightPx >= 1) {
        void *layout = *g_STW_layout_draw_scrollbar;
        int yOffset;
        if (this->hasFrame == 0) {
            yOffset = 0;
        } else {
            yOffset = LayoutI(layout, 8);
        }
        ((Layout *)(layout))->drawScrollBar((this->x + this->width) - LayoutI(layout, 0x48) - LayoutI(layout, 0x2c), this->y + LayoutI(layout, 0x2c) + yOffset, scrollHeight, startPx, heightPx);
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
    int x = this->x;
    int y = this->y;
    int w = this->width;
    int pad = LayoutI(layout, 0x2c);
    float relHeight = ScrollTouchBox_getRelativeScrollHeight(this->scrollBox);
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
    int h = this->height;
    char text[sizeof(AbyssEngine::String)];
    String_ctor_cstr(text, g_STW_empty_drawTextBG, false);
    ((Layout *)(layout))->drawBox(5, x, pad + y, w - widthInset, h - heightInset, (AbyssEngine::String *)text, 0);
    ((String *)(text))->dtor();
}

// ---- setText_174264.cpp ----
void ScrollTouchWindow::setText(AbyssEngine::String title, AbyssEngine::String text)
{
    {
        void *box = this->scrollBox;
        char tmp[sizeof(AbyssEngine::String)];
        ((String *)(tmp))->ctor_copy(&text, false);
        ScrollTouchBox_setText(box, (AbyssEngine::String *)tmp);
        ((String *)(tmp))->dtor();
    }
    this->title = title;
}

// ---- ScrollTouchWindow_174128.cpp ----
__attribute__((visibility("hidden"))) extern void **g_STW_layout_174128;

ScrollTouchWindow::ScrollTouchWindow(int x, int y, int w, int h, bool hasFrame)
{
    String_ctor_default(&this->title);
    this->x = x;
    this->y = y;
    this->width = w;
    this->height = h;

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
    this->touchActive = 0;
    this->scrollBox = box;
    this->hasFrame = hasFrame;
}

// ---- setText_1742c4.cpp ----
void ScrollTouchWindow::setText(AbyssEngine::String title, AbyssEngine::String text, int color)
{
    {
        void *box = this->scrollBox;
        char tmp[sizeof(AbyssEngine::String)];
        ((String *)(tmp))->ctor_copy(&text, false);
        ScrollTouchBox_setTextColor(box, (AbyssEngine::String *)tmp, color);
        ((String *)(tmp))->dtor();
    }
    this->title = title;
}

// ---- ScrollTouchWindow_1741c0.cpp ----
__attribute__((visibility("hidden"))) extern void **g_STW_layout_1741c0;

ScrollTouchWindow::ScrollTouchWindow(int x, int y, int w, int h)
{
    String_ctor_default(&this->title);
    this->x = x;
    this->y = y;
    this->width = w;
    this->height = h;

    void *box = operator_new(0x40);
    void *layout = *g_STW_layout_1741c0;
    int top = LayoutI(layout, 8);
    int border = LayoutI(layout, 0x4c);
    ScrollTouchBox_ctor(box, border + x, top + border + y, w - border * 2,
                        (h - top) - border * 2);
    this->scrollBox = box;
    this->touchActive = 0;     // low byte of the 0x100 short store
    this->hasFrame = 1;     // high byte of the 0x100 short store
}

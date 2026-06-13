#include <new>
#include "gof2/ScrollTouchWindow.h"
#include "gof2/ScrollTouchBox.h"
#include "gof2/Layout.h"
#include "gof2/String.h"
#include "gof2/PaintCanvas.h"

using AbyssEngine::PaintCanvas;

extern "C" void *ScrollTouchBox_dtor(void *self);
extern "C" void String_ctor_cstr(void *dst, const char *text, bool copy);
extern "C" void String_ctor_default(void *self);
extern "C" void ScrollTouchBox_ctor(void *self, int x, int y, int w, int h);
extern "C" void ScrollTouchBox_setTextColor(void *self, AbyssEngine::String *text, int color);

// Layout fields (a global UI metrics object) are still accessed by byte offset since the
// Layout class is not modeled here; helper kept local to this translation unit.
static inline int   &LayoutI(void *p, int off) { return *(int *)((char *)p + off); }

// ---- OnTouchEnd_17456a.cpp ----
void ScrollTouchWindow::OnTouchEnd(int x, int y)
{
    ((ScrollTouchBox *)(this->scrollBox))->OnTouchEnd(x, y);
    this->touchActive = 0;
}

// ---- touch_end (engine-name forwarder for OnTouchEnd) ----
void ScrollTouchWindow::touch_end(int x, int y)
{
    this->OnTouchEnd(x, y);
}

// ---- setText4 (engine-name forwarder for the coloured setText) ----
void ScrollTouchWindow::setText4(AbyssEngine::String title, AbyssEngine::String text, int color)
{
    this->setText(title, text, color);
}

// ---- OnTouchMove_174558.cpp ----
void ScrollTouchWindow::OnTouchMove(int x, int y)
{
    ((ScrollTouchBox *)(this->scrollBox))->OnTouchMove(x, y);
    this->touchActive = 1;
}

// ---- setTextCentered_174484.cpp ----
void ScrollTouchWindow::setTextCentered(bool centered)
{
    return ((ScrollTouchBox *)(this->scrollBox))->setTextCentered(centered);
}

// ---- setYPosition_174328.cpp ----
void ScrollTouchWindow::setYPosition(int y)
{
    return ((ScrollTouchBox *)(this->scrollBox))->setYPosition(y);
}

// ---- _ScrollTouchWindow_174244.cpp ----
ScrollTouchWindow::~ScrollTouchWindow()
{
    void *box = this->scrollBox;
    if (box != 0) {
        ::operator delete(ScrollTouchBox_dtor(box));
    }
    this->scrollBox = 0;
    ((String *)(&this->title))->dtor();
}

// ---- update_17454c.cpp ----
void ScrollTouchWindow::update(int dt)
{
    return ((ScrollTouchBox *)(this->scrollBox))->update(dt);
}

// ---- OnTouchBegin_174552.cpp ----
void ScrollTouchWindow::OnTouchBegin(int x, int y)
{
    return ((ScrollTouchBox *)(this->scrollBox))->OnTouchBegin(x, y);
}

// ---- scroll ----
// Keyboard / D-pad driven scroll. The pointer-drag path feeds the box a velocity
// that update() integrates into scrollOffset and clamps to the content extent; a
// discrete key-press scroll does the same by stepping scrollOffset directly (one
// box-height worth per step) and re-running the box's settle logic.
void ScrollTouchWindow::scroll(int amount)
{
    ScrollTouchBox *box = (ScrollTouchBox *)this->scrollBox;
    if (box == 0)
        return;

    int range = box->contentHeight - box->height;
    if (range <= 0)
        return;

    // Step by one visible page per key press, then let update() clamp / settle.
    box->scrollOffset -= amount * box->height;
    box->update(0);
}

// Free-function entry used by callers that hold the window opaquely (e.g.
// SpaceLounge::onKeyPress). Forwards to the real method.
extern "C" void ScrollTouchWindow_scroll(void *window, int amount)
{
    ((ScrollTouchWindow *)window)->scroll(amount);
}

// ---- draw_174330.cpp ----
__attribute__((visibility("hidden"))) extern void **g_STW_canvas_draw;
__attribute__((visibility("hidden"))) extern void **g_STW_layout_draw_plain;
__attribute__((visibility("hidden"))) extern void **g_STW_layout_draw_window;
__attribute__((visibility("hidden"))) extern void **g_STW_layout_draw_scrollbar;

void ScrollTouchWindow::draw()
{
    void **canvasHolder = g_STW_canvas_draw;
    void *canvas = *canvasHolder;
    int color = ((PaintCanvas*)canvas)->GetColor();

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
            ((Layout *)layout)->drawWindow7((AbyssEngine::String *)title, this->x, this->y,
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

    ((ScrollTouchBox *)(this->scrollBox))->draw();
    int scrollHeight = scrollOffset + contentHeight;
    float scale = (float)scrollHeight;
    float start = ((ScrollTouchBox *)(this->scrollBox))->getRelativeScrollStartPos();
    float height = ((ScrollTouchBox *)(this->scrollBox))->getRelativeScrollHeight();
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

    ((PaintCanvas*)*canvasHolder)->SetColor((unsigned int)color);
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
    float relHeight = ((ScrollTouchBox *)(this->scrollBox))->getRelativeScrollHeight();
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
        ((ScrollTouchBox *)(box))->setText(*(AbyssEngine::String *)tmp);
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

    void *box = ::operator new(0x40);
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

    void *box = ::operator new(0x40);
    void *layout = *g_STW_layout_1741c0;
    int top = LayoutI(layout, 8);
    int border = LayoutI(layout, 0x4c);
    ScrollTouchBox_ctor(box, border + x, top + border + y, w - border * 2,
                        (h - top) - border * 2);
    this->scrollBox = box;
    this->touchActive = 0;     // low byte of the 0x100 short store
    this->hasFrame = 1;     // high byte of the 0x100 short store
}

// Free-function ctor/dtor entries used by callers that allocate the window opaquely
// (placement-construct into `self`, run the in-place destructor and hand back the
// pointer for the caller to free).
extern "C" void ScrollTouchWindow_ctor(void *self, int x, int y, int w, int h, bool hasFrame)
{
    new (self) ScrollTouchWindow(x, y, w, h, hasFrame);
}

extern "C" void *ScrollTouchWindow_dtor(void *window)
{
    ((ScrollTouchWindow *)window)->~ScrollTouchWindow();
    return window;
}

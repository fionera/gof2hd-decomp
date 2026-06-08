#include "class.h"

extern "C" int PaintCanvas_GetColor(void *canvas);
extern "C" void PaintCanvas_SetColor(void *canvas, int color);
extern "C" void String_ctor_copy(void *dst, const AbyssEngine::String *src, bool copy);
extern "C" void String_dtor(void *self);
extern "C" void Layout_drawWindow(void *layout, AbyssEngine::String *title, int x, int y,
                                  int w, int h, int framed) __attribute__((nothrow));
extern "C" void Layout_drawScrollBar(void *layout, int x, int y, int h,
                                     int start, int size);
extern "C" void ScrollTouchBox_draw(void *self);
extern "C" float ScrollTouchBox_getRelativeScrollStartPos(void *self);
extern "C" float ScrollTouchBox_getRelativeScrollHeight(void *self);

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

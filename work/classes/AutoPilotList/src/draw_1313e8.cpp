#include "class.h"

namespace AbyssEngine { struct String; }
using AbyssEngine::String;

extern "C" String *GameText_getText(void *self, int id);                  // 0x72f70
extern "C" void String_ctor_copy(void *dst, const String *src, bool);    // 0x6f028
extern "C" void String_dtor(void *self);                                 // 0x6ee30
extern "C" void Layout_drawWindow(void *layout, const String *title, int x, int y,
                                  int w, int h);                          // 0x768d0
extern "C" void PaintCanvas_DrawString(void *canvas, const String *font, void *text,
                                       int x, int y, int shadow);         // 0x6fe14

// PC-relative GOT holders. **holder yields the live object.
__attribute__((visibility("hidden"))) extern void **g_APL_layout_draw;     // -> Layout
__attribute__((visibility("hidden"))) extern void **g_APL_gametext_draw;   // -> GameText
__attribute__((visibility("hidden"))) extern void **g_APL_font_draw;       // -> String* font
__attribute__((visibility("hidden"))) extern void **g_APL_canvas_draw;     // -> PaintCanvas

// AutoPilotList::draw() - draw the window frame plus one row per non-empty entry.
void AutoPilotList::draw() {
    void *layout = *g_APL_layout_draw;
    String *title = GameText_getText(*g_APL_gametext_draw, 0x23c);
    char tmp[12];
    String_ctor_copy(tmp, title, false);
    Layout_drawWindow(layout, (String *)tmp, F<int>(this, 0x4), F<int>(this, 0x8),
                      F<int>(this, 0xc), F<int>(this, 0x14) * 0xf + 0x16);
    String_dtor(tmp);

    int drawn = 0;
    void **canvasHolder = g_APL_canvas_draw;
    void **fontHolder = g_APL_font_draw;
    for (uint32_t i = 0; i < F<Array<void *> *>(this, 0x10)->length; i++) {
        void *text = F<Array<void *> *>(this, 0x10)->data[i];
        if (text != 0) {
            PaintCanvas_DrawString(*canvasHolder, (String *)*fontHolder, text,
                                   F<int>(this, 0x4),
                                   drawn * 0xf + F<int>(this, 0x8) + 0x12, 0);
            drawn = drawn + 1;
        }
    }
}

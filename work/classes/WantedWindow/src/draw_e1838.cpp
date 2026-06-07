#include "class.h"

extern "C" void WantedWindow_draw_tail(void *starMap);
extern "C" float WantedWindow_getRelativeScrollStartPos(WantedWindow *self);
extern "C" float WantedWindow_getRelativeScrollHeight(WantedWindow *self);
extern "C" void PaintCanvas_EnableClip(void *canvas, int x, int y, int w, int h);
extern "C" void PaintCanvas_DisableClip(void *canvas);
extern "C" void PaintCanvas_SetColor(void *canvas, unsigned int color);
extern "C" int PaintCanvas_GetTextHeight(void *canvas, unsigned int font);
extern "C" int PaintCanvas_GetTextWidth(void *canvas, void *font, String *text);
extern "C" void PaintCanvas_DrawString(void *canvas, void *font, String *text, int x, int y, bool flag);
extern "C" void PaintCanvas_DrawImage2D(void *canvas, int image, int x, int y);
extern "C" void Layout_drawScrollBar(void *layout, int x, int y, int h, int start, int size);
extern "C" void Layout_drawBox(void *layout, int style, int x, int y, int w, int h, String *text);
extern "C" void Layout_drawHeader(void *layout, String *text);
extern "C" void Layout_drawFooter(void *layout);
extern "C" void TouchButton_draw(void *button);
extern "C" int Wanted_isActive(void *wanted);
extern "C" void Wanted_getName(String *out, void *wanted);
extern "C" int Status_getCurrentCampaignMission(void *status);
extern "C" String *GameText_getText(void *text, int id);
extern "C" void String_cstr_ctor(String *s, const char *text, bool copy);
extern "C" void String_copy_ctor(String *s, String *src, bool copy);
extern "C" void String_plus(String *out, String *a, String *b);
extern "C" void String_dtor(String *s);
extern "C" void ImageFactory_drawChar(void *factory, void *parts, int x, int y, bool flag);
extern "C" void ScrollTouchWindow_draw(void *scroll);

__attribute__((visibility("hidden"))) extern void **g_WantedWindow_draw_layout;
__attribute__((visibility("hidden"))) extern void **g_WantedWindow_draw_canvas;
__attribute__((visibility("hidden"))) extern void **g_WantedWindow_draw_font;
__attribute__((visibility("hidden"))) extern void **g_WantedWindow_draw_status;
__attribute__((visibility("hidden"))) extern void **g_WantedWindow_draw_text;
__attribute__((visibility("hidden"))) extern void **g_WantedWindow_draw_factory;
__attribute__((visibility("hidden"))) extern const char g_WantedWindow_draw_empty_a[];
__attribute__((visibility("hidden"))) extern const char g_WantedWindow_draw_mark[];
__attribute__((visibility("hidden"))) extern const char g_WantedWindow_draw_empty_b[];
__attribute__((visibility("hidden"))) extern const char g_WantedWindow_draw_empty_c[];
__attribute__((visibility("hidden"))) extern const char g_WantedWindow_draw_label_a[];
__attribute__((visibility("hidden"))) extern const char g_WantedWindow_draw_label_b[];

static inline void *draw_wanted_at(WantedWindow *self, int idx)
{
    return ArrayItem(F<void *>(self, 0x38), idx);
}

extern "C" void WantedWindow_draw(WantedWindow *self)
{
    String s40;
    String s4c;
    String s58;
    String s64;
    String s70;
    String s7c;
    String s88;
    String s94;
    String sa0;

    if (F<uint8_t>(self, 0x14) != 0) {
        WantedWindow_draw_tail(F<void *>(self, 0x4));
        return;
    }

    void *layout = *g_WantedWindow_draw_layout;
    void *canvas = *g_WantedWindow_draw_canvas;
    void *font = *g_WantedWindow_draw_font;

    PaintCanvas_EnableClip(canvas, F<int>(self, 0x1c),
                           F<int>(self, 0x20) + F<int>(layout, 0x0c) +
                               F<int>(layout, 0x20) + F<int>(layout, 0x5c),
                           F<int>(self, 0x24),
                           F<int>(layout, 0x2c) + F<int>(self, 0xa8));

    float relStart = WantedWindow_getRelativeScrollStartPos(self);
    int visible = F<int>(self, 0xa8);
    float visf = (float)visible;
    float relHeight = WantedWindow_getRelativeScrollHeight(self);
    int barSize = (int)(relHeight * visf);
    int barStart = (int)(relStart * visf);
    if (barSize >= 1 || barStart >= 0) {
        Layout_drawScrollBar(layout,
                             ((F<int>(self, 0x1c) + (F<int>(self, 0x24) >> 1)) -
                              F<int>(layout, 0x2c)) - F<int>(layout, 0x48),
                             F<int>(self, 0x20) + F<int>(layout, 0x2c) +
                                 F<int>(layout, 0x0c) + F<int>(layout, 0x20) +
                                 F<int>(layout, 0x5c),
                             visible, barStart, barSize);
    }

    int y = F<int>(self, 0x20) + F<int>(layout, 0x0c) + F<int>(layout, 0x20) +
            F<int>(layout, 0x5c) + F<int>(layout, 0x2c) + F<int>(self, 0x84);
    int inset = barSize < 1 ? 0 : F<int>(layout, 0x2c) + F<int>(layout, 0x48);

    for (uint32_t i = 0; i < F<uint32_t>(F<void *>(self, 0x38), 0x0); ++i) {
        int style = (i == F<uint32_t>(self, 0x30) || i == F<uint32_t>(self, 0x34)) ? 4 : 3;
        String_cstr_ctor(&s40, g_WantedWindow_draw_empty_a, false);
        Layout_drawBox(layout, style, F<int>(layout, 0x28) + F<int>(self, 0x1c), y,
                       (F<int>(self, 0x24) >> 1) -
                           (inset + F<int>(layout, 0x28) + F<int>(layout, 0x2c)),
                       F<int>(layout, 0x70), &s40);
        String_dtor(&s40);

        void *wanted = draw_wanted_at(self, i);
        PaintCanvas_SetColor(canvas, Wanted_isActive(wanted) ? 0xffffffffu : 0xff808080u);
        Wanted_getName(&s4c, wanted);
        int textY = y + F<int>(layout, 0x70) / 2 -
                    PaintCanvas_GetTextHeight(canvas, (unsigned int)(unsigned long)font) / 2;
        PaintCanvas_DrawString(canvas, font, &s4c,
                               F<int>(self, 0x1c) + F<int>(layout, 0x28) +
                                   F<int>(layout, 0x44),
                               textY, false);
        String_dtor(&s4c);

        int campaign = Status_getCurrentCampaignMission(*g_WantedWindow_draw_status);
        if ((i == 0 && campaign == 0x80) ||
            (i == 1 && Status_getCurrentCampaignMission(*g_WantedWindow_draw_status) == 0x82)) {
            Wanted_getName(&s58, wanted);
            String_cstr_ctor(&s64, g_WantedWindow_draw_mark, false);
            String_plus(&s4c, &s58, &s64);
            int textW = PaintCanvas_GetTextWidth(canvas, font, &s4c);
            PaintCanvas_DrawImage2D(canvas, F<int>(self, 0xac),
                                    F<int>(self, 0x1c) + F<int>(layout, 0x28) +
                                        F<int>(layout, 0x44) + textW,
                                    textY);
            String_dtor(&s4c);
            String_dtor(&s64);
            String_dtor(&s58);
        }

        y += F<int>(layout, 0x34) + F<int>(layout, 0x70);
    }

    PaintCanvas_DisableClip(canvas);
    PaintCanvas_SetColor(canvas, 0xffffffffu);
    String_copy_ctor(&s70, GameText_getText(*g_WantedWindow_draw_text, 0xc93), false);
    Layout_drawHeader(layout, &s70);
    String_dtor(&s70);

    for (uint32_t i = 0; i < F<uint32_t>(F<void *>(self, 0x0c), 0x0); ++i) {
        TouchButton_draw(ArrayItem(F<void *>(self, 0x0c), i));
    }

    String_copy_ctor(&s7c, GameText_getText(*g_WantedWindow_draw_text, 0xc95), false);
    Layout_drawBox(layout, 1, F<int>(layout, 0x28) + F<int>(self, 0x1c),
                   F<int>(self, 0x20) + F<int>(layout, 0x0c) + F<int>(layout, 0x20),
                   (F<int>(self, 0x24) >> 1) - (F<int>(layout, 0x2c) + F<int>(layout, 0x28)),
                   F<int>(layout, 0x5c), &s7c);
    String_dtor(&s7c);

    String_cstr_ctor(&s88, g_WantedWindow_draw_empty_b, false);
    Layout_drawBox(layout, 5, F<int>(layout, 0x28) + F<int>(self, 0x1c),
                   F<int>(self, 0x20) + F<int>(layout, 0x0c) + F<int>(layout, 0x20) +
                       F<int>(layout, 0x5c) + F<int>(layout, 0x2c),
                   (F<int>(self, 0x24) >> 1) - (F<int>(layout, 0x2c) + F<int>(layout, 0x28)),
                   ((F<int>(self, 0x28) -
                     (F<int>(layout, 0x20) + F<int>(layout, 0x0c) +
                      F<int>(layout, 0x5c) + F<int>(layout, 0x2c) * 2)) -
                    F<int>(layout, 0x10)) -
                       F<int>(layout, 0x24),
                   &s88);
    String_dtor(&s88);

    String_copy_ctor(&s94, GameText_getText(*g_WantedWindow_draw_text, 0xc95), false);
    Layout_drawBox(layout, 1, F<int>(self, 0x1c) + (F<int>(self, 0x24) >> 1) +
                                  F<int>(layout, 0x2c),
                   F<int>(self, 0x20) + F<int>(layout, 0x0c) + F<int>(layout, 0x20),
                   ((F<int>(self, 0x24) >> 1) - F<int>(layout, 0x2c)) - F<int>(layout, 0x28),
                   F<int>(layout, 0x5c), &s94);
    String_dtor(&s94);

    String_cstr_ctor(&sa0, g_WantedWindow_draw_empty_c, false);
    Layout_drawBox(layout, 5, F<int>(self, 0x1c) + (F<int>(self, 0x24) >> 1) +
                                  F<int>(layout, 0x2c),
                   F<int>(self, 0x20) + F<int>(layout, 0x2c) + F<int>(layout, 0x0c) +
                       F<int>(layout, 0x20) + F<int>(layout, 0x5c),
                   ((F<int>(self, 0x24) >> 1) - F<int>(layout, 0x2c)) - F<int>(layout, 0x28),
                   ((F<int>(self, 0x28) -
                     (F<int>(layout, 0x0c) + F<int>(layout, 0x2c) * 2 +
                      F<int>(layout, 0x20) + F<int>(layout, 0x5c))) -
                    F<int>(layout, 0x10)) -
                       F<int>(layout, 0x24),
                   &sa0);
    String_dtor(&sa0);

    if (F<void *>(self, 0x8) != 0) {
        int charX = F<int>(self, 0x1c) + (F<int>(self, 0x24) >> 1) + F<int>(layout, 0x2c);
        int charY = F<int>(layout, 0x5c) + F<int>(self, 0x20) + F<int>(layout, 0x2c) +
                    F<int>(layout, 0x0c) + F<int>(layout, 0x20);
        ImageFactory_drawChar(*g_WantedWindow_draw_factory, F<void *>(self, 0x8), charX, charY, false);
        int textX = F<int>(layout, 0x2d4) + charX + F<int>(layout, 0x2c);
        PaintCanvas_DrawString(canvas, font, (String *)((char *)self + 0x54), textX, charY, false);

        String_cstr_ctor(&s64, g_WantedWindow_draw_label_a, false);
        String_plus(&s58, &s64, GameText_getText(*g_WantedWindow_draw_text, 0xc93));
        String_plus(&s4c, &s58, (String *)((char *)self + 0x3c));
        PaintCanvas_DrawString(canvas, font, &s4c, textX, charY + F<int>(layout, 0x4) * 2, false);
        String_dtor(&s4c);
        String_dtor(&s58);
        String_dtor(&s64);

        String_cstr_ctor(&s64, g_WantedWindow_draw_label_b, false);
        String_plus(&s58, &s64, GameText_getText(*g_WantedWindow_draw_text, 0xc93));
        String_plus(&s4c, &s58, (String *)((char *)self + 0x48));
        PaintCanvas_DrawString(canvas, font, &s4c, textX, charY + F<int>(layout, 0x4) * 3, false);
        String_dtor(&s4c);
        String_dtor(&s58);
        String_dtor(&s64);

        ScrollTouchWindow_draw(F<void *>(self, 0x2c));
    }

    if (F<void *>(self, 0x18) != 0 &&
        Wanted_isActive(draw_wanted_at(self, F<int>(self, 0x30))) != 0) {
        TouchButton_draw(F<void *>(self, 0x18));
    }

    Layout_drawFooter(layout);
}

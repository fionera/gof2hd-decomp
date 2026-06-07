#include "class.h"

extern "C" int GameText_getLanguage();
extern "C" void PaintCanvas_SetColor(void *canvas, int color);
extern "C" int PaintCanvas_GetTextHeight(void *canvas, String *font);
extern "C" int PaintCanvas_GetTextWidth(void *canvas, String *font, String *text);
extern "C" void PaintCanvas_DrawString(void *canvas, String *font, String *text, int x, int y, int unknown);

__attribute__((visibility("hidden"))) extern void **g_ScrollTouchBox_canvas_135778;
__attribute__((visibility("hidden"))) extern char *g_ScrollTouchBox_flag_135778;
__attribute__((visibility("hidden"))) extern void **g_ScrollTouchBox_font_135778;
__attribute__((visibility("hidden"))) extern uint8_t *g_ScrollTouchBox_rtl_135778;
__attribute__((visibility("hidden"))) extern uint8_t *g_ScrollTouchBox_dirty_135778;

void ScrollTouchBox::draw()
{
    int language = GameText_getLanguage();
    unsigned special = 0;
    unsigned shifted = (unsigned)(language - 10);
    if ((unsigned short)shifted < 6)
        special = (0x33U >> (shifted & 0x3f)) & 1;

    void **canvasHolder = g_ScrollTouchBox_canvas_135778;
    void *canvas = *canvasHolder;
    PaintCanvas_SetColor(canvas, -1);

    void *firstLineArray = *(void * volatile *)((char *)this + 0x0);
    if (firstLineArray != 0) {
        unsigned notSpecial = special ^ (firstLineArray != 0);
        char *flag = g_ScrollTouchBox_flag_135778;
        void **fontHolder = g_ScrollTouchBox_font_135778;
        uint8_t *rtl = g_ScrollTouchBox_rtl_135778;

        for (unsigned i = 0; i < F<unsigned>(*(void * volatile *)((char *)this + 0x0), 0x0); ++i) {
            void *lineArray = *(void * volatile *)((char *)this + 0x0);
            unsigned count = F<unsigned>(lineArray, 0x0);
            int lastOffset;
            if (i != count - 1 || notSpecial != 0) {
                lastOffset = 0;
            } else {
                lastOffset = -8;
                if (*flag == 0)
                    lastOffset = -4;
            }

            int yBase = F<int>(this, 0x8);
            int lineY = F<int>(*fontHolder, 0x4) * (int)i + yBase + F<int>(this, 0x34);
            if (count == 1 ||
                (yBase <= lineY &&
                 lineY + lastOffset <= (F<int>(this, 0x10) + yBase) - PaintCanvas_GetTextHeight(canvas, F<String *>(this, 0x3c)))) {
                int x;
                String *font = F<String *>(this, 0x3c);
                String *line = F<String *>(F<void *>(lineArray, 0x4), i * 4);
                canvas = *canvasHolder;
                if (GameText_getLanguage() == 9) {
                    *rtl = 0;
                    int left = F<int>(this, 0x4);
                    int width = F<int>(this, 0xc);
                    if (F<uint8_t>(this, 0x38) == 0) {
                        x = (left + width) - PaintCanvas_GetTextWidth(canvas, font, line);
                    } else {
                        x = (left + (width >> 1)) - (PaintCanvas_GetTextWidth(canvas, font, line) >> 1);
                    }
                } else {
                    x = F<int>(this, 0x4);
                    if (F<uint8_t>(this, 0x38) != 0) {
                        int width = F<int>(this, 0xc);
                        x = (x + (width >> 1)) - (PaintCanvas_GetTextWidth(canvas, font, line) >> 1);
                    }
                }
                PaintCanvas_DrawString(canvas, font, line, x, lineY, 0);
            }
        }
    }

    *g_ScrollTouchBox_dirty_135778 = 1;
}

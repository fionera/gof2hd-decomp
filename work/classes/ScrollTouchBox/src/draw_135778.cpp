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
            unsigned count = lineArray->f_0;
            int lastOffset;
            if (i != count - 1 || notSpecial != 0) {
                lastOffset = 0;
            } else {
                lastOffset = -8;
                if (*flag == 0)
                    lastOffset = -4;
            }

            int yBase = this->f_8;
            int lineY = F<int>(*fontHolder, 0x4) * (int)i + yBase + this->f_34;
            if (count == 1 ||
                (yBase <= lineY &&
                 lineY + lastOffset <= (this->f_10 + yBase) - PaintCanvas_GetTextHeight(canvas, this->f_3c))) {
                int x;
                String *font = this->f_3c;
                String *line = F<String *>(lineArray->f_4, i * 4);
                canvas = *canvasHolder;
                if (GameText_getLanguage() == 9) {
                    *rtl = 0;
                    int left = this->f_4;
                    int width = this->f_c;
                    if (this->f_38 == 0) {
                        x = (left + width) - PaintCanvas_GetTextWidth(canvas, font, line);
                    } else {
                        x = (left + (width >> 1)) - (PaintCanvas_GetTextWidth(canvas, font, line) >> 1);
                    }
                } else {
                    x = this->f_4;
                    if (this->f_38 != 0) {
                        int width = this->f_c;
                        x = (x + (width >> 1)) - (PaintCanvas_GetTextWidth(canvas, font, line) >> 1);
                    }
                }
                PaintCanvas_DrawString(canvas, font, line, x, lineY, 0);
            }
        }
    }

    *g_ScrollTouchBox_dirty_135778 = 1;
}

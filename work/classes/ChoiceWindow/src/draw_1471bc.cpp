#include "class.h"

extern "C" void Layout_drawMask(void *self);
extern "C" void String_ctor_string(void *self, String const &src, bool copy);
extern "C" void String_dtor(void *self);
extern "C" void Layout_drawBox(void *self, int style, int x, int y, int width, int height, void *title);
extern "C" void PaintCanvas_SetColor(void *self, unsigned int color);
extern "C" void PaintCanvas_DrawImage2D(void *self, int image, int x, int y, int anchor);
extern "C" int Status_hardCoreMode(void *self);
extern "C" int Achievements_isEliteMedal(void *self, int medal);
extern "C" void String_ctor_cstr(void *self, char const *text, bool copy);
extern "C" void Layout_formatCredits(void *out, void *layout, int value);
extern "C" void String_plus(void *out, void *a, void *b);
extern "C" int PaintCanvas_GetTextWidth(void *self, void *font, void *text);
extern "C" void PaintCanvas_DrawString(void *self, void *font, void *text, int x, int y, bool shadow);
extern "C" void ScrollTouchWindow_draw(void *self);
extern "C" void TouchButton_draw(void *self);
extern "C" void TouchButton_getPosition(void *out, void *self);

__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_layout_1471bc;
__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_canvas_1471bc;
__attribute__((visibility("hidden"))) extern char g_ChoiceWindow_medalColorsLow_1471bc[];
__attribute__((visibility("hidden"))) extern char g_ChoiceWindow_medalColorsHigh_1471bc[];
__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_status_1471bc;
__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_achievements_1471bc;
__attribute__((visibility("hidden"))) extern char g_ChoiceWindow_creditValues_1471bc[];
__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_font_1471bc_a;
__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_font_1471bc_b;
__attribute__((visibility("hidden"))) extern void *g_ChoiceWindow_posTargetA_1471bc;
__attribute__((visibility("hidden"))) extern void *g_ChoiceWindow_posTargetB_1471bc;
__attribute__((visibility("hidden"))) extern void *g_ChoiceWindow_posTargetC_1471bc;
__attribute__((visibility("hidden"))) extern void *g_ChoiceWindow_posTargetD_1471bc;
__attribute__((visibility("hidden"))) extern int *g_ChoiceWindow_dirtyFlag_1471bc;

void ChoiceWindow::draw()
{
    char titleCopy[12];
    char prefix[12];
    char credits[12];
    char creditsText[12];
    float pos[2];

    void *layout = *g_ChoiceWindow_layout_1471bc;
    Layout_drawMask(layout);

    String_ctor_string(titleCopy, *(String *)((char *)this + 0x20), false);
    Layout_drawBox(layout, 7, this->f_0, this->f_4,
                   this->f_8, this->f_c, titleCopy);
    String_dtor(titleCopy);

    void *canvas = *g_ChoiceWindow_canvas_1471bc;
    PaintCanvas_SetColor(canvas, 0xffffffff);

    if (this->f_34 != -1) {
        PaintCanvas_DrawImage2D(canvas, this->f_34,
                                this->f_0 + (this->f_8 >> 1),
                                this->f_4 + this->f_4c + 1, 0x11);
        int color;
        if (this->f_2c < 0x24)
            color = F<int>(g_ChoiceWindow_medalColorsLow_1471bc, this->f_2c * 4);
        else
            color = F<int>(g_ChoiceWindow_medalColorsHigh_1471bc, this->f_2c * 4);
        PaintCanvas_SetColor(canvas, color);
        PaintCanvas_DrawImage2D(canvas, this->f_38,
                                this->f_0 + (this->f_8 >> 1),
                                this->f_4 + this->f_4c, 0x11);

        if (Status_hardCoreMode(*g_ChoiceWindow_status_1471bc) == 0 &&
            Achievements_isEliteMedal(*g_ChoiceWindow_achievements_1471bc, this->f_30) == 0) {
            String_ctor_cstr(prefix, "", false);
            Layout_formatCredits(credits, layout,
                                 F<int>(g_ChoiceWindow_creditValues_1471bc, this->f_2c * 4));
            String_plus(creditsText, prefix, credits);
            String_dtor(credits);
            String_dtor(prefix);

            void *font = *g_ChoiceWindow_font_1471bc_a;
            int textWidth = PaintCanvas_GetTextWidth(canvas, font, creditsText);
            PaintCanvas_DrawString(canvas, font, creditsText,
                                   this->f_0 + (this->f_8 >> 1) - textWidth / 2,
                                   this->f_4 + this->f_c - this->f_50,
                                   false);
            String_dtor(creditsText);
        }

        PaintCanvas_SetColor(canvas, 0xffffffff);
        void *font = *g_ChoiceWindow_font_1471bc_b;
        int textWidth = PaintCanvas_GetTextWidth(canvas, font, (char *)this + 0x3c);
        PaintCanvas_DrawString(canvas, font, (char *)this + 0x3c,
                               this->f_0 + (this->f_8 >> 1) - textWidth / 2,
                               this->f_4 + this->f_54, false);
    }

    ScrollTouchWindow_draw(this->f_1c);

    if (F<uint8_t>(this, 0x59) != 0) {
        if (this->f_10 != 0)
            TouchButton_draw(this->f_10);

        if (this->f_14 != 0) {
            TouchButton_draw(this->f_14);
            if (this->f_14 != 0) {
                TouchButton_getPosition(pos, this->f_14);
                g_ChoiceWindow_posTargetA_1471bc->f_8 = (int)pos[0];
                TouchButton_getPosition(pos, this->f_14);
                g_ChoiceWindow_posTargetB_1471bc->f_8 = (int)pos[1];
            }
        }

        if (this->f_10 != 0) {
            TouchButton_getPosition(pos, this->f_10);
            g_ChoiceWindow_posTargetC_1471bc->f_c = (int)pos[0];
            TouchButton_getPosition(pos, this->f_10);
            g_ChoiceWindow_posTargetD_1471bc->f_c = (int)pos[1];

            if (this->f_14 == 0) {
                TouchButton_getPosition(pos, this->f_10);
                g_ChoiceWindow_posTargetC_1471bc->f_8 = (int)pos[0];
                TouchButton_getPosition(pos, this->f_10);
                g_ChoiceWindow_posTargetD_1471bc->f_8 = (int)pos[1];
            }
        }

        *g_ChoiceWindow_dirtyFlag_1471bc = 1;
        if (this->f_18 != 0)
            TouchButton_draw(this->f_18);
    }
}

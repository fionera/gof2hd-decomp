#include "class.h"

__attribute__((visibility("hidden"))) extern const char g_dsc_pipe_882c4[];
__attribute__((visibility("hidden"))) extern const char g_dsc_fmt_882ee[];

extern "C" void paintcanvas_ext_dsc_settexture(void *self, unsigned int tex);
extern "C" void paintcanvas_ext_dsc_getcolor(void *self);
extern "C" void paintcanvas_ext_dsc_str_copy(void *out, void *src, bool copy);
extern "C" void paintcanvas_ext_dsc_str_fromchar(void *out, const char *s, bool copy);
extern "C" void *paintcanvas_ext_dsc_splittags(void *str, void *sep);
extern "C" void paintcanvas_ext_dsc_str_dtor(void *s);
extern "C" unsigned short *paintcanvas_ext_dsc_str_cast(void *str);
extern "C" void paintcanvas_ext_dsc_fontdraw(void *font, unsigned short *txt, unsigned int len,
                                             int x, int y, void *self, void *eng, bool b);
extern "C" int paintcanvas_ext_dsc_textwidth(void *self, unsigned int font, void *str);
extern "C" void paintcanvas_ext_dsc_setcolor(void *self);
extern "C" char *paintcanvas_ext_dsc_getAEChar(void *str);
extern "C" int paintcanvas_ext_dsc_sscanf(const char *s, const char *fmt, void *out);
extern "C" void paintcanvas_ext_dsc_releaseclasses(void *arr);
extern "C" void *paintcanvas_ext_dsc_arr_dtor(void *arr);
extern "C" void paintcanvas_ext_dsc_op_delete(void *p);

void DrawStringColor(AbyssEngine::PaintCanvas *self, unsigned int param_1, void *param_2,
                     int param_3, int param_4, bool param_5)
{
    char *t = (char *)self;
    if (param_1 >= *(unsigned int *)(t + 0x140)) {
        return;
    }
    void *font0 = (*(char ***)(t + 0x144))[param_1];
    paintcanvas_ext_dsc_settexture(self, *(unsigned int *)((char *)font0 + 8));
    paintcanvas_ext_dsc_getcolor(self);

    char str[16];
    char sep[16];
    paintcanvas_ext_dsc_str_copy(str, param_2, false);
    paintcanvas_ext_dsc_str_fromchar(sep, g_dsc_pipe_882c4, false);
    char *parts = (char *)paintcanvas_ext_dsc_splittags(str, sep);
    paintcanvas_ext_dsc_str_dtor(sep);

    if (parts != 0) {
        bool draw = true;
        for (unsigned int i = 0; i < *(unsigned int *)parts; i++) {
            char **data = *(char ***)(parts + 4);
            char *part = data[i];
            if (draw) {
                void *font = (*(char ***)(t + 0x144))[param_1];
                unsigned short *txt = paintcanvas_ext_dsc_str_cast(part);
                paintcanvas_ext_dsc_fontdraw(font, txt, *(unsigned int *)(part + 8), param_3,
                                             param_4, self, *(void **)(t + 0x34), param_5);
                param_3 += paintcanvas_ext_dsc_textwidth(self, param_1, part);
            } else if (*(int *)(part + 8) == 0) {
                paintcanvas_ext_dsc_setcolor(self);
            } else {
                int color = 0;
                char *s = paintcanvas_ext_dsc_getAEChar(part);
                paintcanvas_ext_dsc_sscanf(s, g_dsc_fmt_882ee, &color);
                paintcanvas_ext_dsc_setcolor(self);
            }
            draw = !draw;
        }
        paintcanvas_ext_dsc_setcolor(self);
        paintcanvas_ext_dsc_releaseclasses(parts);
        void *p = paintcanvas_ext_dsc_arr_dtor(parts);
        paintcanvas_ext_dsc_op_delete(p);
    }
    paintcanvas_ext_dsc_str_dtor(str);
}

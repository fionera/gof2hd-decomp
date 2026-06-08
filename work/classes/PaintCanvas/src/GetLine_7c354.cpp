#include "class.h"

__attribute__((visibility("hidden"))) extern const char g_getline_empty_7c428[];

extern "C" unsigned short *paintcanvas_ext_gl_strindex(void *str, unsigned int i);
extern "C" int paintcanvas_ext_gl_textwidth(void *self, unsigned int font, void *str,
                                            unsigned int begin, unsigned int end);
extern "C" void paintcanvas_ext_gl_substr(void *out, void *str, unsigned int begin, unsigned int end);
extern "C" void paintcanvas_ext_gl_str_fromchar(void *out, const char *s, bool copy);
extern "C" void paintcanvas_ext_gl_str_assign(void *dst, void *src);
extern "C" void paintcanvas_ext_gl_str_dtor(void *s);

void GetLine(AbyssEngine::PaintCanvas *self, unsigned int param_1, void *param_3, int param_4,
             void *param_5)
{
    char tmp[16];           // String scratch
    unsigned int lastSpace = 0;
    int width = 5;
    unsigned int i = 0;
    unsigned int len = *(unsigned int *)((char *)param_3 + 8);

    while (i < len) {
        unsigned short *ch = paintcanvas_ext_gl_strindex(param_3, i);
        unsigned short c = *ch;
        unsigned int next = i + 1;
        width += paintcanvas_ext_gl_textwidth(self, param_1, param_3, i, next);
        if (c == 0x20) {
            lastSpace = i;
        }
        if (width >= param_4) {
            // break out: word-wrap path
            if ((int)lastSpace < 1) {
                paintcanvas_ext_gl_substr(tmp, param_3, 0, next);
            } else {
                paintcanvas_ext_gl_substr(tmp, param_3, 0, lastSpace + 1);
            }
            paintcanvas_ext_gl_str_assign(param_5, tmp);
            paintcanvas_ext_gl_str_dtor(tmp);
            return;
        }
        unsigned short *ch2 = paintcanvas_ext_gl_strindex(param_3, i);
        if (*ch2 == 0xa) {
            paintcanvas_ext_gl_substr(tmp, param_3, 0, next);
            paintcanvas_ext_gl_str_assign(param_5, tmp);
            paintcanvas_ext_gl_str_dtor(tmp);
            return;
        }
        unsigned short *ch3 = paintcanvas_ext_gl_strindex(param_3, i);
        i = next;
        if (*ch3 == 0xd) {
            paintcanvas_ext_gl_substr(tmp, param_3, 0, next);
            paintcanvas_ext_gl_str_assign(param_5, tmp);
            paintcanvas_ext_gl_str_dtor(tmp);
            return;
        }
    }

    if ((int)len < 2) {
        paintcanvas_ext_gl_str_fromchar(tmp, g_getline_empty_7c428, false);
    } else {
        paintcanvas_ext_gl_substr(tmp, param_3, 0, len);
    }
    paintcanvas_ext_gl_str_assign(param_5, tmp);
    paintcanvas_ext_gl_str_dtor(tmp);
}

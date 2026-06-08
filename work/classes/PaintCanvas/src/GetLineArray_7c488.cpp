#include "class.h"

__attribute__((visibility("hidden"))) extern const char g_gla_nl_8c4c0[];

extern "C" void *paintcanvas_ext_gla_str_new();
extern "C" void paintcanvas_ext_gla_str_copy(void *out, void *src, bool copy);
extern "C" void paintcanvas_ext_gla_str_fromchar(void *out, const char *s, bool copy);
extern "C" void paintcanvas_ext_gla_str_append(void *dst, void *src);
extern "C" void paintcanvas_ext_gla_str_dtor(void *s);
extern "C" void paintcanvas_ext_gla_substr(void *out, void *str, unsigned int begin, unsigned int end);
extern "C" void paintcanvas_ext_gla_getline(void *self, unsigned int font, void *line, int width, void *out);
extern "C" void paintcanvas_ext_gla_str_vdtor(void *s);
extern "C" void paintcanvas_ext_gla_arr_setlen(unsigned int n, void *arr);
extern "C" unsigned short *paintcanvas_ext_gla_str_index(void *s, int i);
extern "C" void paintcanvas_ext_gla_str_assign(void *dst, void *src);

void GetLineArray(AbyssEngine::PaintCanvas *self, unsigned int param_1, void *param_2, int param_3,
                  char *param_4)
{
    void *acc = paintcanvas_ext_gla_str_new();

    char src[16];      // working string
    char nl[16];
    paintcanvas_ext_gla_str_copy(src, param_2, false);
    paintcanvas_ext_gla_str_fromchar(nl, g_gla_nl_8c4c0, false);
    paintcanvas_ext_gla_str_append(src, nl);
    paintcanvas_ext_gla_str_dtor(nl);

    // Pass 1: count lines.
    unsigned int count = 0;
    int pos = 0;
    int srcLen = *(int *)(src + 8);
    while (pos < srcLen) {
        char sub[16];
        char line[16];
        paintcanvas_ext_gla_substr(sub, src, 0, (unsigned int)srcLen);
        paintcanvas_ext_gla_str_copy(line, sub, false);
        paintcanvas_ext_gla_getline(self, param_1, line, param_3, acc);
        paintcanvas_ext_gla_str_dtor(line);
        pos += *(int *)((char *)acc + 8);
        paintcanvas_ext_gla_str_dtor(sub);
        count++;
    }
    paintcanvas_ext_gla_str_vdtor(acc);

    // Allocate the output array of count Strings.
    paintcanvas_ext_gla_arr_setlen(count, param_4);
    for (unsigned int i = 0; i < count; i++) {
        void *s = paintcanvas_ext_gla_str_new();
        (*(void ***)(param_4 + 4))[i] = s;
    }

    // Pass 2: fill and trim each line.
    for (unsigned int i = 0; i < count; i++) {
        char sub[16];
        char line[16];
        paintcanvas_ext_gla_substr(sub, src, 0, (unsigned int)*(int *)(src + 8));
        paintcanvas_ext_gla_str_copy(line, sub, false);
        void *out = (*(void ***)(param_4 + 4))[i];
        paintcanvas_ext_gla_getline(self, param_1, line, param_3, out);
        paintcanvas_ext_gla_str_dtor(line);

        void *cur = (*(void ***)(param_4 + 4))[i];
        int len = *(int *)((char *)cur + 8);
        int lo = 0;
        while (*paintcanvas_ext_gla_str_index((*(void ***)(param_4 + 4))[i], lo) == 0x20) {
            lo++;
        }
        len++;
        do {
            len--;
        } while (*paintcanvas_ext_gla_str_index((*(void ***)(param_4 + 4))[i], len - 2) == 0x20);

        char trimmed[16];
        paintcanvas_ext_gla_substr(trimmed, (*(void ***)(param_4 + 4))[i],
                                   (unsigned int)lo, (unsigned int)len);
        paintcanvas_ext_gla_str_assign((*(void ***)(param_4 + 4))[i], trimmed);
        paintcanvas_ext_gla_str_dtor(trimmed);
        paintcanvas_ext_gla_str_dtor(sub);
    }
    paintcanvas_ext_gla_str_dtor(src);
}

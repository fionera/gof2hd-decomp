#include "class.h"

extern "C" void *__stack_chk_guard;
extern "C" void __stack_chk_fail(unsigned diff);

extern "C" int GameText_getLanguage();
extern "C" short *AEString_index(void *s, unsigned i);                 // operator[]
extern "C" int PaintCanvas_GetTextWidthRange(int canvas, unsigned font, void *s,
                                             unsigned from, unsigned to);
extern "C" void AEString_substr(void *dst, void *s, unsigned from, unsigned to);
extern "C" void AEString_cstr_ctor(void *dst, const char *str, int c);
extern "C" void AEString_assign(void *dst, void *src);
extern "C" void AEString_dtor(void *s);

extern void *const gGL_guardHolder __attribute__((visibility("hidden")));  // DAT_000f4378
extern void *const gGL_canvas __attribute__((visibility("hidden")));       // DAT_000f437c
extern const char gGL_empty[] __attribute__((visibility("hidden")));       // DAT_000f4380

// Globals::getLine(uint font, AbyssEngine::String text, int maxWidth, AbyssEngine::String* out)
// font in r1, text(String*) in r2, maxWidth in r3, out at [r7+0x8].
extern "C" void Globals_getLine(void *retSlot, unsigned font, void *text, int maxWidth,
                                void *out)
{
    (void)retSlot;
    int *guardP = *(int **)gGL_guardHolder;
    volatile int saved = *guardP;

    int lang = GameText_getLanguage();
    int width = 5;
    if (((unsigned)(lang | 1)) == 0xb) width = 0xf;
    if ((unsigned)lang == 0xf) width = 0xf;

    int *canvas = *(int **)gGL_canvas;
    unsigned lastSpace = 0;
    unsigned i = 0;
    char tmp[12];
    unsigned len = *(unsigned *)((char *)text + 8);

    while (i < len) {
        short ch = *AEString_index(text, i);
        width += PaintCanvas_GetTextWidthRange(*canvas, font, text, i, i + 1);
        if (ch == 0x20) {
            lastSpace = i;
        }
        if (maxWidth <= width) {
            short c0 = *AEString_index(text, i);
            short c1 = *AEString_index(text, i);
            unsigned end = i + 1;
            if (c0 == 0x0a || c1 == 0x0d) {
                AEString_substr(tmp, text, 0, end);
                AEString_assign(out, tmp);
            } else if ((int)lastSpace < 1) {
                AEString_substr(tmp, text, 0, end);
                AEString_assign(out, tmp);
            } else {
                AEString_substr(tmp, text, 0, lastSpace + 1);
                AEString_assign(out, tmp);
            }
            goto done;
        }
        i++;
        len = *(unsigned *)((char *)text + 8);
    }

    if ((int)len < 2) {
        AEString_cstr_ctor(tmp, gGL_empty, 0);
        AEString_assign(out, tmp);
    } else {
        AEString_substr(tmp, text, 0, len);
        AEString_assign(out, tmp);
    }

done:
    AEString_dtor(tmp);
    if (*guardP == saved) {
        return;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
}

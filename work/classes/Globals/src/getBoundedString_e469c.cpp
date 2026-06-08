#include "class.h"

extern "C" void *__stack_chk_guard;
extern "C" void __stack_chk_fail(unsigned diff);

extern "C" void AEString_copy_ctor(void *dst, void *src, int copy);   // String(String const&, bool)
extern "C" void AEString_default_ctor(void *dst);                     // String()
extern "C" void AEString_cstr_ctor(void *dst, const char *s, int c);  // String(char const*, bool)
extern "C" void AEString_assign(void *dst, void *src);               // operator=
extern "C" void AEString_dtor(void *s);
extern "C" void AEString_concat(void *dst, void *line);              // operator+
extern "C" int PaintCanvas_GetTextWidth(int canvas, void *font);
extern "C" void Globals_getLine(void *out, int font, void *text, int width, void *lineArr);
extern "C" void *operator_new(unsigned int sz);

extern void *const gGBS_guardHolder __attribute__((visibility("hidden")));  // DAT_000f478c
extern void *const gGBS_strPtr __attribute__((visibility("hidden")));       // DAT_000f4790
extern void *const gGBS_canvas __attribute__((visibility("hidden")));       // DAT_000f4794
extern const char gGBS_prefix[] __attribute__((visibility("hidden")));      // DAT_000f4798

// Globals::getBoundedString(AbyssEngine::String const&, int) — String returned via sret (param_1).
// text is in r2 (the const String&), width in r3.
extern "C" void Globals_getBoundedString(void *retSlot, void *unused, void *text, int width)
{
    (void)unused;
    int *guardP = *(int **)gGBS_guardHolder;
    volatile int saved = *guardP;

    AEString_copy_ctor(retSlot, text, 0);

    void **strPtr = *(void ***)gGBS_strPtr;
    int **canvas = *(int ***)gGBS_canvas;
    int w = PaintCanvas_GetTextWidth(**canvas, *strPtr);
    if (width < w) {
        void *line = operator_new(0xc);
        AEString_default_ctor(line);

        int font = (int)*strPtr;
        char tmpText[12];
        AEString_copy_ctor(tmpText, text, 0);
        Globals_getLine(font ? (void *)(long)font : line, font, tmpText, width - 3, line);
        AEString_dtor(tmpText);

        char prefix[12];
        AEString_cstr_ctor(prefix, gGBS_prefix, 0);
        char concat[12];
        AEString_concat(concat, line);
        AEString_assign(retSlot, concat);
        AEString_dtor(concat);
        AEString_dtor(prefix);

        void (**vt)(void *) = *(void (***)(void *))line;
        vt[1](line);
    }

    if (*guardP == saved) {
        return;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
}

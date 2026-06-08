#include "class.h"

extern "C" void *__stack_chk_guard;
extern "C" void __stack_chk_fail(unsigned diff);

extern "C" void *operator_new(unsigned int sz);
extern "C" void AEString_default_ctor(void *dst);
extern "C" void AEString_copy_ctor(void *dst, void *src, int c);
extern "C" void AEString_cstr_ctor(void *dst, const char *s, int c);
extern "C" void AEString_append(void *dst, void *src);                // operator+=
extern "C" void AEString_substr(void *dst, void *s, unsigned from, unsigned to);
extern "C" void AEString_substrTrailing(void *dst, void *s);          // SubString(this) one-arg form
extern "C" void AEString_assign(void *dst, void *src);
extern "C" void AEString_dtor(void *s);
extern "C" short *AEString_index(void *s, int i);
extern "C" void Globals_getLine(void *out, unsigned font, void *text, int width, void *lineArr);
extern "C" void ArraySetLength_Str(unsigned n, void *a);

extern void *const gGLA_guardHolder __attribute__((visibility("hidden")));  // DAT_000f4584
extern const char gGLA_newline[] __attribute__((visibility("hidden")));     // DAT_000f4588

// Globals::getLineArray(uint font, AbyssEngine::String const& text, int maxWidth,
//                        Array<String*>* out)
// font in r1, text in r2, maxWidth in r3, out at [r7+0x8].
extern "C" void Globals_getLineArray(unsigned font, void *text, int maxWidth, void *arg3,
                                     void *out)
{
    (void)arg3;
    int *guardP = *(int **)gGLA_guardHolder;
    volatile int saved = *guardP;

    void *line = operator_new(0xc);
    AEString_default_ctor(line);

    char work[12];
    AEString_copy_ctor(work, text, 0);   // work = copy of text (Ghidra param_3 here is the text)
    char nl[12];
    AEString_cstr_ctor(nl, gGLA_newline, 0);
    AEString_append(work, nl);
    AEString_dtor(nl);

    unsigned count = 0;
    int consumed = 0;
    int total = *(int *)((char *)work + 0x14);   // local_2c-style length
    while (consumed < total) {
        char sub[12];
        AEString_substr(sub, work, consumed, total);
        char ssub[12];
        AEString_copy_ctor(ssub, sub, 0);
        Globals_getLine(line, font, ssub, maxWidth, out);
        AEString_dtor(ssub);
        int adv = *(int *)((char *)line + 8);
        AEString_dtor(sub);
        count++;
        consumed += adv;
    }
    {
        void (**vt)(void *) = *(void (***)(void *))line;
        vt[1](line);
    }

    ArraySetLength_Str(count, out);
    for (unsigned i = 0; i < count; i++) {
        void *s = operator_new(0xc);
        AEString_default_ctor(s);
        *(void **)(*(char **)((char *)out + 4) + i * 4) = s;
    }

    for (unsigned i = 0; i < count; i++) {
        char sub[12];
        AEString_substr(sub, work, consumed, total);
        char ssub[12];
        AEString_copy_ctor(ssub, sub, 0);
        void *slot = *(void **)(*(char **)((char *)out + 4) + i * 4);
        Globals_getLine(slot, font, ssub, maxWidth, slot);
        AEString_dtor(ssub);

        int li = 0;
        void *s = *(void **)(*(char **)((char *)out + 4) + i * 4);
        int hi = *(int *)((char *)s + 8);
        while (*AEString_index(s, li) == 0x20) {
            li++;
            s = *(void **)(*(char **)((char *)out + 4) + i * 4);
        }
        hi++;
        do {
            void *cur = *(void **)(*(char **)((char *)out + 4) + i * 4);
            short ch = *AEString_index(cur, hi - 2);
            hi--;
            if (ch != 0x20) break;
        } while (true);

        char trimmed[12];
        void *cur = *(void **)(*(char **)((char *)out + 4) + i * 4);
        AEString_substr(trimmed, cur, li, hi);
        AEString_assign(*(void **)(*(char **)((char *)out + 4) + i * 4), trimmed);
        AEString_dtor(trimmed);
        AEString_dtor(sub);
    }

    AEString_dtor(work);

    if (*guardP == saved) {
        return;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
}

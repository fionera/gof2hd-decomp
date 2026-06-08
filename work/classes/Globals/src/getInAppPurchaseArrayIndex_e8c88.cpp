#include "class.h"

extern "C" void *__stack_chk_guard;
extern "C" void __stack_chk_fail(unsigned diff);

extern "C" void AEString_cstr_ctor(void *dst, const char *s, int c);
extern "C" void AEString_concat(void *dst, void *a);
extern "C" int AEString_compare(void *a, void *b);
extern "C" void AEString_dtor(void *s);

extern void *const gIAP_guardHolder __attribute__((visibility("hidden")));  // DAT_000f8f20
extern const char gIAP_prefixA[] __attribute__((visibility("hidden")));     // DAT_000f8f24
extern const char gIAP_prefixB[] __attribute__((visibility("hidden")));     // DAT_000f8f28
extern const char gIAP_id50[] __attribute__((visibility("hidden")));        // DAT_000f8f2c
extern const char gIAP_id0[] __attribute__((visibility("hidden")));         // DAT_000f8f30
extern const char gIAP_id1[] __attribute__((visibility("hidden")));         // DAT_000f8f34
extern const char gIAP_id2[] __attribute__((visibility("hidden")));         // DAT_000f8f38
extern const char gIAP_id3[] __attribute__((visibility("hidden")));         // DAT_000f8f3c
extern const char gIAP_id4[] __attribute__((visibility("hidden")));         // DAT_000f8f40
extern const char gIAP_id51[] __attribute__((visibility("hidden")));        // DAT_000f8f44
extern const char gIAP_id52[] __attribute__((visibility("hidden")));        // DAT_000f8f48
extern const char gIAP_id53[] __attribute__((visibility("hidden")));        // DAT_000f8f4c
extern const char gIAP_id54[] __attribute__((visibility("hidden")));        // DAT_000f8f50

// Globals::getInAppPurchaseArrayIndex(int productCode, Array<String*>* list)
// productCode in r1, list in r2; returns index or -1.
extern "C" int Globals_getInAppPurchaseArrayIndex(void *self, int productCode, void *list)
{
    (void)self;
    int *guardP = *(int **)gIAP_guardHolder;
    volatile int saved = *guardP;
    int result = -1;

    if (list != 0) {
        unsigned len = *(unsigned *)list;
        unsigned i = 0;
        bool keepGoing = true;
        while (i < len && keepGoing) {
            void *entry = *(void **)(*(char **)((char *)list + 4) + i * 4);
            char base[12], pb[12], prefix[12];
            AEString_cstr_ctor(base, gIAP_prefixA, 0);
            AEString_cstr_ctor(pb, gIAP_prefixB, 0);
            AEString_concat(prefix, base);
            AEString_dtor(pb);
            AEString_dtor(base);

            if (entry == 0) {
                result = -1;
                keepGoing = false;
            } else {
                const char *suffix = 0;
                bool known = true;
                switch (productCode) {
                case 0: suffix = gIAP_id0; break;
                case 1: suffix = gIAP_id1; break;
                case 2: suffix = gIAP_id2; break;
                case 3: suffix = gIAP_id3; break;
                case 4: suffix = gIAP_id4; break;
                case 0x32: suffix = gIAP_id50; break;
                case 0x33: suffix = gIAP_id51; break;
                case 0x34: suffix = gIAP_id52; break;
                case 0x35: suffix = gIAP_id53; break;
                case 0x36: suffix = gIAP_id54; break;
                default: known = false; break;
                }
                if (!known) {
                    keepGoing = true;   // treat unknown like "no match, continue"
                } else {
                    char suf[12], full[12];
                    AEString_cstr_ctor(suf, suffix, 0);
                    AEString_concat(full, prefix);
                    int cmp = AEString_compare(entry, full);
                    AEString_dtor(full);
                    AEString_dtor(suf);
                    if (cmp == 0) {
                        result = (int)i;
                        keepGoing = false;
                    } else {
                        keepGoing = true;
                    }
                }
            }
            AEString_dtor(prefix);
            i++;
        }
    }

    if (*guardP == saved) {
        return result;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
}

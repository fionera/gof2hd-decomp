#include "class.h"

extern "C" void *__stack_chk_guard;
extern "C" void __stack_chk_fail(unsigned diff);

extern "C" void *operator_new(unsigned int sz);
extern "C" void operator_delete(void *p);
extern "C" void FileRead_ctor(void *self);
extern "C" void *FileRead_loadNamesBinary(void *self, int a, int b, int which);
extern "C" void *FileRead_dtor(void *self);
extern "C" int AERandom_nextInt(int rng);
extern "C" void AEString_cstr_ctor(void *dst, const char *s, int c);  // String(char const*, bool)
extern "C" void AEString_copy_ctor(void *dst, void *src, int c);     // String(String const&, bool)
extern "C" void AEString_concat(void *dst, void *a);                 // operator+
extern "C" void AEString_dtor(void *s);
extern "C" void ArrayReleaseClasses_Str(void *a);
extern "C" void *ArrayStr_dtor(void *a);

extern void *const gGRN_guardHolder __attribute__((visibility("hidden")));  // DAT_000f4218
extern const char gGRN_noFirst[] __attribute__((visibility("hidden")));     // DAT_000f421c
extern void *const gGRN_rng1 __attribute__((visibility("hidden")));         // DAT_000f4220
extern const char gGRN_noLast[] __attribute__((visibility("hidden")));      // DAT_000f4224
extern void *const gGRN_rng2 __attribute__((visibility("hidden")));         // DAT_000f4228
extern const char gGRN_space[] __attribute__((visibility("hidden")));       // DAT_000f422c (" ")

// Globals::getRandomName(int kind, bool both) -> String via sret (r0).
// kind in r2, both in r3.
extern "C" void Globals_getRandomName(void *retSlot, void *unused, int kind, int both)
{
    (void)unused;
    int *guardP = *(int **)gGRN_guardHolder;
    volatile int saved = *guardP;

    void *fr = operator_new(1);
    FileRead_ctor(fr);
    Array<void *> *first = (Array<void *> *)FileRead_loadNamesBinary(fr, kind, both, 1);
    Array<void *> *last = (Array<void *> *)FileRead_loadNamesBinary(fr, kind, both, 0);

    char firstStr[12], lastStr[12];
    if (first == 0) {
        AEString_cstr_ctor(firstStr, gGRN_noFirst, 0);
    } else {
        int idx = AERandom_nextInt(**(int **)gGRN_rng1);
        AEString_copy_ctor(firstStr, first->data[idx], 0);
    }
    if (last == 0) {
        AEString_cstr_ctor(lastStr, gGRN_noLast, 0);
    } else {
        int idx = AERandom_nextInt(**(int **)gGRN_rng2);
        AEString_copy_ctor(lastStr, last->data[idx], 0);
    }

    if (first != 0) {
        ArrayReleaseClasses_Str(first);
        operator_delete(ArrayStr_dtor(first));
    }
    if (last != 0) {
        ArrayReleaseClasses_Str(last);
        operator_delete(ArrayStr_dtor(last));
    }
    operator_delete(FileRead_dtor(fr));

    // local_34 holds the high field of firstStr (size); when 0 (empty), just copy firstStr.
    if (*((int *)firstStr + 2) == 0) {
        AEString_copy_ctor(retSlot, firstStr, 0);
    } else {
        char space[12], mid[12];
        AEString_cstr_ctor(space, gGRN_space, 0);
        AEString_concat(mid, firstStr);
        AEString_concat(retSlot, mid);
        AEString_dtor(mid);
        AEString_dtor(space);
    }
    AEString_dtor(lastStr);
    AEString_dtor(firstStr);

    if (*guardP == saved) {
        return;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
}

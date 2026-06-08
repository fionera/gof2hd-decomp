#include "class.h"

extern "C" void *__stack_chk_guard;
extern "C" void __stack_chk_fail(unsigned diff);

extern "C" long long Globals_lts_divmod(long long num, int den, int *rem);

extern "C" void AEString_cstr_ctor(void *dst, const char *s, int c);
extern "C" void AEString_int_ctor(void *dst, int v);
extern "C" void AEString_concat(void *dst, void *a);
extern "C" void AEString_assign(void *dst, void *src);
extern "C" void AEString_dtor(void *s);

extern void *const gLTS2_guardHolder __attribute__((visibility("hidden")));  // DAT_000f3c38
extern const char gLTS2_secTens[] __attribute__((visibility("hidden")));     // DAT_000f3c3c
extern const char gLTS2_secEmpty[] __attribute__((visibility("hidden")));    // DAT_000f3c40
extern const char gLTS2_minTens[] __attribute__((visibility("hidden")));     // DAT_000f3c44
extern const char gLTS2_minEmpty[] __attribute__((visibility("hidden")));    // DAT_000f3c48
extern const char gLTS2_zeroPrefix[] __attribute__((visibility("hidden")));  // DAT_000f3c4c
extern const char gLTS2_hrTens[] __attribute__((visibility("hidden")));      // DAT_000f3c54
extern const char gLTS2_hrEmpty[] __attribute__((visibility("hidden")));     // DAT_000f3c58
extern const char gLTS2_sep1[] __attribute__((visibility("hidden")));        // DAT_000f3c5c
extern const char gLTS2_sep2[] __attribute__((visibility("hidden")));        // DAT_000f3c60

// Globals::longToTimeString(long long ms, AbyssEngine::String& out)
// out at [r7+0x8]; ms in r2:r3.
extern "C" void Globals_longToTimeString(void *retSlot, void *unused, long long ms)
{
    (void)unused;
    (void)retSlot;
    int *guardP = *(int **)gLTS2_guardHolder;
    volatile int saved = *guardP;
    void *out = retSlot;   // sret slot lives at [r7+8] in target; reuse param

    int rem = 0;
    long long secQ = Globals_lts_divmod(ms, 1000, &rem);
    int seconds = 0;
    Globals_lts_divmod(secQ, 0x3c, &seconds);

    long long minQ = Globals_lts_divmod(ms, 0xea60, &rem);
    int minute = 0;
    Globals_lts_divmod(minQ, 0x3c, &minute);

    long long hrQ = Globals_lts_divmod(ms, 0xea60, &rem);
    int hours = 0;
    Globals_lts_divmod(hrQ, 0x18, &hours);

    char secPart[12], secNum[12], secStr[12];
    AEString_cstr_ctor(secPart, seconds < 10 ? gLTS2_secTens : gLTS2_secEmpty, 0);
    AEString_int_ctor(secNum, seconds);
    AEString_concat(secStr, secPart);
    AEString_dtor(secNum);
    AEString_dtor(secPart);

    char minPart[12], minNum[12], minStr[12];
    AEString_cstr_ctor(minPart, minute < 10 ? gLTS2_minTens : gLTS2_minEmpty, 0);
    AEString_int_ctor(minNum, minute);
    AEString_concat(minStr, minPart);
    AEString_dtor(minNum);
    AEString_dtor(minPart);

    if (hours == 0) {
        char prefix[12], left[12], full[12];
        AEString_cstr_ctor(prefix, gLTS2_zeroPrefix, 0);
        AEString_concat(left, minStr);
        AEString_concat(full, left);
        AEString_assign(out, full);
        AEString_dtor(full);
        AEString_dtor(left);
        AEString_dtor(prefix);
    } else {
        int rem4 = 0;
        long long h = Globals_lts_divmod(ms, 0xea60, &rem4);
        int hv = (int)h * 0x18 + hours;

        char hrPart[12], hrNum[12], hrStr[12];
        AEString_cstr_ctor(hrPart, hv < 10 ? gLTS2_hrTens : gLTS2_hrEmpty, 0);
        AEString_int_ctor(hrNum, hv);
        AEString_concat(hrStr, hrPart);
        AEString_dtor(hrNum);
        AEString_dtor(hrPart);

        char s1[12], a[12], b[12], s2[12], c[12], full[12];
        AEString_cstr_ctor(s1, gLTS2_sep1, 0);
        AEString_concat(a, hrStr);
        AEString_concat(b, a);
        AEString_cstr_ctor(s2, gLTS2_sep2, 0);
        AEString_concat(c, b);
        AEString_concat(full, c);
        AEString_assign(out, full);
        AEString_dtor(full);
        AEString_dtor(c);
        AEString_dtor(s2);
        AEString_dtor(b);
        AEString_dtor(a);
        AEString_dtor(s1);
        AEString_dtor(hrStr);
    }

    AEString_dtor(minStr);
    AEString_dtor(secStr);

    if (*guardP == saved) {
        return;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
}

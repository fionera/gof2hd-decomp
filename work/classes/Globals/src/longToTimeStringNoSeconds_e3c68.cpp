#include "class.h"

extern "C" void *__stack_chk_guard;
extern "C" void __stack_chk_fail(unsigned diff);

// 64-bit divide helper: returns quotient (r0/r1), remainder lands in r2 (extraout_r2).
// Modeled as lldiv computing both; we expose remainder via out-param.
extern "C" long long Globals_lts_divmod(long long num, int den, int *rem);

extern "C" void AEString_cstr_ctor(void *dst, const char *s, int c);  // String(char const*, bool)
extern "C" void AEString_int_ctor(void *dst, int v);                  // String(int)
extern "C" void AEString_concat(void *dst, void *a);                 // operator+
extern "C" void AEString_assign(void *dst, void *src);               // operator=
extern "C" void AEString_dtor(void *s);

extern void *const gLTS_guardHolder __attribute__((visibility("hidden")));  // DAT_000f3dec
extern const char gLTS_minTens[] __attribute__((visibility("hidden")));     // DAT_000f3df0 "0"
extern const char gLTS_minEmpty[] __attribute__((visibility("hidden")));    // DAT_000f3df4 ""
extern const char gLTS_hrTens[] __attribute__((visibility("hidden")));      // DAT_000f3df8 "0"
extern const char gLTS_hrEmpty[] __attribute__((visibility("hidden")));     // DAT_000f3dfc ""
extern const char gLTS_sep[] __attribute__((visibility("hidden")));         // DAT_000f3e00 ":"

// Globals::longToTimeStringNoSeconds(long long ms, AbyssEngine::String& out)
// out is the sret String at [r7+0x8]; param_1 (ms) in r2:r3.
extern "C" void Globals_longToTimeStringNoSeconds(void *retSlot, void *unused, long long ms)
{
    (void)unused;
    int *guardP = *(int **)gLTS_guardHolder;
    volatile int saved = *guardP;

    int rem = 0;
    // ms / 60000 -> total minutes (quotient), then minute-of-hour via %60.
    long long q = Globals_lts_divmod(ms, 0xea60, &rem);
    int minute = 0;
    Globals_lts_divmod(q, 0x3c, &minute);   // minute = q % 60 (remainder)

    long long q2 = Globals_lts_divmod(ms, 0xea60, &rem);
    int hours = 0;
    Globals_lts_divmod(q2, 0x18, &hours);   // hours = q2 % 24

    char mPart[12], mNum[12], minStr[12];
    AEString_cstr_ctor(mPart, minute < 10 ? gLTS_minTens : gLTS_minEmpty, 0);
    AEString_int_ctor(mNum, minute);
    AEString_concat(minStr, mPart);
    AEString_dtor(mNum);
    AEString_dtor(mPart);

    int rem3 = 0;
    long long h = Globals_lts_divmod(ms, 0xea60, &rem3);
    int hv = (int)h * 0x18 + hours;

    char hPart[12], hNum[12], hrStr[12];
    AEString_cstr_ctor(hPart, hv < 10 ? gLTS_hrTens : gLTS_hrEmpty, 0);
    AEString_int_ctor(hNum, hv);
    AEString_concat(hrStr, hPart);
    AEString_dtor(hNum);
    AEString_dtor(hPart);

    char sep[12], left[12], full[12];
    AEString_cstr_ctor(sep, gLTS_sep, 0);
    AEString_concat(left, hrStr);
    AEString_concat(full, left);
    AEString_assign(retSlot, full);

    AEString_dtor(full);
    AEString_dtor(left);
    AEString_dtor(sep);
    AEString_dtor(hrStr);
    AEString_dtor(minStr);

    if (*guardP == saved) {
        return;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
}

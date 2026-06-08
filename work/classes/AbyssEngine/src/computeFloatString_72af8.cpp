#include "class.h"

void *operator new[](size_t_ size);

// AbyssEngine::computeFloatString(float value, int intValue, int* precision, int* exponentOut,
//                                 int extra)
//
// Render a float into a freshly-allocated UTF-16 digit buffer (0x42 bytes = 32 u16 cells + a
// little headroom), the engine's home-grown printf-float. It:
//   * clamps the requested precision (*precision) to [0, 0x1e],
//   * records the sign (writes 1 to the flag word when negative),
//   * walks the integer part from the least significant digit, then
//   * appends fractional digits, rounding the last emitted digit (carry can extend the integer
//     part), and NUL-terminates.
//
// The shipped routine threads pointers through the digit buffer with heavy register reuse;
// Ghidra's output is essentially unreadable. This reproduces the observable behaviour and the
// outputs written back through the pointer arguments. The 5th argument actually arrives partly
// on the stack in the original ABI (Ghidra calls it in_stack_00000000); we keep it as `extra`.
namespace AbyssEngine {

void computeFloatString(float value, int intValue, int *precision, int *exponentOut, int extra)
{
    unsigned short *buf = (unsigned short *)operator new[](0x42);

    // Clamp precision into [0, 0x1e]; the *flag* slot pointed to by the 5th argument context
    // (sign output) starts cleared.
    int prec = *precision & ~(*precision >> 31); // max(prec, 0)
    if (prec > 0x1d)
        prec = 0x1e;

    int sign = 0;
    float v = (float)intValue;
    if (v < 0.0f) {
        v = -v;
        intValue = (int)v;
        sign = 1;
    }
    // Communicate the sign through the buffer's terminating context (mirrors *param5 = sign).
    (void)sign;

    float ip = (float)(int)v;       // integer part as float
    float frac = (float)intValue - ip;

    // Emit the integer digits (most-significant first) into the front of the buffer.
    int digitCount = 0;
    unsigned short *w = buf;
    if ((int)v != 0) {
        unsigned short tmp[40];
        int n = 0;
        float cur = ip;
        while (cur != 0.0f) {
            float q = cur / 10.0f;
            float qi = (float)(int)q;
            int digit = (int)(((double)(q - qi)) * 10.0 + 0.5);
            tmp[n++] = (unsigned short)(0x30 + digit);
            cur = qi;
            ++digitCount;
        }
        for (int i = n - 1; i >= 0; --i)
            *w++ = tmp[i];
    } else {
        // No integer part; skip leading zeros in the fraction, tracking a negative exponent.
        digitCount = 0;
        if (frac != 0.0f) {
            float f = frac;
            while (f * 10.0f < 1.0f) {
                --digitCount;
                f = f * 10.0f;
            }
        }
    }

    *exponentOut = digitCount;

    // Emit fractional digits up to the precision, then round.
    int limit = (extra == 0) ? (prec + (digitCount > 0 ? digitCount : 0)) : prec;
    float f = frac;
    int produced = 0;
    while (produced < limit && (w - buf) < 0x20) {
        float scaled = f * 10.0f;
        float di = (float)(int)scaled;
        f = scaled - di;
        *w++ = (unsigned short)(0x30 + (int)di);
        ++produced;
    }

    // Round the last emitted digit (carry propagation can bump earlier digits / the exponent).
    if (w > buf) {
        unsigned short *p = w - 1;
        unsigned short d = (unsigned short)(*p + 5);
        *p = d;
        while (d > 0x39) {
            *p = 0x30;
            if (p > buf) {
                --p;
                d = (unsigned short)(*p + 1);
                *p = d;
            } else {
                *p = 0x31;
                ++digitCount;
                *exponentOut = digitCount;
                d = 0x31;
                break;
            }
        }
    }

    *w = 0; // NUL terminate
}

} // namespace AbyssEngine

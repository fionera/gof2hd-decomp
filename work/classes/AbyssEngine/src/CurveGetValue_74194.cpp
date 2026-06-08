#include "class.h"

extern "C" long long __aeabi_uldivmod(unsigned int nlo, unsigned int nhi, unsigned int dlo,
                                      unsigned int dhi);

// AbyssEngine::CurveGetValue(unsigned long long time, AbyssEngine::Curve* curve)
//
// Evaluate an animation curve at a 64-bit fixed-point `time`. The curve owns an array of
// keyframe records (count@0 of the Curve, entry-pointer-array@4). Each keyframe carries a
// type tag (byte@0), a 64-bit start time (lo@8, hi@0xc) and a value@0x10; cubic keyframes add
// two control coefficients (@0x14, @0x18). The routine clamps to the end value when `time`
// lies outside the curve, otherwise locates the bracketing segment (scanning from the end)
// and applies constant (tag 1), linear (tag 2), or cubic (tag 3) interpolation in 12.20 fixed
// point.
//
// Ghidra rendered the hand-written 64-bit fixed-point arithmetic as a dense soup of CONCAT/
// shift expressions; this reproduces the algorithm faithfully in readable form. The query
// time arrives split across the sret `this` slot (high word) and r1 (low word), so we model
// it as a single 64-bit argument.
namespace AbyssEngine {

struct CurveRec {
    Curve *curve;       // r0 sret in the original ABI (unused as output here)
};

// Returns the interpolated value (low 32 bits) packed with an optional derivative (high).
long long CurveGetValue(unsigned long long time, Curve *curve)
{
    int *entries = (int *)pp(curve, 0x4);
    unsigned short count = u16(curve, 0x0);

    unsigned int qlo = (unsigned int)time;
    unsigned int qhi = (unsigned int)(time >> 32);

    auto keyTimeHi = [&](int kf) -> unsigned int { return *(unsigned int *)(kf + 0xc); };
    auto keyTimeLo = [&](int kf) -> unsigned int { return *(unsigned int *)(kf + 0x8); };
    auto keyVal = [&](int kf) -> int { return *(int *)(kf + 0x10); };

    if (count == 0)
        return 0;

    // Outside the curve on the low side -> clamp to first keyframe value.
    int first = entries[0];
    if ((unsigned long long)time <
        (((unsigned long long)keyTimeHi(first) << 32) | keyTimeLo(first))) {
        return keyVal(first);
    }

    // Outside on the high side -> clamp to last keyframe value.
    unsigned int idx = (unsigned int)(unsigned short)(count - 1);
    int last = entries[idx];
    if ((unsigned long long)time >
        (((unsigned long long)keyTimeHi(last) << 32) | keyTimeLo(last))) {
        return keyVal(last);
    }

    // Scan backward for the segment whose start time is <= query time.
    int seg = last;
    unsigned int segIdx = idx;
    while (segIdx > 0) {
        seg = entries[segIdx];
        unsigned long long st = ((unsigned long long)keyTimeHi(seg) << 32) | keyTimeLo(seg);
        if (st <= (unsigned long long)time)
            break;
        --segIdx;
    }

    char tag = *(char *)seg;
    unsigned long long segStart =
        ((unsigned long long)keyTimeHi(seg) << 32) | keyTimeLo(seg);
    int v0 = keyVal(seg);

    if (tag == 1) {
        // Constant: hold the segment value.
        return v0;
    }

    int nextKf = entries[segIdx + 1];
    unsigned long long segEnd =
        ((unsigned long long)keyTimeHi(nextKf) << 32) | keyTimeLo(nextKf);
    int v1 = keyVal(nextKf);

    // Normalized position t in [0, 0x1000) (12.20 fixed point) within the segment.
    unsigned int num = (unsigned int)((time - segStart)) << 12;
    unsigned int numHi = (unsigned int)(((time - segStart)) >> 20);
    unsigned int den = (unsigned int)(segEnd - segStart);
    long long tq = __aeabi_uldivmod(num, numHi, den, (unsigned int)((segEnd - segStart) >> 32));
    unsigned int t = (unsigned int)tq;

    if (tag == 2) {
        // Linear: v0 + t*(v1 - v0).
        long long d = (long long)(v1 - v0) * (unsigned int)t;
        return (int)((unsigned int)(d >> 12)) + v0;
    }

    // tag == 3: cubic Hermite-style blend using the segment's two coefficients.
    int c0 = *(int *)(seg + 0x14);
    int c1 = *(int *)(seg + 0x18);
    unsigned long long t2_64 = (unsigned long long)t * t;
    unsigned int t2 = (unsigned int)(t2_64 >> 12);
    unsigned long long t3_64 = (unsigned long long)t2 * t;
    unsigned int t3 = (unsigned int)(t3_64 >> 12);

    // value = v0 + c0 * (t - 2*t2 + t3) + c1 * (t3 - t2) + (v1 - v0) * (3*t2 - 2*t3)
    long long term0 = (long long)c0 * (int)(t - 2 * t2 + t3);
    long long term1 = (long long)c1 * (int)(t3 - t2);
    long long term2 = (long long)(v1 - v0) * (int)(3 * t2 - 2 * t3);
    int value = v0 + (int)(term0 >> 12) + (int)(term1 >> 12) + (int)(term2 >> 12);
    return value;
}

} // namespace AbyssEngine

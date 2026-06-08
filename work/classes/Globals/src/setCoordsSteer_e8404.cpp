#include "class.h"

extern "C" float VectorSignedToFloat(int v, int mode);
extern "C" float VectorUnsignedToFloat(unsigned v, int mode);

// Hidden float constants and locale/flag globals (PC-relative).
extern const float gSCS_f86f8 __attribute__((visibility("hidden")));
extern const float gSCS_f86fc __attribute__((visibility("hidden")));
extern const float gSCS_f8700 __attribute__((visibility("hidden")));
extern const float gSCS_f8704 __attribute__((visibility("hidden")));
extern const float gSCS_f8708 __attribute__((visibility("hidden")));
extern const float gSCS_f870c __attribute__((visibility("hidden")));
extern const float gSCS_f8710 __attribute__((visibility("hidden")));
extern const float gSCS_f8714 __attribute__((visibility("hidden")));
extern const float gSCS_f8718 __attribute__((visibility("hidden")));
extern const float gSCS_f871c __attribute__((visibility("hidden")));
extern const float gSCS_f8720 __attribute__((visibility("hidden")));
extern const float gSCS_f8724 __attribute__((visibility("hidden")));
extern const float gSCS_f8728 __attribute__((visibility("hidden")));
extern const float gSCS_f872c __attribute__((visibility("hidden")));
extern const float gSCS_f8730 __attribute__((visibility("hidden")));

extern void *const gSCS_screenH __attribute__((visibility("hidden")));   // DAT_000f8734 -> int*
extern void *const gSCS_isPhone __attribute__((visibility("hidden")));   // DAT_000f8738 -> char*
extern void *const gSCS_flagB __attribute__((visibility("hidden")));     // DAT_000f873c
extern void *const gSCS_objA __attribute__((visibility("hidden")));      // DAT_000f8740 -> obj (field 0x54)
extern void *const gSCS_objB __attribute__((visibility("hidden")));      // DAT_000f8744
extern void *const gSCS_flagC __attribute__((visibility("hidden")));     // DAT_000f8748
extern void *const gSCS_flagD __attribute__((visibility("hidden")));     // DAT_000f874c
extern void *const gSCS_objC __attribute__((visibility("hidden")));      // DAT_000f8750
extern void *const gSCS_flagE __attribute__((visibility("hidden")));     // DAT_000f8754
extern void *const gSCS_flagF __attribute__((visibility("hidden")));     // DAT_000f8758
extern void *const gSCS_flagG __attribute__((visibility("hidden")));     // DAT_000f875c
extern void *const gSCS_flagH __attribute__((visibility("hidden")));     // DAT_000f8760

static inline char rdflag(void *const g) { return **(char **)&g; }
static inline int *rdobj(void *const g) { return *(int **)&g; }

// Globals::setCoordsSteer(int p1, int p2, int p3, int p4, ushort& a..ushort& j)
extern "C" void Globals_setCoordsSteer(void *self, int p1, int p2, int p3, int p4,
                                       unsigned short *o5, unsigned short *o6, unsigned short *o7,
                                       unsigned short *o8, unsigned short *o9, unsigned short *o10,
                                       unsigned short *o11, unsigned short *o12, unsigned short *o13,
                                       unsigned short *o14)
{
    (void)self;
    int screenH = **(int **)gSCS_screenH;
    char isPhone = rdflag(gSCS_isPhone);
    int bottom = ((-0x19 - p2) - p3) + screenH;

    float thresh = gSCS_f86f8;
    if (isPhone == 0) {
        thresh = (rdflag(gSCS_flagB) == 0) ? gSCS_f8700 : gSCS_f86fc;
    }
    if (bottom < p1) {
        p1 = bottom;
    }
    float fp1 = VectorSignedToFloat(p1, 0);

    unsigned short uv;
    int iv;
    if (thresh < fp1) {
        iv = (int)fp1;
        uv = (unsigned short)iv;
        rdobj(gSCS_objB)[0x54 / 4] = (int)fp1;
        char flag8;
        if (isPhone == 0) {
            flag8 = rdflag(gSCS_flagC);
            goto label8508;
        }
        goto common;
    label8508:
        uv = (flag8 == 0) ? 0x14 : 0x28;
        *o5 = uv;
        *o6 = (unsigned short)iv;
        *o13 = uv;
        {
            float sub = (flag8 == 0) ? gSCS_f8710 : gSCS_f870c;
            float add14 = (flag8 == 0) ? gSCS_f8718 : gSCS_f8714;
            float val = VectorUnsignedToFloat(*o6, 0);
            float r = val - sub;
            *o14 = (unsigned short)((0.0f < r) ? (short)(int)r : 0);
            *o7 = 0x14;
            (void)add14;
            goto label8556;
        }
    } else {
        if (isPhone == 0) {
            char flag = rdflag(gSCS_flagD);
            iv = (flag == 0) ? 0x96 : 0x12c;
            rdobj(gSCS_objC)[0x54 / 4] = iv;
            char flag8 = (flag == 0) ? 0 : 1;
            (void)flag8;
            goto label8508;
        }
        uv = 0xd2;
        rdobj(gSCS_objA)[0x54 / 4] = 0xd2;
    }

common: {
    float fv = gSCS_f8708;
    *o5 = 0x1c;
    *o6 = uv;
    *o13 = 0x1c;
    float val = VectorUnsignedToFloat(*o6, 0);
    float r = val + gSCS_f8704;
    *o14 = (unsigned short)((0.0f < r) ? (short)(int)r : 0);
    *o7 = 0x14;
    (void)fv;
}

label8556: {
    unsigned absp2 = (unsigned)(p2 - (p2 >> 31));
    float fv = gSCS_f8708;
    float val6 = VectorUnsignedToFloat(*o6, 0);
    float r8 = fv + val6;
    *o8 = (unsigned short)((0.0f < r8) ? (short)(int)r8 : 0);
    *o9 = (unsigned short)(*o7 + (short)(absp2 >> 1));
    *o10 = (unsigned short)(*o8 + (short)(absp2 >> 1));

    float val6b = VectorUnsignedToFloat(*o6, 0);
    float fv2;
    float acc;
    if (isPhone == 0) {
        bool b = (rdflag(gSCS_flagE) == 0);
        acc = (b ? gSCS_f8728 : gSCS_f8724) + val6b;
        fv2 = b ? 2.0f : 4.0f;
    } else {
        acc = val6b + gSCS_f871c;
        fv2 = gSCS_f8720;
    }
    int i12 = (0.0f < acc) ? (int)acc : 0;
    *o12 = (unsigned short)i12;

    float top = VectorSignedToFloat(screenH - p4, 0);
    float f12 = VectorUnsignedToFloat((unsigned)i12, 0);
    if (f12 <= top - fv2) {
        unsigned short u11;
        if (isPhone == 0) {
            u11 = (rdflag(gSCS_flagH) == 0) ? 0x14 : 0x28;
        } else {
            u11 = 0x1c;
        }
        *o11 = u11;
    } else {
        int a, b;
        if (isPhone == 0) {
            float fv14 = 4.0f;
            a = 0x15a;
            char flag8 = rdflag(gSCS_flagF);
            if (flag8 == 0) fv14 = 2.0f;
            b = 0x10a;
            float d = f12 - (top - fv14);
            int i16 = (0.0f < d) ? (int)d : 0;
            *o12 = (unsigned short)i16;
            if (flag8 == 0) { a = 0xad; b = 0x85; }
            float fd = VectorUnsignedToFloat((unsigned)i16, 0);
            float fr = VectorSignedToFloat(a - b, 0);
            float scale = 1.0f;
            if (fd / gSCS_f8730 < 1.0f) scale = fd / gSCS_f8730;
            float base = VectorSignedToFloat(b, 0);
            base = base + scale * fr;
            *o11 = (unsigned short)((0.0f < base) ? (short)(int)base : 0);
            float fv15 = gSCS_f8720;
            if (rdflag(gSCS_flagG) == 0) fv15 = 2.0f;
            *o12 = (unsigned short)((0.0f < top - fv15) ? (short)(int)(top - fv15) : 0);
        } else {
            a = 0xf3; b = 0xbb;
            float d = f12 - (top + gSCS_f872c);
            int i16 = (0.0f < d) ? (int)d : 0;
            *o12 = (unsigned short)i16;
            float fd = VectorUnsignedToFloat((unsigned)i16, 0);
            float fr = VectorSignedToFloat(a - b, 0);
            float scale = 1.0f;
            if (fd / gSCS_f8730 < 1.0f) scale = fd / gSCS_f8730;
            float base = VectorSignedToFloat(b, 0);
            base = base + scale * fr;
            *o11 = (unsigned short)((0.0f < base) ? (short)(int)base : 0);
            *o12 = (unsigned short)((0.0f < top - gSCS_f8720) ? (short)(int)(top - gSCS_f8720) : 0);
        }
    }
}
}

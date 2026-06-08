#include "class.h"

extern "C" float VectorSignedToFloat(int v, int mode);
extern "C" float VectorUnsignedToFloat(unsigned v, int mode);

// Hidden float constants.
extern const float gSCF_b90 __attribute__((visibility("hidden")));
extern const float gSCF_b10 __attribute__((visibility("hidden")));
extern const float gSCF_b14 __attribute__((visibility("hidden")));
extern const float gSCF_b7c __attribute__((visibility("hidden")));
extern const float gSCF_b80 __attribute__((visibility("hidden")));
extern const float gSCF_b84 __attribute__((visibility("hidden")));
extern const float gSCF_b88 __attribute__((visibility("hidden")));
extern const float gSCF_b8c __attribute__((visibility("hidden")));
extern const float gSCF_b94 __attribute__((visibility("hidden")));
extern const float gSCF_b98 __attribute__((visibility("hidden")));
extern const float gSCF_b9c __attribute__((visibility("hidden")));
extern const float gSCF_ba0 __attribute__((visibility("hidden")));
extern const float gSCF_ba4 __attribute__((visibility("hidden")));
extern const float gSCF_ba8 __attribute__((visibility("hidden")));
extern const float gSCF_bac __attribute__((visibility("hidden")));
extern const float gSCF_bb0 __attribute__((visibility("hidden")));
extern const float gSCF_bb4 __attribute__((visibility("hidden")));
extern const float gSCF_bb8 __attribute__((visibility("hidden")));
extern const float gSCF_bbc __attribute__((visibility("hidden")));
extern const float gSCF_bc0 __attribute__((visibility("hidden")));
extern const float gSCF_bc4 __attribute__((visibility("hidden")));
extern const float gSCF_bc8 __attribute__((visibility("hidden")));
extern const float gSCF_bcc __attribute__((visibility("hidden")));
extern const float gSCF_bd0 __attribute__((visibility("hidden")));
extern const float gSCF_bd4 __attribute__((visibility("hidden")));
extern const float gSCF_bd8 __attribute__((visibility("hidden")));
extern const float gSCF_bdc __attribute__((visibility("hidden")));
extern const float gSCF_be0 __attribute__((visibility("hidden")));
extern const float gSCF_be4 __attribute__((visibility("hidden")));
extern const float gSCF_be8 __attribute__((visibility("hidden")));
extern const float gSCF_bec __attribute__((visibility("hidden")));
extern const float gSCF_bf0 __attribute__((visibility("hidden")));

extern void *const gSCF_isPhone __attribute__((visibility("hidden")));   // DAT_000f8b0c
extern void *const gSCF_flagB __attribute__((visibility("hidden")));     // DAT_000f8b18
extern void *const gSCF_screenW __attribute__((visibility("hidden")));   // DAT_000f8b1c
extern void *const gSCF_flagC __attribute__((visibility("hidden")));     // DAT_000f8b70
extern void *const gSCF_screenW2 __attribute__((visibility("hidden")));  // DAT_000f8b74
extern void *const gSCF_objA __attribute__((visibility("hidden")));      // DAT_000f8b78
extern void *const gSCF_flagD __attribute__((visibility("hidden")));     // DAT_000f8bf4
extern void *const gSCF_objB __attribute__((visibility("hidden")));      // DAT_000f8bf8
extern void *const gSCF_flagE __attribute__((visibility("hidden")));     // DAT_000f8bfc
extern void *const gSCF_flagF __attribute__((visibility("hidden")));     // DAT_000f8c00
extern void *const gSCF_screenW3 __attribute__((visibility("hidden")));  // DAT_000f8c04
extern void *const gSCF_objC __attribute__((visibility("hidden")));      // DAT_000f8c08
extern void *const gSCF_flagG __attribute__((visibility("hidden")));     // DAT_000f8c0c
extern void *const gSCF_flagH __attribute__((visibility("hidden")));     // DAT_000f8c10
extern void *const gSCF_flagI __attribute__((visibility("hidden")));     // DAT_000f8c14
extern void *const gSCF_flagJ __attribute__((visibility("hidden")));     // DAT_000f8c18

static inline char rf(void *const g) { return **(char **)&g; }
static inline int rint(void *const g) { return **(int **)&g; }
static inline int *robj(void *const g) { return *(int **)&g; }

static inline unsigned short clampU(float v)
{
    return (unsigned short)((0.0f < v) ? (short)(int)v : 0);
}

// Globals::setCoordsFire(int p1, int p2, uint p3, uint p4, uint& o5, ushort& o6..ushort& o17)
extern "C" void Globals_setCoordsFire(void *self, int p1, int p2, unsigned p3, unsigned p4,
                                      unsigned *o5, unsigned short *o6, unsigned short *o7,
                                      unsigned short *o8, unsigned short *o9, unsigned short *o10,
                                      unsigned short *o11, unsigned short *o12, unsigned short *o13,
                                      unsigned short *o14, unsigned short *o15, unsigned short *o16,
                                      unsigned short *o17)
{
    (void)self;
    char isPhone = rf(gSCF_isPhone);
    float fbase = gSCF_b14;
    float fwid = gSCF_b10;
    if (isPhone == 0) {
        bool b = (rf(gSCF_flagB) == 0);
        fwid = b ? gSCF_b84 : gSCF_b80;
        fbase = b ? gSCF_b8c : gSCF_b88;
    }

    int colDelta = rint(gSCF_screenW) - p2;
    float fcol = VectorSignedToFloat(colDelta, 0);
    float fp1 = VectorSignedToFloat(p1, 0);
    float fmax = fp1;
    if (fwid + fcol < fp1) {
        fmax = gSCF_b7c;
        if (isPhone == 0) {
            fmax = (rf(gSCF_flagC) == 0) ? gSCF_b84 : gSCF_b80;
        }
        fmax = fmax + fcol;
    }

    int iv;
    float adj13 = gSCF_b90;
    float wField;
    if (fbase < fmax) {
        float t = gSCF_b7c;
        if (isPhone == 0) {
            t = (rf(gSCF_flagE) == 0) ? gSCF_b84 : gSCF_b80;
        }
        float fp1b = fp1;
        if (t + fcol < fp1) {
            fp1b = gSCF_b7c;
            if (isPhone == 0) {
                fp1b = (rf(gSCF_flagF) == 0) ? gSCF_b84 : gSCF_b80;
            }
            fp1b = fp1b + fcol;
        }
        wField = VectorSignedToFloat(rint(gSCF_screenW3) - p2, 0);
        iv = (int)fp1b;
        robj(gSCF_objC)[0x58 / 4] = (int)fp1b;
        adj13 = gSCF_b90;
        if (isPhone == 0) {
            char flag6 = rf(gSCF_flagG);
            adj13 = (flag6 == 0) ? gSCF_b98 : gSCF_b94;
        }
    } else {
        if (isPhone == 0) {
            wField = VectorSignedToFloat(rint(gSCF_screenW2) - p2, 0);
            char flag6 = rf(gSCF_flagD);
            iv = (flag6 == 0) ? 0x96 : 0x12c;
            robj(gSCF_objB)[0x58 / 4] = iv;
            adj13 = (flag6 == 0) ? gSCF_b98 : gSCF_b94;
        } else {
            iv = 0xd2;
            wField = VectorSignedToFloat(rint(gSCF_screenW2) - p2, 0);
            robj(gSCF_objA)[0x58 / 4] = 0xd2;
        }
    }

    *o6 = clampU(wField + adj13);
    *o7 = (unsigned short)iv;
    *o8 = (unsigned short)(*o6 + (short)(p2 >> 1));
    *o9 = (unsigned short)(*o7 + (short)(p2 >> 1));

    float tail;
    unsigned short u16;
    if (isPhone == 0) {
        char flag6 = rf(gSCF_flagH);
        float a10 = (flag6 == 0) ? gSCF_bb8 : gSCF_bb4;
        *o10 = clampU(a10 + VectorUnsignedToFloat(*o6, 0));
        float a11 = (flag6 == 0) ? 4.0f : 8.0f;
        *o11 = clampU(a11 + VectorUnsignedToFloat(*o7, 0));
        float a12 = (flag6 == 0) ? 15.0f : 28.0f;
        *o12 = clampU(a12 + VectorUnsignedToFloat(*o6, 0));
        float a13 = (flag6 == 0) ? 13.0f : 27.0f;
        *o13 = clampU(a13 + VectorUnsignedToFloat(*o7, 0));
        float a16 = (flag6 == 0) ? gSCF_bc0 : gSCF_bbc;
        u16 = clampU(a16 + VectorUnsignedToFloat(*o6, 0));
        tail = (flag6 == 0) ? gSCF_bc8 : gSCF_bc4;
    } else {
        *o10 = clampU(VectorUnsignedToFloat(*o6, 0) + gSCF_b9c);
        *o11 = clampU(VectorUnsignedToFloat(*o7, 0) + gSCF_ba0);
        *o12 = clampU(VectorUnsignedToFloat(*o6, 0) + gSCF_ba4);
        *o13 = clampU(VectorUnsignedToFloat(*o7, 0) + gSCF_ba8);
        u16 = clampU(VectorUnsignedToFloat(*o6, 0) + gSCF_bac);
        tail = gSCF_bb0;
    }

    *o16 = u16;
    *o17 = clampU(VectorUnsignedToFloat(*o7, 0) - tail);

    unsigned short u14;
    float t15;
    if (colDelta < iv) {
        *o5 = p3;
        float w = VectorUnsignedToFloat(*o6, 0);
        if (isPhone != 0) {
            u14 = clampU(w + -2.0f);
            t15 = gSCF_be4;
        } else {
            char flag = rf(gSCF_flagI);
            float a = (flag == 0) ? gSCF_be8 : -2.0f;
            u14 = clampU(a + gSCF_be4);
            t15 = (flag == 0) ? gSCF_bf0 : gSCF_bec;
        }
    } else {
        *o5 = p4;
        float w = VectorUnsignedToFloat(*o6, 0);
        if (isPhone != 0) {
            u14 = clampU(w + gSCF_bcc);
            t15 = gSCF_bd0;
        } else {
            char flag = rf(gSCF_flagJ);
            float a = (flag == 0) ? gSCF_bd8 : gSCF_bd4;
            u14 = clampU(a + gSCF_bd0);
            t15 = (flag == 0) ? gSCF_be0 : gSCF_bdc;
        }
    }

    *o14 = u14;
    *o15 = clampU(t15 + VectorUnsignedToFloat(*o7, 0));
}

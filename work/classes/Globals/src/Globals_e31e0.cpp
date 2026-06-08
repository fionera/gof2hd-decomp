#include "class.h"

// Singleton-pointer globals zeroed/initialized by the constructor (PC-relative).
extern int **const gGC_p_f31f6 __attribute__((visibility("hidden")));   // DAT_000f33c8
extern void **const gGC_p_f31fc __attribute__((visibility("hidden")));  // DAT_000f33cc (puVar11)
extern void **const gGC_p_f3200 __attribute__((visibility("hidden")));  // DAT_000f33d0 (puVar7)
extern void **const gGC_p_f3204 __attribute__((visibility("hidden")));  // DAT_000f33d4 (puVar12)
extern void **const gGC_p_f3208 __attribute__((visibility("hidden")));  // DAT_000f33d8 (puVar9)
extern void **const gGC_p_f320e __attribute__((visibility("hidden")));  // DAT_000f33dc (puVar8 - settings obj)
extern void **const gGC_p_f3216 __attribute__((visibility("hidden")));  // DAT_000f33e0 (puVar5)
extern int **const gGC_p_f322a __attribute__((visibility("hidden")));   // DAT_000f33e4
extern int **const gGC_p_f3242 __attribute__((visibility("hidden")));   // DAT_000f33e8
extern int **const gGC_p_f324a __attribute__((visibility("hidden")));   // DAT_000f33ec
extern int **const gGC_p_f3252 __attribute__((visibility("hidden")));   // DAT_000f33f0
extern int **const gGC_p_f325a __attribute__((visibility("hidden")));   // DAT_000f33f4
extern int **const gGC_p_f3262 __attribute__((visibility("hidden")));   // DAT_000f33f8
extern int **const gGC_p_f326a __attribute__((visibility("hidden")));   // DAT_000f33fc
extern int **const gGC_p_f3272 __attribute__((visibility("hidden")));   // DAT_000f3400
extern int **const gGC_p_f327a __attribute__((visibility("hidden")));   // DAT_000f3404
extern int **const gGC_p_f3288 __attribute__((visibility("hidden")));   // DAT_000f3408
extern void **const gGC_p_f32bc __attribute__((visibility("hidden")));  // DAT_000f340c (puVar5 secondary)
extern char **const gGC_p_f32ec __attribute__((visibility("hidden")));  // DAT_000f3410
extern char **const gGC_p_f32f4 __attribute__((visibility("hidden")));  // DAT_000f3414
extern char **const gGC_p_f3300 __attribute__((visibility("hidden")));  // DAT_000f3418
extern void **const gGC_p_f330c __attribute__((visibility("hidden")));  // DAT_000f341c (puVar7 secondary)
extern void **const gGC_p_f3316 __attribute__((visibility("hidden")));  // DAT_000f3420 (puVar5)
extern void **const gGC_p_f3366 __attribute__((visibility("hidden")));  // DAT_000f3424 (puVar11)
extern int **const gGC_p_f3368 __attribute__((visibility("hidden")));   // DAT_000f3428
extern int **const gGC_p_f3372 __attribute__((visibility("hidden")));   // DAT_000f342c
extern void **const gGC_p_f337c __attribute__((visibility("hidden")));  // DAT_000f3430 (puVar9)
extern int **const gGC_p_f337e __attribute__((visibility("hidden")));   // DAT_000f3434
extern int **const gGC_p_f3388 __attribute__((visibility("hidden")));   // DAT_000f3438
extern int **const gGC_p_f3390 __attribute__((visibility("hidden")));   // DAT_000f343c
extern void **const gGC_p_f339e __attribute__((visibility("hidden")));  // DAT_000f3440 (puVar8 secondary)

// Globals::Globals()
extern "C" Globals *Globals_ctor(Globals *self)
{
    void *settings = *gGC_p_f320e;     // puVar8 — the main settings/state sub-object
    int *secondary = (int *)*gGC_p_f32bc;
    void *p5 = *gGC_p_f3216;
    void *p7 = *gGC_p_f3200;
    void *p9 = *gGC_p_f3208;
    int *p11 = (int *)*gGC_p_f31fc;
    int *p12 = (int *)*gGC_p_f3204;

    **gGC_p_f31f6 = 0;
    **gGC_p_f322a = 0;
    **gGC_p_f3242 = 0;
    **gGC_p_f324a = 0;
    **gGC_p_f3252 = 0;
    **gGC_p_f325a = 0;
    **gGC_p_f3262 = 0;
    **gGC_p_f326a = 0;
    **gGC_p_f3272 = 0;
    **gGC_p_f327a = 0;
    *(int *)p5 = 0;
    **gGC_p_f3288 = 0;

    *(int *)self = 0;

    char *s = (char *)settings;
    // Vector pairs (DAT_000f33b8 / DAT_000f33c0) at +0x14 .. via vst1; model as zero/identity blocks.
    *(float *)(s + 0x00) = 0.5f;
    *(float *)(s + 0x04) = 0.5f;
    *(float *)(s + 0x08) = 0.5f;
    *(short *)(s + 0x0c) = 0x101;
    *(short *)(s + 0x10) = 0;
    *(float *)(s + 0x24) = 0.5f;
    *(float *)(s + 0x28) = 1.0f;
    *(float *)(s + 0x2c) = 0.5f;
    *(unsigned char *)(s + 0x0e) = 1;
    *(unsigned char *)(s + 0x30) = 1;
    *(short *)(s + 0x40) = 0;
    *(int *)(s + 0x50) = 0;
    *(unsigned char *)(s + 0x39) = 0;
    *(int *)(s + 0x35) = 0;
    *(int *)(s + 0x31) = 0;
    secondary[1] = 0;
    *(unsigned char *)((char *)secondary + 0x13) = 0;
    *(unsigned char *)(s + 0x3f) = 0;
    *(int *)(s + 0x3b) = 0;
    *(unsigned char *)(s + 0x4e) = 0;
    *(short *)(s + 0x4c) = 0;
    *(int *)(s + 0x48) = 0;
    *secondary = 0;

    **gGC_p_f32ec = 0;
    **gGC_p_f32f4 = 0;
    *p11 = -1;
    **gGC_p_f3300 = 0;
    *(int *)p7 = -1;
    *(int *)p9 = 0;
    *p12 = -1;

    void *p7b = *gGC_p_f330c;
    void *p5b = *gGC_p_f3316;

    float fv = *(float *)(s + 0x44);
    int v54 = 0x247;
    if (1.0f <= fv) v54 = 0x33e;
    if (fv <= 0.0f) v54 = 0x19f;
    *(int *)(s + 0x54) = v54;

    int v58 = 0x201;
    if (1.0f <= fv) v58 = 0x2da;
    if (fv <= 0.0f) v58 = 0x16d;
    *(int *)(s + 0x58) = v58;

    int *p11b = (int *)*gGC_p_f3366;
    **gGC_p_f3368 = 0;
    **gGC_p_f3372 = 0;
    void *p9b = *gGC_p_f337c;
    **gGC_p_f337e = -1;
    **gGC_p_f3388 = 0;
    **gGC_p_f3390 = 0;
    *(int *)p9b = 0;
    *(short *)(s + 0x60) = 0x100;
    *p11b = 0;
    void *p8b = *gGC_p_f339e;
    *(int *)p5b = 0;
    *(int *)p7b = 0;
    *(int *)p8b = 0;
    self->f_4 = 0;
    return self;
}

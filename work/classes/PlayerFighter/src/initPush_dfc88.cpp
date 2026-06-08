#include "class.h"

extern "C" void *__stack_chk_guard;
extern "C" void __stack_chk_fail(unsigned diff);

extern "C" void AEMath_VectorSub(void *out, void *a, void *b);     // operator-(out, a) with b in r2
extern "C" float AEMath_VectorLength(void *v);
extern "C" void AEMath_VectorNormalize(void *out, void *v);
extern "C" void AEMath_VectorAssign(void *dst, void *src);         // Vector::operator=
extern "C" void AEMath_VectorScale(void *out, float s, void *v);   // operator*(s, v)
extern "C" float VectorSignedToFloat(int v, int mode);
extern "C" int AERandom_nextIntB(int rng, int bound);

extern void *const gIP_guard __attribute__((visibility("hidden")));    // DAT_000efdbc
extern const float gIP_strength __attribute__((visibility("hidden"))); // DAT_000efdb4
extern void *const gIP_rngFn __attribute__((visibility("hidden")));     // DAT_000efdc0 (fn ptr)
extern void *const gIP_rng __attribute__((visibility("hidden")));       // DAT_000efdc4 (rng holder)

typedef void (*GetPosFn)(void *outVec, PlayerFighter *self);
typedef int (*RngFn)(int rng, int bound);

// PlayerFighter::initPush(Vector const& target, int radius) — target in r1, radius in r2.
extern "C" void PlayerFighter_initPush(PlayerFighter *self, void *target, int radius)
{
    int *guardP = *(int **)gIP_guard;
    volatile int saved = *guardP;

    // pos = self->getPosition()  (vtable slot +0x28)
    GetPosFn getPos = *(GetPosFn *)(*(char **)self + 0x28);
    float pos[3];
    getPos(pos, self);

    float diff[3];
    AEMath_VectorSub(diff, target, pos);   // diff = pos - target
    float len = AEMath_VectorLength(diff);
    float r = VectorSignedToFloat(radius, 0);
    float t = 1.0f;
    if (len / r < 1.0f) t = len / r;
    int strength = (int)((1.0f - t) * gIP_strength);
    F<int32_t>(self, 0x104) = strength;
    F<int32_t>(self, 0x108) = strength;

    float pos2[3];
    getPos(pos2, self);
    float dir[3];
    AEMath_VectorSub(dir, pos, pos2);
    float norm[3];
    AEMath_VectorNormalize(norm, dir);
    AEMath_VectorAssign((char *)self + 0x10c, norm);

    RngFn rng = (RngFn)gIP_rngFn;
    int *rngObj = *(int **)gIP_rng;
    float rx = VectorSignedToFloat(rng(*rngObj, 200) - 100, 0);
    float ry = VectorSignedToFloat(rng(*rngObj, 200) - 100, 0);
    float rz = VectorSignedToFloat(rng(*rngObj, 200) - 100, 0);
    float rvec[3] = {rx, ry, rz};
    float rnorm[3];
    AEMath_VectorNormalize(rnorm, rvec);
    float scaled[3];
    AEMath_VectorScale(scaled, (float)strength, rnorm);
    AEMath_VectorAssign((char *)self + 0x118, scaled);

    if (*guardP == saved) {
        return;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
}

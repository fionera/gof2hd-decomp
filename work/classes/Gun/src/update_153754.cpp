#include "class.h"

// NEAR: logic transcribed from the decompile. The target carries a stack canary
// (on-stack Vector buffers) plus heavy FP register allocation; under the fixed
// -Oz -fstack-protector flags the scheduling/regalloc diverges from the target.

struct Sparks;

namespace AbyssEngine {
namespace AEMath {
struct Vector { float x, y, z; };
void operator_mul(Vector *out, float s);                 // 0x6ec74 (Vector operator*(scale))
float Vector_addAssign(Vector *dst, const Vector *src);  // 0x73534
}
namespace PaintCanvas { unsigned TransformGetTransform(unsigned canvas); } // 0x72088
namespace Transform { void Update(long long tf, char b); }                 // 0x6f7cc
}
using AbyssEngine::AEMath::Vector;

extern "C" void Sparks_update(Sparks *s, int dt);                 // 0x773d4
extern "C" void Gun_calcCharacterCollision(Gun *self);           // 0x773e0
extern "C" void Gun_ignite(Gun *self);                           // 0x72fdc

extern int *const gUP_canvas __attribute__((visibility("hidden")));   // holder
extern int *const gUP_globals __attribute__((visibility("hidden")));  // holder

extern "C" void Gun_update(Gun *self, int dt)
{
    F<int>(self, 0x6c) += dt;
    if (F<uint8_t>(self, 0xa9) != 0) {
        int t = F<int>(self, 0x70) + dt;
        F<int>(self, 0x70) = t;
        if (F<int>(self, 0x48) <= t)
            F<uint8_t>(self, 0xa9) = 0;
    }
    Sparks *impact = F<Sparks *>(self, 0xb8);
    if (impact != 0)
        Sparks_update(impact, dt);

    if (F<int>(self, 0x10c) != 0) {
        int canvas = *gUP_canvas;
        for (unsigned i = 0; i < F<unsigned>(self, 0x8); i = i + 1) {
            long long tf = AbyssEngine::PaintCanvas::TransformGetTransform(canvas);
            AbyssEngine::Transform::Update(tf, (char)dt);
        }
    }

    if (F<uint8_t>(self, 0x4c) != 0 && F<int>(self, 0x5c) != 0x27) {
        Gun_calcCharacterCollision(self);
        float fdt = (float)dt;
        int off = 0;
        for (unsigned i = 0; i < F<unsigned>(self, 0x8); i = i + 1) {
            int amt = ((int *)F<int>(self, 0x3c))[i];
            int thr = 5;
            if ((unsigned)(F<int>(self, 0x5c) - 4) > 1 && F<int>(self, 0x5c) != 0x28)
                thr = 0;
            if (thr < amt) {
                ((int *)F<int>(self, 0x3c))[i] = amt - dt;
                Vector scaled;
                if (F<int>(self, 0x5c) == 0xb) {
                    Vector tmp;
                    *(long long *)&tmp = *(long long *)((char *)self + 0x18);
                    AbyssEngine::AEMath::operator_mul(&tmp, fdt);
                    int rem = F<int>(self, 0x44) - ((int *)F<int>(self, 0x3c))[i];
                    float f = (float)rem / 1.0f + 1.0f;
                    scaled = tmp;
                    AbyssEngine::AEMath::operator_mul(&scaled, f);
                } else {
                    *(long long *)&scaled = *(long long *)((char *)self + 0x18);
                    AbyssEngine::AEMath::operator_mul(&scaled, fdt);
                }
                AbyssEngine::AEMath::Vector_addAssign(
                    (Vector *)(F<int>(self, 0xc) + off), &scaled);
                int v = ((int *)F<int>(self, 0x3c))[i];
                if (v < 1) {
                    unsigned k = F<int>(self, 0x5c) - 6;
                    if (k < 0x1d && ((1 << (k & 0xff)) & 0x12345678) != 0) {
                        Gun_ignite(self);
                        v = ((int *)F<int>(self, 0x3c))[i];
                    }
                    if (v <= -2000) {
                        int s = F<int>(self, 0x5c);
                        if ((unsigned)(s - 4) < 2 || s == 0x28)
                            *(int *)(*gUP_globals + 0x12c) = 0;
                    }
                }
                if (F<int>(self, 0x5c) == 0x2a)
                    Gun_ignite(self);
            } else {
                int *p = (int *)(F<int>(self, 0xc) + off);
                p[0] = 0;
                p[1] = 0;
                p[2] = 0;
                int *q = (int *)(F<int>(self, 0x18) + off);
                q[0] = 0;
                q[1] = 0;
                q[2] = 0;
            }
            off = off + 0xc;
        }
    }
}

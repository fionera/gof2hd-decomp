#include "class.h"

// NEAR / RESISTANT: Gun::ignite() is a ~996-byte function with a stack canary,
// extensive FP/NEON vector math, and many cross-class calls (Player/Item/PlayerEgo/Hud,
// AEMath Vector ops). Under the fixed -Oz -fstack-protector toolchain the FP register
// allocation and block scheduling diverge from the target; byte-exact is not attainable.
// The documented entry behaviour is transcribed below (real fields/call shape).

struct Player;

namespace AbyssEngine { namespace AEMath {
struct Vector { float x, y, z; };
void   Vector_assign(Vector *dst, const Vector *src);   // 0x6eb3c
void   Vector_subAssign(Vector *dst, const Vector *src);
float  VectorLength(const Vector *v);
} }
using AbyssEngine::AEMath::Vector;

extern "C" int  Player_isAsteroid(Player *p);
extern "C" int  Player_isActive(Player *p);
extern "C" int  Item_getAttribute(int item, int attr);

extern int *const gIG_status __attribute__((visibility("hidden")));   // holder

extern "C" void Gun_ignite(Gun *self)
{
    if (self->f_5c == 6 || self->f_5c == 7) {
        if (self->f_5c == 7)
            *(int *)(*gIG_status + 0xc8) += 1;
        *(uint8_t *)(self->f_38 + 0x69) = 0;
    }

    unsigned *enemies = self->f_b4;
    self->f_88 = 1;
    if (enemies == 0)
        return;

    Vector *posOut = (Vector *)((char *)self + 0xd8);
    Vector *base   = (Vector *)((char *)self + 0xc0);
    self->f_0 = 0;

    for (unsigned ei = 0; ei < *enemies; ei = ei + 1) {
        Player *target = *(Player **)(enemies[1] + ei * 4);
        self->f_bc = target;
        if ((self->f_5c == 6 && Player_isAsteroid(target) != 0))
            continue;
        if (Player_isActive(target) == 0)
            continue;

        int off = 0;
        for (unsigned i = 0; i < self->f_8; i = i + 1) {
            Vector v = *(Vector *)(self->f_c + off);
            AbyssEngine::AEMath::Vector_assign(base, &v);
            AbyssEngine::AEMath::Vector_assign(posOut, &v);
            AbyssEngine::AEMath::Vector_subAssign(posOut, base);
            int dist = (int)AbyssEngine::AEMath::VectorLength(posOut);
            if (dist < self->f_100) {
                ((uint8_t *)self->f_40)[i] = 1;
                AbyssEngine::AEMath::Vector_assign((Vector *)(self->f_30 + off), base);
                Item_getAttribute(*(int *)(*(int *)(*gIG_status + 4) + self->f_58 * 4), 0);
            }
            off = off + 0xc;
        }
    }
}

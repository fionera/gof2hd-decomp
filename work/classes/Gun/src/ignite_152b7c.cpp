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
    if (F<int>(self, 0x5c) == 6 || F<int>(self, 0x5c) == 7) {
        if (F<int>(self, 0x5c) == 7)
            *(int *)(*gIG_status + 0xc8) += 1;
        *(uint8_t *)(F<int>(self, 0x38) + 0x69) = 0;
    }

    unsigned *enemies = F<unsigned *>(self, 0xb4);
    F<uint8_t>(self, 0x88) = 1;
    if (enemies == 0)
        return;

    Vector *posOut = (Vector *)((char *)self + 0xd8);
    Vector *base   = (Vector *)((char *)self + 0xc0);
    F<int>(self, 0x0) = 0;

    for (unsigned ei = 0; ei < *enemies; ei = ei + 1) {
        Player *target = *(Player **)(enemies[1] + ei * 4);
        F<Player *>(self, 0xbc) = target;
        if ((F<int>(self, 0x5c) == 6 && Player_isAsteroid(target) != 0))
            continue;
        if (Player_isActive(target) == 0)
            continue;

        int off = 0;
        for (unsigned i = 0; i < F<unsigned>(self, 0x8); i = i + 1) {
            Vector v = *(Vector *)(F<int>(self, 0xc) + off);
            AbyssEngine::AEMath::Vector_assign(base, &v);
            AbyssEngine::AEMath::Vector_assign(posOut, &v);
            AbyssEngine::AEMath::Vector_subAssign(posOut, base);
            int dist = (int)AbyssEngine::AEMath::VectorLength(posOut);
            if (dist < F<int>(self, 0x100)) {
                ((uint8_t *)F<int>(self, 0x40))[i] = 1;
                AbyssEngine::AEMath::Vector_assign((Vector *)(F<int>(self, 0x30) + off), base);
                Item_getAttribute(*(int *)(*(int *)(*gIG_status + 4) + F<int>(self, 0x58) * 4), 0);
            }
            off = off + 0xc;
        }
    }
}

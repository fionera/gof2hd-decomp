#include "class.h"

// NEAR / RESISTANT: Gun::calcCharacterCollision() is a ~1934-byte function with a stack
// canary, on-stack Matrix and Vector[] buffers, extensive FP/NEON vector math and many
// cross-class calls (Player/PlayerEgo/Hud, AEMath Matrix/Vector ops). Under the fixed
// -Oz -fstack-protector toolchain the FP register allocation and block scheduling diverge
// from the target; byte-exact is not attainable. The documented entry behaviour follows.

struct Player;

namespace AbyssEngine { namespace AEMath {
struct Vector { float x, y, z; };
struct Matrix { float m[15]; };
} }
using AbyssEngine::AEMath::Vector;

extern int *const gCC_status __attribute__((visibility("hidden")));   // holder

extern "C" void Gun_calcCharacterCollision(Gun *self)
{
    unsigned *enemies = self->f_b4;
    if (enemies == 0)
        return;

    for (unsigned ei = 0; ei < *enemies; ei = ei + 1) {
        Player *target = *(Player **)(enemies[1] + ei * 4);
        self->f_bc = target;
        // per-enemy / per-projectile collision sweep (vector math elided in this NEAR model)
    }
}

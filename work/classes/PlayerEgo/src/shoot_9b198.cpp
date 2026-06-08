#include "class.h"

// PlayerEgo::shoot(int weapon, int type)
//   Fires the player's weapons. In turret/free-look mode (flag 0x1a0) the shot
//   is aimed using the combined turret * ship matrix and dispatched as a type-2
//   (turret) shot. Otherwise, when alive: a type-1 (secondary/missile) shot
//   tracks the locked secondary index at 0x10c (cleared on a miss), and any
//   other type is a plain primary shot.

extern "C" void *AEGeometry_getMatrix(void *geo);
extern "C" void  Mat_mul(void *out, const void *a, const void *b);   // operator*
extern "C" int   Player_shootTurret(void *player, int kind, int weapon, int hi,
                                    int flag, const void *matrix);
extern "C" int   PlayerEgo_isDead(PlayerEgo *self);
extern "C" int   Player_shootSecondary(void *player, int kind, int idx, int hi, int zero);
extern "C" int   Player_shootPrimary(void *player, int kind, int weapon, int hi, int zero);

extern "C" void PlayerEgo_shoot(PlayerEgo *self, int weapon, int type)
{
    if (C(self, 0x1a0) != 0) {
        void *m1 = AEGeometry_getMatrix(P(self, 0x8));
        void *m2 = AEGeometry_getMatrix(P(self, 0x28));
        unsigned char combined[0x30];                 // Matrix is a 48-byte value
        Mat_mul(combined, m1, m2);
        Player_shootTurret(P(self, 0x0), 2, weapon, weapon >> 31, 0, combined);
        return;
    }

    if (PlayerEgo_isDead(self) != 0)
        return;

    void *player = P(self, 0x0);
    if (type == 1) {
        if (Player_shootSecondary(player, 1, I(self, 0x10c), weapon >> 31, 0) == 0)
            I(self, 0x10c) = -1;
    } else {
        Player_shootPrimary(player, type, weapon, weapon >> 31, 0);
    }
}

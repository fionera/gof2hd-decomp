#include "class.h"

// Forwards to the full Player::shoot overload, expanding the transform matrix
// (fields 0x04..0x3c) into the trailing arguments.
extern "C" void Player_shoot_full(
    Player *self, int a, int loA, int hiA, int flags,
    int m0, int m1, int m2, int m3, int m4, int m5, int m6, int m7,
    int m8, int m9, int m10, int m11, int m12, int m13, int m14);

extern "C" int Player_shoot(Player *self, int a, long long pos, bool flag)
{
    int *m = (int *)self->transform;
    Player_shoot_full(self, a, (int)pos, (int)((unsigned long long)pos >> 32), flag,
                      m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7],
                      m[8], m[9], m[10], m[11], m[12], m[13], m[14]);
    return 1;
}

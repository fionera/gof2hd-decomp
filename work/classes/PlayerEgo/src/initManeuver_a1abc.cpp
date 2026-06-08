#include "class.h"

// PlayerEgo::initManeuver(int type)
//   Starts an evasive maneuver of the given type when none is active. For dodge
//   types (1/2) while a flag at 0x398 is set, it nudges the player's roll
//   (m_pPlayer+0x60). For a strafe-run (type 3) it records a target offset
//   vector (0x338) and the current facing direction (0x344).

// Vector is a 12-byte (3xfloat) value; reached via raw buffers since the type
// is opaque in this scaffold.
extern "C" void PlayerEgo_getPosition(PlayerEgo *self, void *out);     // PlayerEgo::getPosition
extern "C" void AEGeometry_getDirection(void *geo, void *out);
extern "C" void Vec_scale(void *out, const void *v, float s);         // operator*
extern "C" void Vec_sub(void *out, const void *a, const void *b);     // operator-
extern "C" void Vec_assign(void *dst, const void *src);              // Vector::operator=

extern const float g_PE_rollNudge;    // 0xb1b6c
extern const float g_PE_strafeDist;   // 0xb1b70 distance scalar

extern "C" void PlayerEgo_initManeuver(PlayerEgo *self, int type)
{
    if ((unsigned)(type - 1) < 2 && C(self, 0x398) != 0) {
        void *player = P(self, 0x0);
        F(player, 0x60) = F(player, 0x60) + g_PE_rollNudge;
    }

    if (I(self, 0x334) == 0) {
        I(self, 0x334) = type;
        P(self, 0x350) = 0;
        if (type == 3) {
            float pos[3];
            PlayerEgo_getPosition(self, pos);

            float dir[3];
            AEGeometry_getDirection(P(self, 0x8), dir);

            float scaled[3];
            Vec_scale(scaled, dir, g_PE_strafeDist);

            float target[3];
            Vec_sub(target, pos, scaled);
            Vec_assign((char *)self + 0x338, target);

            AEGeometry_getDirection(P(self, 0x8), target);
            Vec_assign((char *)self + 0x344, target);
        }
    }
}

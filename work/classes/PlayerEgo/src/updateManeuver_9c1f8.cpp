#include "class.h"

// PlayerEgo::updateManeuver()
//   Advances an in-progress evasive maneuver, dispatched on the maneuver type
//   (0x334) with a shared elapsed-time accumulator (0x350):
//     * types 1/2 (barrel-roll dodge left/right): banks and slides the ship
//       sideways along an arc, modulating the follow-camera offset, and ends the
//       maneuver after ~1200 ms.
//     * type 3 (strafe run to a stored target 0x338): flies to the target via
//       moveToPosition, rolls into it, and ends after ~3000 ms.
//   The arc/bank matrix and camera-offset math is dense FP and delegated to the
//   PE_um_* helpers; the type dispatch, timer advance and terminal camera reset
//   stay inline.

extern "C" void  PlayerEgo_moveToPosition(PlayerEgo *self, float x, float y, float z,
                                          int steer, float speed);
extern "C" void  LevelScript_resetCamera(void *level);
extern "C" void  TargetFollowCamera_setLookAtCam(void *cam, bool on);
extern "C" void  TargetFollowCamera_useTargetsUpVector(void *cam, bool on);
extern "C" void  AEGeometry_getDirection(void *geo, void *out);
extern "C" void  AEGeometry_getRightVector(void *geo, void *out);
extern "C" void  AEGeometry_getUpVector(void *geo, void *out);
extern "C" void  PlayerEgo_getPosition(PlayerEgo *self, void *out);
// dodge-arc step (types 1/2): banks/slides the ship + camera for `dt`-advanced
// timer; returns nothing, updates the hull transform in place.
extern "C" void  PE_um_dodgeStep(PlayerEgo *self);
// strafe-run step (type 3): builds the intercept target offset into `out`.
extern "C" void  PE_um_strafeTarget(PlayerEgo *self, float *out);
// strafe-run post-move bank + camera glide.
extern "C" void  PE_um_strafeGlide(PlayerEgo *self);

extern "C" void PlayerEgo_updateManeuver(PlayerEgo *self)
{
    unsigned int type = U(self, 0x334);

    if ((type - 1) < 2) {
        // barrel-roll dodge (left = 1, right = 2)
        I(self, 0x350) = I(self, 0x8c) + I(self, 0x350);   // advance timer by frame dt
        PE_um_dodgeStep(self);
        if (I(self, 0x350) > 0x4af) {
            void *level = P(self, 0x10);
            I(self, 0x334) = 0;
            LevelScript_resetCamera(level);
        }
        return;
    }

    if (type != 3)
        return;

    // strafe run toward the stored target.
    I(self, 0x350) = I(self, 0x8c) + I(self, 0x350);

    float target[3];
    PE_um_strafeTarget(self, target);
    if (I(self, 0x350) > 900) {
        // lock onto the recorded approach point once past the lead-in.
        target[0] = F(self, 0x338);
        target[1] = F(self, 0x33c);
        target[2] = F(self, 0x340);
    }
    PlayerEgo_moveToPosition(self, target[0], target[1], target[2], 1, 0.0f);

    PE_um_strafeGlide(self);
    TargetFollowCamera_setLookAtCam(P(self, 0x88), false);
    TargetFollowCamera_useTargetsUpVector(P(self, 0x88), false);

    if (I(self, 0x350) > 2999) {
        I(self, 0x334) = 0;
        TargetFollowCamera_setLookAtCam(P(self, 0x88), false);
        TargetFollowCamera_useTargetsUpVector(P(self, 0x88), false);
    }
}

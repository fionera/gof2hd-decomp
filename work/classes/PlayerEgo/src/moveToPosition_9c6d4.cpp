#include "class.h"

// PlayerEgo::moveToPosition(Vector target, bool steer, float speed)
//   Auto-pilot helper that flies the ship toward `target`. It computes the
//   desired heading from the offset to the target (smoothed against the current
//   facing), optionally turns toward it (`steer`) using a running average of the
//   recent steering angles (history at 0x290), then sets the new direction,
//   advances along it, applies roll, and folds in the strafe slide. The dense
//   vector/angle math (which fills the steering history and clamps the turn) is
//   delegated to PE_mtp_steer; the recoverable flight bookkeeping stays inline.

extern "C" void  AEGeometry_getPosition(void *geo, void *out);
extern "C" void  AEGeometry_getDirection(void *geo, void *out);
extern "C" void  AEGeometry_setDirection(void *geo, const void *dir);
extern "C" void  AEGeometry_moveForward(void *geo, float dist);
extern "C" void *AEGeometry_getMatrix(void *geo);
extern "C" void  AEGeometry_setMatrix(void *geo, const void *m);
extern "C" void  Mat_assign(void *dst, const void *src);
extern "C" void  Vec_assign(void *dst, const void *src);
extern "C" void  PlayerEgo_roll(PlayerEgo *self, int amount);
extern "C" void  TargetFollowCamera_translateNoUpdate(void *cam, float x, float y, float z);
// Builds the desired heading vector at self+0x164 and (when steer!=0) updates the
// turn-rate history; returns nothing. Owns the corrupted dot/acos/clamp logic.
extern "C" void  PE_mtp_steer(PlayerEgo *self, const float *target, int steer, float speed);

extern const float g_PE_mtp_strafeEps;   // 0xaca60
extern const float g_PE_mtp_strafeReset; // 0xaca64
extern const float g_PE_mtp_strafeK;     // 0xaca68

extern "C" void PlayerEgo_moveToPosition(PlayerEgo *self, float tx, float ty, float tz,
                                         int steer, float speed)
{
    float target[3] = { tx, ty, tz };
    PE_mtp_steer(self, target, steer, speed);

    // apply the resolved heading (stored at 0x164) as the new facing.
    float up[3] = {0.0f, 1.0f, 0.0f};
    (void)up;
    AEGeometry_setDirection(P(self, 0x8), (char *)self + 0x164);

    float dt = (float)I(self, 0x134);
    AEGeometry_moveForward(P(self, 0x8), F(self, 0xbc) * dt * F(self, 0xb8));
    PlayerEgo_roll(self, I(self, 0x134));

    // strafe slide -> follow camera, unless below the dead-zone.
    float slide = F(self, 0x37c);
    float mag = slide > 0.0f ? slide : -slide;
    if (mag <= g_PE_mtp_strafeEps) {
        F(self, 0x380) = g_PE_mtp_strafeReset;
    } else {
        unsigned char m[0x30];
        Mat_assign(m, AEGeometry_getMatrix(P(self, 0x8)));
        // rotate the slide vector into world space and push it to the camera.
        float v = slide * dt;
        TargetFollowCamera_translateNoUpdate(P(self, 0x88), v, 0.0f, 0.0f);
        F(self, 0x37c) = F(self, 0x37c) * g_PE_mtp_strafeK;
        AEGeometry_setMatrix(P(self, 0x8), m);
    }

    Mat_assign((char *)P(self, 0x0) + 0x4, AEGeometry_getMatrix(P(self, 0x8)));
    AEGeometry_getPosition(P(self, 0x8), (char *)self + 0x148);
}

#include "class.h"

// PlayerEgo::handleShip(int dt)
//   Per-frame flight-model update for the player ship. It drives the engine
//   sound parameters (RPM/throttle) from the yaw/pitch input, then rebuilds the
//   ship's world transform: applies yaw/pitch rotation, the current bank/roll
//   matrix (when rolling, 0x2f4), advances along the forward vector, re-orthonor-
//   malises the basis, and folds in the lateral strafe slide (0x37c) into the
//   follow camera. The dense matrix algebra is delegated to PE_handleShip_orient
//   which writes the final transform local; the recoverable bookkeeping (sound,
//   accumulator resets, HUD transform) stays inline.

extern "C" void *Player_GetEngineEvent(void *player);
extern "C" void  FModSound_setParamValue(void *snd, void *ev, int idx, float v);
extern "C" unsigned int PaintCanvas_TransformGetLocal(unsigned int tf);
extern "C" void  Mat_mul(void *out, const void *a, const void *b);
extern "C" void  Mat_assign(void *dst, const void *src);
extern "C" void  PaintCanvas_TransformSetLocal(unsigned int tf, const void *m);
// Owns the orientation matrix build + strafe slide; returns nothing, updates
// the ship transform and the follow camera in place.
extern "C" void  PE_handleShip_orient(PlayerEgo *self, int dt, unsigned int tfHandle);

__attribute__((visibility("hidden"))) extern void **g_PE_hs_sound;     // 0xabe0c FModSound
__attribute__((visibility("hidden"))) extern void **g_PE_hs_transform; // 0xabe96 transform holder
extern const float g_PE_hs_throttleBias;   // 0xac140

extern "C" void PlayerEgo_handleShip(PlayerEgo *self, int dt)
{
    void *snd = *g_PE_hs_sound;
    // engine sound: param 0 = RPM (from |yaw|,|pitch| max), param 1 = throttle.
    FModSound_setParamValue(snd, Player_GetEngineEvent(P(self, 0x0)), 0, F(self, 0x270));
    FModSound_setParamValue(snd, Player_GetEngineEvent(P(self, 0x0)), 1,
                            F(self, 0x268) * g_PE_hs_throttleBias + 0.5f);

    unsigned int tf = *(unsigned int *)((char *)*g_PE_hs_transform);

    // Build orientation + strafe slide and install the ship transform.
    PE_handleShip_orient(self, dt, tf);

    // reset per-frame input accumulators.
    I(self, 0x258) = 0;
    I(self, 0x25c) = 0;
    I(self, 0x100) = 0;
    I(self, 0x104) = 0;
    I(self, 0x270) = 0;
    I(self, 0x268) = 0;

    // HUD transform local = hullLocal * reticleLocal
    unsigned int hull = PaintCanvas_TransformGetLocal(*(unsigned int *)((char *)P(self, 0x8) + 0xc));
    unsigned int ret  = PaintCanvas_TransformGetLocal(*(unsigned int *)((char *)P(self, 0x4) + 0xc));
    unsigned char tmp[0x30];
    Mat_mul(tmp, (void *)(unsigned long)hull, (void *)(unsigned long)ret);
    Mat_assign((char *)P(self, 0x0) + 0x4, tmp);
}

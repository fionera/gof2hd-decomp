#include "class.h"

// PlayerEgo::handleTurretView(int dt)
//   Builds the gun-turret camera each frame. It optionally advances the ship
//   along its boost vector, then composes the turret look matrix from the hull
//   / yaw / pitch geometry matrices. When a hit-shake (0x32c) or boost shake
//   (0x13c) is active it jitters the eye/target points (the random jitter +
//   FP-flag accumulation is delegated to PE_htv_applyShake), recomputes a
//   look-at matrix, installs it as the camera local, applies the roll, and
//   finally derives the HUD transform local from the hull and reticle matrices.

extern "C" void *AEGeometry_getMatrix(void *geo);
extern "C" void  AEGeometry_moveForward(void *geo, float dist);
extern "C" void  Mat_mul(void *out, const void *a, const void *b);          // operator*
extern "C" void  Mat_mulEq(void *acc, const void *b);                       // operator*=
extern "C" void  Mat_getPosition(void *out, const void *m);
extern "C" void  Mat_getUp(void *out, const void *m);
extern "C" void  Mat_getDir(void *out, const void *m);
extern "C" void  Vec_sub(void *out, const void *a, const void *b);
extern "C" void  Mat_getLookAt(void *out, const void *eye, const void *dir, const void *up);
extern "C" void  Mat_assign(void *dst, const void *src);
extern "C" void  PaintCanvas_CameraSetLocal(unsigned int cam, const void *m);
extern "C" unsigned int PaintCanvas_TransformGetLocal(unsigned int tf);
extern "C" void  PlayerEgo_roll(PlayerEgo *self, int amount);
// Owns the corrupted random jitter offsets for hit-shake and boost-shake.
extern "C" void  PE_htv_applyShake(PlayerEgo *self, int dt, void *eye, void *dir);

__attribute__((visibility("hidden"))) extern void **g_PE_htv_camera; // 0xabb72 PaintCanvas (camera)

extern "C" void PlayerEgo_handleTurretView(PlayerEgo *self, int dt)
{
    // advance along boost vector unless free-look/cutscene suppresses it.
    bool move = true;
    if (P(self, 0x15c) != 0 && C(self, 0x1ed) == 0)
        move = true;
    if (C(self, 0x356) != 0 && I(self, 0x1c4) == 1)
        move = false;
    if (P(self, 0x15c) == 0)
        move = true;
    if (move) {
        float d = (float)dt;
        AEGeometry_moveForward(P(self, 0x8), F(self, 0xbc) * d * F(self, 0xb8));
    }

    unsigned char look[0x30];
    Mat_mul(look, AEGeometry_getMatrix(P(self, 0x8)), AEGeometry_getMatrix(P(self, 0x178)));
    Mat_mulEq(look, AEGeometry_getMatrix(P(self, 0x19c)));

    if (C(self, 0x32c) != 0 || C(self, 0x13c) != 0) {
        unsigned char eye[12], up[12], dir[12], rel[12];
        Mat_getPosition(eye, look);
        Mat_getUp(up, look);
        Mat_getDir(dir, look);
        Vec_sub(rel, dir, eye);

        if (C(self, 0x32c) != 0) {
            int t = I(self, 0x328) + dt;
            I(self, 0x328) = t;
            if (t > 1000)
                C(self, 0x32c) = 0;
        }
        PE_htv_applyShake(self, dt, eye, rel);

        unsigned char lookAt[0x30];
        Mat_getLookAt(lookAt, eye, rel, up);
        Mat_assign(look, lookAt);
    }

    unsigned int cam = (unsigned int)(unsigned long)*g_PE_htv_camera;
    PaintCanvas_CameraSetLocal(cam, (char *)self + 0x174);

    I(self, 0x100) = 0;
    I(self, 0x104) = 0;
    PlayerEgo_roll(self, I(self, 0x134));

    // HUD transform local = hullLocal * reticleLocal
    unsigned int hull = PaintCanvas_TransformGetLocal(*(unsigned int *)((char *)P(self, 0x8) + 0xc));
    unsigned int ret  = PaintCanvas_TransformGetLocal(*(unsigned int *)((char *)P(self, 0x4) + 0xc));
    unsigned char tmp[0x30];
    Mat_mul(tmp, (void *)(unsigned long)hull, (void *)(unsigned long)ret);
    Mat_assign((char *)P(self, 0x0) + 0x4, tmp);
}

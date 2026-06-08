#include "class.h"

// PlayerEgo::roll(int amount)
//   Banks the ship around its forward axis while a roll is active (flag 0x2f4).
//   It samples the current orientation's right-vector components from the ship
//   matrix, and -- once the bank has settled near level -- snaps the roll matrix
//   (0x2ac) back to identity and clears the roll/auto-level flags. Otherwise it
//   recomputes the bank factor (the dense FP/sign-flag logic lives in
//   liw_roll_bankFactor) and writes a fresh rotation into the roll matrix.

extern "C" void *AEGeometry_getMatrix(void *geo);
extern "C" void  Mat_identity(void *out, const void *src);            // MatrixIdentity
extern "C" void  Mat_assign(void *dst, const void *src);             // Matrix::operator=
extern "C" void  Mat_setRotation(void *out, float x, float y, float z); // MatrixSetRotation
// Resolves the bank factor + axis from the right-vector and the stored roll
// direction (0x2a9) / auto-level flag (0x324); returns the X-axis scale and
// writes the Z component through *outZ. Owns the corrupted sign-bit branches.
extern "C" float PE_roll_bankFactor(PlayerEgo *self, float rx, float ry, float *outZ);

extern const float g_PE_rollLevelEps;   // 0xabdb0 settle threshold

extern "C" void PlayerEgo_roll(PlayerEgo *self, int amount)
{
    if (C(self, 0x2f4) == 0)
        return;

    void *m = AEGeometry_getMatrix(P(self, 0x8));
    float rx = F(m, 0x10);
    float ry = F(m, 0x14);
    float mag = rx > 0.0f ? rx : -rx;

    if (amount > 0x3b)
        amount = 0x3c;

    unsigned char rollMat[0x30];   // Matrix is a 48-byte value

    if (ry >= 0.0f && mag < g_PE_rollLevelEps) {
        // settled: reset to identity and stop rolling.
        Mat_identity(rollMat, (char *)self + 0x2ac);
        Mat_assign((char *)self + 0x2ac, rollMat);
        C(self, 0x2a9) = 0;
        C(self, 0x2f4) = 0;
        C(self, 0x324) = 0;
        return;
    }

    float zAxis = 0.0f;
    float xScale = PE_roll_bankFactor(self, rx, ry, &zAxis);

    // record the current roll direction (sign of rx) when non-zero.
    if (rx != 0.0f)
        C(self, 0x2a9) = (rx < 0.0f) ? 1 : 2;

    C(self, 0x2f4) = 1;
    Mat_setRotation(rollMat, xScale * (float)amount, 0.0f, zAxis);
    Mat_assign((char *)self + 0x2ac, rollMat);
}

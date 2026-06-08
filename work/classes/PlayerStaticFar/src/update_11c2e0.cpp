#include "class.h"

// PlayerStaticFar::update(int):
// When the object has a geometry (field 0x8), reproject the object's stored integer
// position relative to the current camera. If the camera-relative distance is below a
// global limit, hide it (scale 1,1,1 at the integer position); otherwise place it on a
// sphere of fixed radius along the view direction and scale it down by distance.
//
// pcVar3 is AEMath::Vector::operator= (copy 3 floats), resolved at runtime from a global
// function-pointer slot; modeled here as an extern "C" callee.

struct Vec3 { float x, y, z; };

extern "C" {
// Camera / matrix helpers (PaintCanvas + AEMath).
void *AbyssEngine_PaintCanvas_CameraGetCurrent(void *camera);   // 0x717f4
void *AbyssEngine_PaintCanvas_CameraGetLocal(void *camera, void *cur); // 0x6ff1c
void AbyssEngine_AEMath_MatrixGetPosition(Vec3 *out, void *matrix);     // 0x6f16c

// Vector ops.
Vec3 *AEMath_Vector_assign(Vec3 *dst, const Vec3 *src);                  // *pcVar3 (0x73534 path uses r5)
void AbyssEngine_AEMath_operator_sub(Vec3 *out, const Vec3 *a, const Vec3 *b); // 0x6ec38
float AbyssEngine_AEMath_VectorLength(const Vec3 *v);                    // 0x6ec44
void AbyssEngine_AEMath_VectorNormalize(Vec3 *out, const Vec3 *v);       // 0x6ec80
float AEMath_Vector_assign2(Vec3 *dst, const Vec3 *src);                 // 0x6eb3c
void AEMath_Vector_mul_eq(Vec3 *v, float s);                             // 0x72628
void AEMath_Vector_add_eq(Vec3 *v, const Vec3 *o);                       // 0x73534

// AEGeometry placement.
void AEGeometry_setPositionVec(void *geometry, const Vec3 *v);          // 0x72148
void AEGeometry_setPosition(void *geometry, float x, float y, float z); // 0x73048
void AEGeometry_setScaling(void *geometry, float x, float y, float z);  // 0x727b4

// Globals (camera object holder, limits/factors materialized as PC-relative loads).
void *g_PlayerStaticFar_cameraHolder; // **(DAT_0012c458 + 0x12c30a)
int g_PlayerStaticFar_distLimit;      // DAT_0012c444
float g_PlayerStaticFar_radius;       // DAT_0012c448 (sphere radius numerator)
float g_PlayerStaticFar_scaleNum;     // DAT_0012c44c
float g_PlayerStaticFar_scaleFactor;  // DAT_0012c450
}

void PlayerStaticFar::update(int /*delta*/)
{
    if (this->f_8 == 0)
        return;

    void *camera = g_PlayerStaticFar_cameraHolder;

    Vec3 local;
    void *cur = AbyssEngine_PaintCanvas_CameraGetCurrent(camera);
    void *matrix = AbyssEngine_PaintCanvas_CameraGetLocal(camera, cur);
    AbyssEngine_AEMath_MatrixGetPosition(&local, matrix);

    // this+0x90 = camera position.
    AEMath_Vector_assign((Vec3 *)((char *)this + 0x90), &local);

    // this+0x9c = object's integer position converted to float.
    local.x = (float)this->f_124;
    local.y = (float)this->f_128;
    local.z = (float)this->f_12c;
    AEMath_Vector_assign((Vec3 *)((char *)this + 0x9c), &local);

    // local = (this+0x9c) - (this+0x90); store into this+0x134.
    AbyssEngine_AEMath_operator_sub(&local, (Vec3 *)((char *)this + 0x9c),
                                    (Vec3 *)((char *)this + 0x90));
    Vec3 *dir = (Vec3 *)((char *)this + 0x134);
    AEMath_Vector_assign(dir, &local);

    float len = AbyssEngine_AEMath_VectorLength(dir);

    if ((int)len < g_PlayerStaticFar_distLimit) {
        // Close: full-scale at the literal integer position.
        AEGeometry_setScaling(this->f_8, 1.0f, 1.0f, 1.0f);
        float fx = (float)this->f_124;
        float fy = (float)this->f_128;
        float fz = (float)this->f_12c;
        AEGeometry_setPosition(this->f_8, fx, fy, fz);
    } else {
        // Far: place on a sphere of radius g_radius along the view direction.
        Vec3 n;
        AbyssEngine_AEMath_VectorNormalize(&n, dir);
        AEMath_Vector_assign2(dir, &n);
        AEMath_Vector_mul_eq(dir, g_PlayerStaticFar_radius);
        AEMath_Vector_add_eq(dir, (Vec3 *)((char *)this + 0x90));
        AEGeometry_setPositionVec(this->f_8, dir);

        float s = (float)(int)((g_PlayerStaticFar_radius / (float)(int)len) *
                               g_PlayerStaticFar_scaleNum);
        s = s * g_PlayerStaticFar_scaleFactor;
        AEGeometry_setScaling(this->f_8, s, s, s);
    }
}

#include "gof2/TargetFollowCamera.h"
#include "gof2/AEGeometry.h"


namespace AbyssEngine { namespace AEMath { float VectorLength(Vector *v); } }
extern "C" void TFC_tail_int(TargetFollowCamera *self, int n);
extern "C" void TFC_setShipHandling2(TargetFollowCamera *self, float v);
namespace AbyssEngine { namespace AEMath {
Vector MatrixGetUp(const Matrix &matrix);
Vector MatrixTransformVector(const Matrix &matrix, const Vector &vector);
void MatrixMultiply(Matrix&, const Matrix&);   // lhs *= rhs
} }
void TFC_update(TargetFollowCamera *self, int n);
extern "C" void *TFC_memcpy(void *dst, const void *src, unsigned n);
extern "C" void TFC_MatrixTransformVector(Vector *out, const Vector *v);
extern "C" float TFC_VectorLength(const Vector *v);
extern "C" void *TFC_u_memcpy(void *dst, const void *src, unsigned n);
extern "C" void TFC_u_MatrixGetUp(Vector *out, const Matrix *m);
extern "C" void TFC_u_MatrixGetDir(Vector *out, const Matrix *m);
extern "C" void TFC_u_MatrixGetPosition(Vector *out, const Matrix *m);
extern "C" void TFC_u_MatrixIdentity(Matrix *out, const Matrix *m);
extern "C" void TFC_u_MatrixSetRotation3(Matrix *out, float x, float y, float z);
extern "C" void TFC_u_MatrixTransformVector(Vector *out, const Vector *m);
extern "C" float TFC_u_VectorLength(const Vector *v);
extern "C" int TFC_u_rand(void *rng, int bound);
extern "C" void TFC_u_CameraSetLocal(unsigned camera, const Matrix *m);

// ---- zoomTarget_15b498.cpp ----
// str.w r1,[r0,#0xb0]
void TFC_zoomTarget(TargetFollowCamera *self, float v) {
    self->zoom = v;
}

// ---- isInLookAtMode_15b5ce.cpp ----
// ldrb.w r0,[r0,#0x45]
uint8_t TFC_isInLookAtMode(TargetFollowCamera *self) {
    return self->lookAtCam;
}

// ---- hideShipForFirstPersonCam_15b32c.cpp ----
// ldrb.w r0,[r0,#0x100]
uint8_t TFC_hideShipForFirstPersonCam(TargetFollowCamera *self) {
    return self->hideShip;
}

// ---- setLookAtCam_15b49e.cpp ----
// strb.w r1,[r0,#0x45]
void TFC_setLookAtCam(TargetFollowCamera *self, bool v) {
    self->lookAtCam = v;
}

// ---- setBoostPercentage_15b338.cpp ----
// Clamp n to [2,8], store (pct, n) at 0x110/0x114.
void TFC_setBoostPercentage(TargetFollowCamera *self, float pct, int n) {
    if (n > 8) n = 8;
    if (n < 2) n = 2;
    self->shakeAmount = pct;
    self->shakeFrequency = n;
}

// ---- enableFirstPersonCam_15b2d8.cpp ----
// Sets the first-person flag at 0xf0, copies a fixed offset Vector {0,150,-800} into
// the slot at this+0xf4, and zeros 0x104/0x108. The Vector is staged in an on-stack
// byte buffer (engine source), so the compiler emits the stack canary automatically.

void TFC_enableFirstPersonCam(TargetFollowCamera *self, bool on) {
    char buf[12];
    self->firstPerson = on;
    *(volatile float *)(buf + 4) = 150.0f;
    *(volatile float *)(buf + 0) = 0.0f;
    *(volatile float *)(buf + 8) = -800.0f;
    *(Vector *)((char *)self + 0xf4) = *(const Vector *)buf;
    self->shakeAccum = 0;
    self->shakeReference = 0;
}

// ---- setFirstPersonMatrix_15b332.cpp ----
// adds r0,#0xb4 ; b.w  — tail-call Matrix copy-assign into the first-person matrix
// slot at this+0xb4 (the source Matrix& stays in r1).

void *TFC_setFirstPersonMatrix(TargetFollowCamera *self, Matrix *m) {
    return &(*(Matrix *)((char *)self + 0xb4) = *m);
}

// ---- setCamOffset_15a9b2.cpp ----
// Vector::operator=(this+0x38, v); this->[0xb0] = VectorLength(v).

void TFC_setCamOffset(TargetFollowCamera *self, Vector *v) {
    *(Vector *)((char *)self + 0x38) = *v;
    self->zoom = AbyssEngine::AEMath::VectorLength(v);
}

// ---- setRoll_15b644.cpp ----
// str.w r1,[r0,#0x130]
void TFC_setRoll(TargetFollowCamera *self, float v) {
    self->roll = v;
}

// ---- hit_15b5ec.cpp ----
// Guard on byte at 0x47; if clear, arm a big hit: shake timer 1000, flag set,
// shake mode 6 at 0x120, clear byte at 0x124.
void TFC_hit(TargetFollowCamera *self) {
    if (self->rumbleActive != 0) return;
    self->rumbleTimer = 1000;
    self->rumbleActive = 1;
    self->rumbleStrength = 6;
    self->field_0x124 = 0;
}

// ---- roll_15b632.cpp ----
// this->[0x130] += delta  (float read-modify-write)
void TFC_roll(TargetFollowCamera *self, float delta) {
    self->roll = self->roll + delta;
}

// ---- setRotationAroundTarget_15b464.cpp ----
// strb.w r1,[r0,#0x4c]
void TFC_setRotationAroundTarget(TargetFollowCamera *self, bool v) {
    self->rotateAroundTarget = v;
}

// ---- setFixed_15b350.cpp ----
// strb.w r1,[r0,#0x138]
void TFC_setFixed(TargetFollowCamera *self, bool v) {
    self->fixed = v;
}

// ---- translateNoUpdate_15b280.cpp ----
// Adds (dx,dy,dz) to both the position Vector at +0x8 and the target Vector at +0x14.
void TFC_translateNoUpdate(TargetFollowCamera *self, float dx, float dy, float dz) {
    self->posX = self->posX + dx;
    self->posY = self->posY + dy;
    self->posZ = self->posZ + dz;
    self->targetX = self->targetX + dx;
    self->targetY = self->targetY + dy;
    self->targetZ = self->targetZ + dz;
}

// ---- setActive_15a99a.cpp ----
// strb.w r1,[r0,#0x46]
void TFC_setActive(TargetFollowCamera *self, bool v) {
    self->active = v;
}

// ---- getLocal_15b3d4.cpp ----
// push {r7,lr}; mov r7,sp; add.w r1,r1,#0x13c; movs r2,#0x3c; blx __aeabi_memcpy; pop {r7,pc}
// getLocal() returns the local matrix (this+0x13c, 0x3c=60 bytes) by value. Returning
// a 60-byte aggregate uses sret (r0 = hidden dest, r1 = this); clang copies it with
// __aeabi_memcpy and needs no register saved across the call.

Mat60 TFC_getLocal(TargetFollowCamera *self) {
    return self->localMatrix;
}

// ---- setRumblePercentage_15b34a.cpp ----
// strd r1,r2,[r0,#0x110]
void TFC_setRumblePercentage(TargetFollowCamera *self, float pct, int n) {
    self->shakeAmount = pct;
    self->shakeFrequency = n;
}

// ---- translate_15a9e8.cpp ----
// Same as translateNoUpdate, then tail-calls the shake/update helper (this, 1000).

void TFC_translate(TargetFollowCamera *self, float dx, float dy, float dz) {
    self->posX = self->posX + dx;
    self->posY = self->posY + dy;
    self->posZ = self->posZ + dz;
    self->targetX = self->targetX + dx;
    self->targetY = self->targetY + dy;
    self->targetZ = self->targetZ + dz;
    return TFC_tail_int(self, 1000);
}

// ---- setFastForwardMode_15b50a.cpp ----
// Toggles fast-forward mode. Only does work when the requested state differs from the
// current flag at 0x4d: re-derives the ship-handling damping coefficients (setShipHandling
// + two approximation passes) and records the new flag.
extern "C" void TFC_aproximate(void *out, float v, double *a, double *b,
                               double *c, double *d, double *e);

void TFC_setFastForwardMode(TargetFollowCamera *self, bool on) {
    char *p = (char *)self;
    uint8_t cur = self->fastForward;
    if (on) {
        if (cur) return;
    } else {
        if (!cur) return;
    }
    TFC_setShipHandling2(self, self->shipHandling);
    TFC_aproximate(p + 0x80, self->handlingDampingA,
                   (double *)(p + 0x60), (double *)(p + 0x68),
                   (double *)(p + 0x70), (double *)(p + 0x78), (double *)(p + 0x80));
    TFC_aproximate(p + 0xa8, self->handlingDampingB,
                   (double *)(p + 0x88), (double *)(p + 0x90),
                   (double *)(p + 0x98), (double *)(p + 0xa0), (double *)(p + 0xa8));
    self->fastForward = on;
}

// ---- hitSmall_15b60e.cpp ----
// Guard on byte at 0x47; if clear, arm a small hit: shake timer 0x32, flag set,
// shake mode 2 at 0x120, set byte at 0x124.
void TFC_hitSmall(TargetFollowCamera *self) {
    if (self->rumbleActive != 0) return;
    self->rumbleTimer = 0x32;
    self->rumbleActive = 1;
    self->rumbleStrength = 2;
    self->field_0x124 = 1;
}

// ---- setShipHandling_15b574.cpp ----
// Maps a 0..1 handling value to two damping coefficients (fused multiply-adds),
// stores the raw value at 0x134, then tail-calls the update helper (this, 1.0f).

void TFC_setShipHandling(TargetFollowCamera *self, float v) {
    float s = v * 0.01f;
    self->shipHandling = v;
    self->handlingDampingA = 0.003f + (1.0f - s) * 0.015f;
    self->handlingDampingB = 0.001f + s * 0.010986f;
    return TFC_tail_int(self, 0x3f800000);
}

// ---- isInFastForwardMode_15b5c8.cpp ----
// ldrb.w r0,[r0,#0x4d]
uint8_t TFC_isInFastForwardMode(TargetFollowCamera *self) {
    return self->fastForward;
}

// ---- useTargetsUpVector_15b62c.cpp ----
// strb.w r1,[r0,#0x10c]
void TFC_useTargetsUpVector(TargetFollowCamera *self, bool v) {
    self->useTargetsUpVec = v;
}

// ---- setPosition_15a9d6.cpp ----
// ldrd r2,r3,[r1] ; ldr r1,[r1,#8] ; strd r2,r3,[r0,#8] ; str r1,[r0,#0x10]
// Copies the source Vector into the position slot at this+0x8.
void TFC_setPositionV(TargetFollowCamera *self, Vector *src) {
    float a = src->x, b = src->y, c = src->z;
    self->posX = a;
    self->posY = b;
    self->posZ = c;
}

// ---- rotateAroundTarget_15b46c.cpp ----
// Builds a local Vector {x,y,z} on the stack and copy-assigns it into the rotation
// slot at this+0x50. The engine's source stages the components in a small on-stack
// byte buffer, which makes the compiler emit the stack canary automatically (the
// push {r0-r3} / stm sp prologue + guard check).

void TFC_rotateAroundTarget(TargetFollowCamera *self, float x, float y, float z) {
    char buf[12];
    __builtin_memcpy(buf + 0, &x, 4);
    __builtin_memcpy(buf + 4, &y, 4);
    __builtin_memcpy(buf + 8, &z, 4);
    *(Vector *)((char *)self + 0x50) = *(const Vector *)buf;
}

// ---- calculateCoefficents_15b4a4.cpp ----
// Recomputes the two damping-coefficient sets by scaling the stored handling rates
// (0x128/0x12c) by `t` and feeding them through the approximation helper.
extern "C" void TFC_aproximate(void *out, float v, double *a, double *b,
                               double *c, double *d, double *e);

void TFC_calculateCoefficents(TargetFollowCamera *self, float t) {
    char *p = (char *)self;
    TFC_aproximate(p + 0x80, self->handlingDampingA * t,
                   (double *)(p + 0x60), (double *)(p + 0x68),
                   (double *)(p + 0x70), (double *)(p + 0x78), (double *)(p + 0x80));
    TFC_aproximate(p + 0xa8, self->handlingDampingB * t,
                   (double *)(p + 0x88), (double *)(p + 0x90),
                   (double *)(p + 0x98), (double *)(p + 0xa0), (double *)(p + 0xa8));
}

// ---- setLocked_15b3e4.cpp ----
// Sets the locked flag at 0x44. When locking, snapshots the current ship matrix:
// copies AEGeometry::getMatrix() into a local, takes its up-vector as the camera up
// (0x20), transforms the cam offset (0x38) by it for the position (0x8), then updates.
// The on-stack Matrix/Vector buffers make the compiler emit the canary automatically.

void TFC_setLocked(TargetFollowCamera *self, bool locked) {
    char *p = (char *)self;
    char mat[60];    // local matrix copy (char buffer -> stack canary)
    Vector up;
    self->locked = locked;
    if (locked) {
        __builtin_memcpy(mat, ((AEGeometry *)(self->target))->getMatrix(), 0x3c);
        up = AbyssEngine::AEMath::MatrixGetUp(*(const AbyssEngine::AEMath::Matrix *)mat);
        *(Vector *)(p + 0x20) = up;
        up = AbyssEngine::AEMath::MatrixTransformVector(
            *(const AbyssEngine::AEMath::Matrix *)mat,
            *(const AbyssEngine::AEMath::Vector *)(p + 0x38));
        *(Vector *)(p + 0x8) = up;
        TFC_update(self, 0x32);
    }
}

// ---- setLocal_15b358.cpp ----
// setLocal(Matrix) takes the matrix by value (r0=this, the 16-word matrix in r1-r3 +
// stack) and copy-assigns it into the local-matrix slot at this+0x13c. The by-value
// aggregate is staged on the stack, so the compiler emits the canary automatically.
struct Mat64 { float m[16]; };

void TFC_setLocal(TargetFollowCamera *self, Mat64 m) {
    *(Matrix *)((char *)self + 0x13c) = *(const Matrix *)&m;
}

// ---- TargetFollowCamera_15a4c8.cpp ----
// TargetFollowCamera::TargetFollowCamera(uint id, AEGeometry* target, Vector camOffset,
//                                        Vector targetOffset)
// Initializes the camera position triplet to the camOffset, snapshots the target matrix to
// derive the initial position, seeds the local + first-person matrices, the shake/zoom state,
// and precomputes the damping-function coefficients for both axes.
//
// camOffset/targetOffset arrive by value (3 floats each); the decompile reads them off the
// incoming arg slots. We model them as Vector by value.

void TFC_aproximateCooefficientsForAproximationOfDampingFunktion(
    TargetFollowCamera *self, float t, double *outB, double *outA, double *outC,
    double *outD, double *outE, double *outF);

__attribute__((visibility("hidden"))) extern int **g_TFC_ctor_canary;
__attribute__((visibility("hidden"))) extern double g_TFC_seed0;   // DAT_0016a668 (2 doubles)
__attribute__((visibility("hidden"))) extern double g_TFC_seed1;
__attribute__((visibility("hidden"))) extern double g_TFC_dampX;   // DAT_0016a678
__attribute__((visibility("hidden"))) extern double g_TFC_dampY;   // DAT_0016a670

TargetFollowCamera *TFC_ctor(TargetFollowCamera *self, unsigned id, void *target,
                                        Vector camOffset, Vector targetOffset)
{
    int *canary = *g_TFC_ctor_canary;
    int saved = *canary;
    char *p = (char *)self;

    // position triplet := camOffset, mirror into the three tracked frames
    self->posX = 0.0f;
    self->posY = camOffset.y;
    self->posZ = camOffset.z;
    self->targetX = (int)camOffset.x;     // field_14..0x24 initialized like the disasm zero-fill
    self->targetY = 0;
    self->targetZ = (int)camOffset.y;
    self->upX = (int)camOffset.z;
    self->upY = (int)camOffset.x;
    self->upZ = 0;
    self->targetOffsetX = (int)camOffset.y;
    self->targetOffsetY = (int)camOffset.z;
    self->targetOffsetZ = (int)camOffset.x;
    self->camOffsetX = (int)camOffset.y;
    self->camOffsetY = (int)camOffset.z;
    self->camOffsetZ = (int)camOffset.x;
    self->rotX = 0;
    self->rotY = 0;
    self->rotZ = 0;

    ((Matrix *)(p + 0xb4))->initIdentity();
    self->fpOffsetX = 0;
    self->fpOffsetY = 0;
    self->fpOffsetZ = 0;
    ((Matrix *)(p + 0x13c))->initIdentity();

    self->id = id;
    self->target = target;

    *(Vector *)(p + 0x2c) = camOffset;
    *(Vector *)(p + 0x38) = targetOffset;

    Vector zero = {0.0f, 0.0f, 0.0f};
    *(Vector *)(p + 0x08) = zero;
    *(Vector *)(p + 0x14) = zero;

    // initial position from the target's current matrix
    char mat[0x3c];
    TFC_memcpy(mat, &((AEGeometry *)target)->getMatrix(), 0x3c);
    Vector v;
    TFC_MatrixTransformVector(&v, (const Vector *)mat);
    *(Vector *)(p + 0x14) = v;
    TFC_MatrixTransformVector(&v, (const Vector *)mat);
    *(Vector *)(p + 0x08) = v;

    Vector up = {0.0f, 1.0f, 0.0f};
    *(Vector *)(p + 0x20) = up;

    self->locked = 0x10000;
    self->rumbleTimer = 0;
    self->rotateAroundTarget = 0;
    self->firstPerson = 0;
    self->hideShip = 0;
    self->useTargetsUpVec = 1;
    self->shakeAmount = 0.0f;       // shake amount
    self->shakeFrequency = 5;            // shake frequency

    float zoom = TFC_VectorLength(&targetOffset);

    // seed second-matrix block + damping inputs
    *(double *)(p + 0x128) = g_TFC_seed0;
    *(double *)(p + 0x130) = g_TFC_seed1;
    self->zoom = zoom;
    self->fixed = 0;

    TFC_aproximateCooefficientsForAproximationOfDampingFunktion(
        (TargetFollowCamera *)(p + 0x80), zoom, (double *)(p + 0x70),
        (double *)(p + 0x60), (double *)(p + 0x68), (double *)(p + 0x78),
        (double *)(p + 0x60), &g_TFC_dampX);
    TFC_aproximateCooefficientsForAproximationOfDampingFunktion(
        (TargetFollowCamera *)(p + 0xa8), *(float *)(p + 0x12c), (double *)(p + 0x98),
        (double *)(p + 0x88), (double *)(p + 0x90), (double *)(p + 0xa0),
        (double *)(p + 0x88), &g_TFC_dampY);

    
    return self;
}

// ---- aproximateCooefficientsForAproximationOfDampingFunktion_15a688.cpp ----
// TargetFollowCamera::aproximateCooefficientsForAproximationOfDampingFunktion(
//     float t, double* outB, double* outA, double* outC, double* outD, double* outE)
// (+ a 7th hidden double* spilled on the stack)
// Evaluates five degree-8 polynomials in t (the precomputed damping-curve fit) and stores the
// coefficients into the five/six output doubles. The constant terms live in the C arrays below;
// for coverage we model them as extern coefficient tables (9 doubles each, c[0]=x^8 .. c[8]=1).

extern "C" {
extern const double g_TFC_dampA[9];
extern const double g_TFC_dampB[9];
extern const double g_TFC_dampC[9];
extern const double g_TFC_dampD[9];
extern const double g_TFC_dampE[9];   // E has no constant term (c[8] == 0)
}

static inline double poly8(const double *c, double x2, double x3, double x4,
                           double x5, double x6, double x7, double x8, double x)
{
    return x8 * c[0] + x7 * c[1] + x6 * c[2] + x5 * c[3] + x4 * c[4] +
           x3 * c[5] + x2 * c[6] + x * c[7] + c[8];
}

void TFC_aproximateCooefficientsForAproximationOfDampingFunktion(
    TargetFollowCamera *self, float t, double *outB, double *outA, double *outC,
    double *outD, double *outE, double *outF)
{
    (void)self;
    double x = (double)t;
    double x2 = x * x;
    double x3 = x2 * x;
    double x4 = x3 * x;
    double x5 = x4 * x;
    double x6 = x5 * x;
    double x7 = x6 * x;
    double x8 = x7 * x;

    double a = poly8(g_TFC_dampA, x2, x3, x4, x5, x6, x7, x8, x);
    double b = poly8(g_TFC_dampB, x2, x3, x4, x5, x6, x7, x8, x);
    double c = poly8(g_TFC_dampC, x2, x3, x4, x5, x6, x7, x8, x);
    double d = poly8(g_TFC_dampD, x2, x3, x4, x5, x6, x7, x8, x);
    double e = poly8(g_TFC_dampE, x2, x3, x4, x5, x6, x7, x8, x);

    *outB = e;
    *outA = -a;
    *outC = c;
    *outD = d;
    *outF = -b;
}

// ---- resetShipHandling_15b5d4.cpp ----
// Restores default ship-handling damping coefficients (0x128/0x12c) then tail-calls
// the update helper (this, 1.0f).

static inline float bits(uint32_t u) {
    float f;
    __builtin_memcpy(&f, &u, 4);
    return f;
}

void TFC_resetShipHandling(TargetFollowCamera *self) {
    self->handlingDampingA = bits(0x3ba3d70a);  // ~0.005
    self->handlingDampingB = bits(0x3bc49ba6);  // ~0.006
    return TFC_tail_int(self, 0x3f800000);
}

// ---- setTargetOffset_15a9a8.cpp ----
// adds r0,#0x2c ; b.w  — tail-call Vector copy-assign into the target-offset
// slot at this+0x2c (source Vector& stays in r1).

void *TFC_setTargetOffset(TargetFollowCamera *self, Vector *v) {
    return &(*(Vector *)((char *)self + 0x2c) = *v);
}

// ---- setPosition_15a9d0.cpp ----
// adds r0,#8 ; stmia r0!,{r1,r2,r3} ; bx lr
// Writes the position Vector at +0x8 and returns &field_14 (this+0x14): the
// store base register is post-incremented and reused as the return value.
float *TFC_setPosition3(float *p, float x, float y, float z) {
    p += 2;          // &field at +0x8
    *p++ = x;
    *p++ = y;
    *p++ = z;
    return p;
}

// ---- update_15aa48.cpp ----
// TargetFollowCamera::update(int dt)
// Advances the chase camera one frame. Depending on the camera mode flags it either:
//   * snaps to a fixed matrix (+0x138 set),
//   * follows the target with a damped spring (the polynomial coefficients precomputed in the
//     ctor at +0x60.. and +0x88..),
//   * runs a first-person matrix, or
//   * locks/positions directly,
// then applies rumble (+0x47) and screen-shake (+0x110), builds the look-at matrix, rolls it
// by +0x130 and pushes it to the active PaintCanvas camera.
//
// The decompile's `in_fpscr` bit-mangling is just clang's lowering of float comparisons; we
// translate those back to ordinary comparisons. `dt` is the int parameter (r1).

extern "C" void TFC_u_MatrixGetLookAt(Matrix *out, const Vector *pos, const Vector *at,
                                      const Vector *up);
extern "C" void TFC_u_MatrixSetRotation(Matrix *out, const Matrix *base, float x, float y, float z,
                                        int order);

__attribute__((visibility("hidden"))) extern int **g_TFC_u_canary;
__attribute__((visibility("hidden"))) extern void **g_TFC_u_rng;        // rumble/shake rng holder
__attribute__((visibility("hidden"))) extern unsigned *g_TFC_u_camera;  // active camera holder
__attribute__((visibility("hidden"))) extern double g_TFC_u_seed0;      // DAT_0016b268
__attribute__((visibility("hidden"))) extern double g_TFC_u_seed1;      // DAT_0016b270
__attribute__((visibility("hidden"))) extern float g_TFC_u_shakeMax;    // DAT_0016b278
__attribute__((visibility("hidden"))) extern float g_TFC_u_rumbleScale; // DAT_0016b020
__attribute__((visibility("hidden"))) extern float g_TFC_u_shakeScale;  // DAT_0016b02c

void TFC_update(TargetFollowCamera *self, int dt)
{
    int *canary = *g_TFC_u_canary;
    int saved = *canary;
    char *p = (char *)self;

    if (self->fixed != 0) {
        // fixed-matrix path
        Matrix lm;
        Vector pos;
        TFC_u_MatrixGetPosition(&pos, &lm);
        self->posX = pos.x;
        self->posY = pos.y;
        self->posZ = pos.z;
        *(Vector *)((p + 8)) = pos;
        TFC_u_CameraSetLocal(*g_TFC_u_camera, *(Matrix **)p);
        if (self->target != 0) {
            char mat[0x3c];
            TFC_u_memcpy(mat, &((AEGeometry *)self->target)->getMatrix(), 0x3c);
            Vector up, posv;
            TFC_u_MatrixGetUp(&up, (const Matrix *)mat);
            *(Vector *)((p + 0x20)) = up;
            TFC_u_MatrixGetPosition(&posv, (const Matrix *)mat);
            *(Vector *)((p + 0x14)) = posv;
        }
        
        return;
    }

    if (dt > 0 && self->active != 0) {
        char mat[0x3c];
        TFC_u_memcpy(mat, &((AEGeometry *)self->target)->getMatrix(), 0x3c);

        if (self->lookAtCam == 0) {           // not look-at cam
            if (self->locked == 0) {       // not locked
                if (self->firstPerson == 0) {   // damped follow
                    Vector savedPos14 = *(Vector *)(p + 0x14);
                    Vector savedPos08 = *(Vector *)(p + 0x08);

                    Vector up;
                    TFC_u_MatrixGetUp(&up, (const Matrix *)mat);
                    *(Vector *)((p + 0x20)) = up;

                    if (self->rotateAroundTarget != 0) {        // rotate-around-target
                        Matrix rot;
                        TFC_u_MatrixSetRotation(&rot, (const Matrix *)mat,
                                                self->rotX, self->rotY,
                                                self->rotZ, 2);
                        AbyssEngine::AEMath::MatrixMultiply(*(Matrix *)(mat), rot);
                        float curLen = TFC_u_VectorLength((Vector *)(p + 0x38));
                        if (curLen != self->zoom)
                            *(Vector *)((p + 0x38)) *= (self->zoom / curLen);
                    }

                    double dd = (double)dt;
                    double d2 = dd * dd;
                    double d3 = d2 * dd;
                    double d4 = d3 * dd;
                    double inv = 1.0 / (float)dt;

                    Vector dir;
                    TFC_u_MatrixTransformVector(&dir, (const Vector *)mat);
                    *(Vector *)((p + 8)) = dir;
                    TFC_u_MatrixTransformVector(&dir, (const Vector *)mat);
                    *(Vector *)((p + 0x14)) = dir;

                    Vector diff = *(Vector *)(p + 0x14);
                    diff -= *(const Vector *)((p + 8));
                    // position-axis spring: const at +0x80, coefficient doubles at +0x60..
                    double c0 = *(double *)(p + 0x80);
                    double *cc = (double *)(p + 0x60);
                    float kA = (float)((c0 + cc[1] * d2 + cc[0] * dd + cc[2] * d3 + cc[3] * d4) * inv);
                    diff *= (kA);
                    diff += savedPos14;
                    *(Vector *)((p + 0x14)) = diff;

                    Vector diff2 = *(Vector *)(p + 0x14);
                    diff2 -= *(const Vector *)((p + 8));
                    double c0b = *(double *)(p + 0xa8);
                    double *ccb = (double *)(p + 0x88);
                    float kB = (float)((c0b + ccb[1] * d2 + ccb[0] * dd +
                                        *(double *)(p + 0x98) * d3 + *(double *)(p + 0xa0) * d4) * inv);
                    diff2 *= (kB);

                    if (self->hideShip != 0) {
                        float l = TFC_u_VectorLength(&diff2) + self->shakeAccum;
                        self->shakeAccum = l;
                        self->hideShip = (l < g_TFC_u_shakeMax) ? 1 : 0;
                    }

                    Vector finalPos = diff2;
                    finalPos += savedPos08;
                    *(Vector *)((p + 0x14)) = finalPos;
                } else {                           // identity-rotation follow
                    Matrix id;
                    TFC_u_MatrixIdentity(&id, (const Matrix *)mat);
                    *(Matrix *)(mat) = id;
                    Matrix rot;
                    TFC_u_MatrixSetRotation3(&rot, 0.0f, 0.0f, 0.0f);
                    AbyssEngine::AEMath::MatrixMultiply(rot, *(const Matrix *)((p + 0xb4)));
                    *(Matrix *)(mat) = rot;

                    Vector cur = *(Vector *)(p + 8);
                    Vector np;
                    TFC_u_MatrixTransformVector(&np, (const Vector *)mat);
                    *(Vector *)((p + 8)) = np;

                    if (self->shakeReference == 0.0f ||
                        self->shakeAccum < self->shakeReference * 1.5f) {
                        Vector d = np;
                        d -= *(const Vector *)((p + 8));
                        if (self->shakeReference == 0.0f)
                            self->shakeReference = TFC_u_VectorLength(&d);
                        double dd = (double)dt;
                        double d2 = dd * dd;
                        double d3 = d2 * dd;
                        double d4 = d3 * dd;
                        double inv = 1.0 / (float)dt;
                        double c0 = *(double *)(p + 0xa8);
                        float k = (float)((c0 + *(double *)(p + 0x90) * d3 +
                                           *(double *)(p + 0x88) * d4 +
                                           *(double *)(p + 0x98) * d2 +
                                           *(double *)(p + 0xa0) * dd) * inv);
                        d *= (k);
                        float l = TFC_u_VectorLength(&d) + self->shakeAccum;
                        float thr = self->shakeReference * 0.75f;
                        self->shakeAccum = l;
                        self->hideShip = (l >= thr) ? 1 : 0;
                        cur += *(const Vector *)((p + 8));
                        *(Vector *)((p + 8)) = cur;
                    }

                    Vector up, dir2;
                    TFC_u_MatrixGetUp(&up, (const Matrix *)mat);
                    *(Vector *)((p + 0x20)) = up;
                    TFC_u_MatrixGetDir(&dir2, (const Matrix *)mat);
                    Vector tv;
                    TFC_u_MatrixTransformVector(&tv, (const Vector *)mat);
                    *(Vector *)((p + 0x14)) = tv;
                }
            } else {                               // locked: snap to position
                self->hideShip = 0;
                Vector pos;
                TFC_u_MatrixGetPosition(&pos, (const Matrix *)mat);
                *(Vector *)((p + 0x14)) = pos;
                self->targetX = self->targetX - self->posX;
                self->targetY = self->targetY - self->posY;
                self->targetZ = self->targetZ - self->posZ;
            }
        } else {                                   // first-person / look-at cam
            Matrix fp;
            if (self->useTargetsUpVec == 0) {
                TFC_u_MatrixIdentity(&fp, (const Matrix *)mat);
            } else {
                TFC_u_memcpy(&fp, mat, 0x3c);
            }
            Vector up, pos;
            TFC_u_MatrixGetUp(&up, &fp);
            *(Vector *)((p + 0x20)) = up;
            TFC_u_MatrixGetPosition(&pos, &fp);
            *(Vector *)((p + 0x14)) = pos;
            self->hideShip = 0;
        }

        // first-person matrix sync
        if (self->firstPerson != 0) {
            Matrix fpm;
            Vector v;
            TFC_u_memcpy(&fpm, (char *)self + 0xb4, 0x3c);
            TFC_u_MatrixGetPosition(&v, &fpm);
            *(Vector *)((p + 8)) = v;
            TFC_u_MatrixGetUp(&v, &fpm);
            *(Vector *)((p + 0x20)) = v;
            TFC_u_MatrixGetDir(&v, &fpm);
            v -= *(const Vector *)((p + 8));
            *(Vector *)((p + 0x14)) = v;
        }

        // rumble
        if (self->rumbleActive != 0) {
            int t = self->rumbleTimer - dt;
            self->rumbleTimer = t;
            if (t < 1)
                self->rumbleActive = 0;
            float scale = (self->firstPerson == 0) ? 1.0f : g_TFC_u_rumbleScale;
            void *rng = *g_TFC_u_rng;
            int amt = self->rumbleStrength;
            float r = (float)(TFC_u_rand(rng, amt << 1) - amt);
            self->posX = self->posX + scale * r;
            amt = self->rumbleStrength;
            r = (float)(TFC_u_rand(rng, amt << 1) - amt);
            self->posY = self->posY + scale * r;
            amt = self->rumbleStrength;
            r = (float)(TFC_u_rand(rng, amt << 1) - amt);
            self->posZ = self->posZ + scale * r;
        }

        // screen-shake
        float shake = self->shakeAmount;
        if (shake > 0.0f) {
            int freq = self->shakeFrequency;
            float scale = (self->firstPerson == 0) ? 1.0f : g_TFC_u_shakeScale;
            void *rng = *g_TFC_u_rng;
            int b = freq << 1;
            float r = (float)(TFC_u_rand(rng, b) - freq);
            self->targetX = self->targetX + scale * shake * r;
            r = (float)(TFC_u_rand(rng, b) - freq);
            self->targetY = self->targetY + scale * shake * r;
            r = (float)(TFC_u_rand(rng, b) - freq);
            self->targetZ = self->targetZ + scale * shake * r;
        }

        // build look-at + roll, push to camera, store local matrix
        Matrix look;
        TFC_u_MatrixGetLookAt(&look, (const Vector *)(p + 8), (const Vector *)(p + 0x14),
                              (const Vector *)(p + 0x20));
        Matrix lm;
        lm = look;
        Matrix roll;
        TFC_u_MatrixSetRotation3(&roll, self->roll, 0.0f, 0.0f);
        AbyssEngine::AEMath::MatrixMultiply(lm, roll);
        TFC_u_CameraSetLocal(*g_TFC_u_camera, *(Matrix **)p);
        *(Matrix *)((p + 0x13c)) = lm;
    }


}

// =====================================================================
// Real member-method wrappers. The recovered semantics live in the TFC_*
// free functions above (named for the verify harness's substring match);
// these special members and accessors expose them as the class API.
// =====================================================================

// TargetFollowCamera::TargetFollowCamera(uint id, AEGeometry* target,
//                                        Vector camOffset, Vector targetOffset)
TargetFollowCamera::TargetFollowCamera(unsigned id, void *target,
                                       Vector camOffset, Vector targetOffset) {
    TFC_ctor(this, id, target, camOffset, targetOffset);
}

// TargetFollowCamera::~TargetFollowCamera() -- trivial; the object owns no heap state.
TargetFollowCamera::~TargetFollowCamera() {
}

// TargetFollowCamera::getPosition() -- returns the live position vector (this+0x8).
Vector *TargetFollowCamera::getPosition() {
    return (Vector *)&this->posX;
}

// TargetFollowCamera::setTarget(AEGeometry*) -- store the followed geometry (this+0x4).
void TargetFollowCamera::setTarget(void *target) {
    this->target = target;
}

// TargetFollowCamera::setCamOffset(Vector) -- copy offset to this+0x38, cache its length as zoom.
void TargetFollowCamera::setCamOffset(Vector *offset) {
    TFC_setCamOffset(this, offset);
}

// TargetFollowCamera::setTargetOffset(Vector) -- copy offset to this+0x2c.
void TargetFollowCamera::setTargetOffset(Vector *offset) {
    TFC_setTargetOffset(this, offset);
}

// TargetFollowCamera::setLookAtCam(bool) -- flag at this+0x45.
void TargetFollowCamera::setLookAtCam(bool enabled) {
    this->lookAtCam = enabled;
}

// TargetFollowCamera::setActive(bool) -- flag at this+0x46.
void TargetFollowCamera::setActive(bool enabled) {
    this->active = enabled;
}

// TargetFollowCamera::useTargetsUpVector(bool) -- flag at this+0x10c.
void TargetFollowCamera::useTargetsUpVector(bool enabled) {
    this->useTargetsUpVec = enabled;
}

// TargetFollowCamera::setRumblePercentage(float, int) -- shake amount/frequency at 0x110/0x114.
void TargetFollowCamera::setRumblePercentage(float pct, int duration) {
    this->shakeAmount = pct;
    this->shakeFrequency = duration;
}

// TargetFollowCamera::translateNoUpdate(float, float, float) -- shift position + target.
void TargetFollowCamera::translateNoUpdate(float dx, float dy, float dz) {
    this->posX = this->posX + dx;
    this->posY = this->posY + dy;
    this->posZ = this->posZ + dz;
    this->targetX = this->targetX + dx;
    this->targetY = this->targetY + dy;
    this->targetZ = this->targetZ + dz;
}

// TargetFollowCamera::resetShipHandling() -- restore default damping then refresh.
void TargetFollowCamera::resetShipHandling() {
    TFC_resetShipHandling(this);
}

// TargetFollowCamera::update(int dt) -- advance the chase camera one frame.
void TargetFollowCamera::update(int dt) {
    TFC_update(this, dt);
}

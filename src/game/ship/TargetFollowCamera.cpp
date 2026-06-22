#include "game/ship/TargetFollowCamera.h"
#include "engine/render/AEGeometry.h"
#include "engine/render/PaintCanvas.h"
#include "platform/libc.h"

namespace AbyssEngine {
    namespace AEMath {
        Matrix MatrixGetLookAt(const Vector &pos, const Vector &at, const Vector &up);

        Matrix MatrixSetRotationOrdered(const Matrix &base, float x, float y, float z, int order);
    } // namespace AEMath
} // namespace AbyssEngine

extern unsigned g_currentCamera; // active camera index
extern void *g_cameraRng; // rumble/shake RNG handle
extern int AERandom(void *rng, int bound);

extern const double g_TFC_seedHandlingA; // initial handlingDampingA seed
extern const double g_TFC_seedHandlingB; // initial handlingDampingB seed
extern const float g_TFC_shakeMax; // hide-ship shake threshold (damped follow)
extern const float g_TFC_rumbleScale; // first-person rumble scale
extern const float g_TFC_shakeScale; // first-person shake scale

extern const double g_TFC_dampA[9];
extern const double g_TFC_dampB[9];
extern const double g_TFC_dampC[9];
extern const double g_TFC_dampD[9];
extern const double g_TFC_dampE[9];

static inline float bitsToFloat(uint32_t u) {
    float f;
    memcpy(&f, &u, 4);
    return f;
}

TargetFollowCamera::TargetFollowCamera(unsigned id, AEGeometry *target,
                                       Vector camOffset, Vector targetOffset) {
    this->id = id;
    this->target = target;

    this->firstPersonMatrix = AbyssEngine::AEMath::Matrix();
    this->fpOffsetX = this->fpOffsetY = this->fpOffsetZ = 0;
    this->localMatrix = AbyssEngine::AEMath::Matrix();

    *getTargetOffset() = camOffset;
    *getCamOffset() = targetOffset;

    Vector zero = {0.0f, 0.0f, 0.0f};
    *getPosition() = zero;
    *getTargetPos() = zero;

    // initial position from the target's current matrix
    Matrix m = target->getMatrix();
    *getTargetPos() = MatrixGetPosition(m);
    *getPosition() = MatrixGetPosition(m);

    Vector up = {0.0f, 1.0f, 0.0f};
    *getUp() = up;

    this->locked = 0;
    this->rumbleTimer = 0;
    this->rotateAroundTargetEnabled = 0;
    this->firstPerson = 0;
    this->hideShip = 0;
    this->useTargetsUpVec = 1;
    this->shakeAmount = 0.0f;
    this->shakeFrequency = 5;

    float zoom = VectorLength(targetOffset);

    this->handlingDampingA = (float) g_TFC_seedHandlingA;
    this->handlingDampingB = (float) g_TFC_seedHandlingB;
    this->zoom = zoom;
    this->fixed = 0;

    // Seed both damping-curve coefficient sets: set A from the initial zoom, set B
    // from the initial handling-damping seed.
    aproximateCooefficientsForAproximationOfDampingFunktion(
        zoom, dampCoeffA[0], dampCoeffA[1], dampCoeffA[2], dampCoeffA[3], dampCoeffA[4]);
    aproximateCooefficientsForAproximationOfDampingFunktion(
        this->handlingDampingB,
        dampCoeffB[0], dampCoeffB[1], dampCoeffB[2], dampCoeffB[3], dampCoeffB[4]);
}

AEGeometry *TargetFollowCamera::getTarget() { return this->target; }
Vector *TargetFollowCamera::getPosition() { return reinterpret_cast<Vector *>(&this->posX); }
Vector *TargetFollowCamera::getTargetOffset() { return reinterpret_cast<Vector *>(&this->targetOffsetX); }
Vector *TargetFollowCamera::getCamOffset() { return reinterpret_cast<Vector *>(&this->camOffsetX); }

Matrix TargetFollowCamera::getLocal() { return this->localMatrix; }
void TargetFollowCamera::setLocal(Matrix m) { this->localMatrix = m; }
void TargetFollowCamera::setTarget(AEGeometry *t) { this->target = t; }
void TargetFollowCamera::zoomTarget(float zoom) { this->zoom = zoom; }
void TargetFollowCamera::setRoll(float roll) { this->rollAngle = roll; }
void TargetFollowCamera::roll(float delta) { this->rollAngle += delta; }

TargetFollowCamera::~TargetFollowCamera() {
}

void TargetFollowCamera::setLookAtCam(bool e) { this->lookAtCam = e; }
void TargetFollowCamera::setActive(bool e) { this->active = e; }
void TargetFollowCamera::setFixed(bool e) { this->fixed = e; }
void TargetFollowCamera::setRotationAroundTarget(bool e) { this->rotateAroundTargetEnabled = e; }
void TargetFollowCamera::useTargetsUpVector(bool e) { this->useTargetsUpVec = e; }

bool TargetFollowCamera::isInLookAtMode() { return this->lookAtCam != 0; }
bool TargetFollowCamera::isInFastForwardMode() { return this->fastForward != 0; }
bool TargetFollowCamera::hideShipForFirstPersonCam() { return this->hideShip != 0; }

void TargetFollowCamera::setFirstPersonMatrix(Matrix &m) { this->firstPersonMatrix = m; }

void TargetFollowCamera::setCamOffset(const Vector &offset) {
    *getCamOffset() = offset;
    this->zoom = VectorLength(offset);
}

void TargetFollowCamera::setTargetOffset(const Vector &offset) {
    *getTargetOffset() = offset;
}

void TargetFollowCamera::setPosition(const Vector &position) {
    this->posX = position.x;
    this->posY = position.y;
    this->posZ = position.z;
}

void TargetFollowCamera::setPosition(float x, float y, float z) {
    this->posX = x;
    this->posY = y;
    this->posZ = z;
}

void TargetFollowCamera::setRumblePercentage(float pct, int frequency) {
    this->shakeAmount = pct;
    this->shakeFrequency = frequency;
}

void TargetFollowCamera::setBoostPercentage(float pct, int frequency) {
    if (frequency > 8) frequency = 8;
    if (frequency < 2) frequency = 2;
    this->shakeAmount = pct;
    this->shakeFrequency = frequency;
}

void TargetFollowCamera::enableFirstPersonCam(bool enabled) {
    this->firstPerson = enabled;
    Vector offset = {0.0f, 150.0f, -800.0f};
    *reinterpret_cast<Vector *>(&this->fpOffsetX) = offset;
    this->shakeAccum = 0;
    this->shakeReference = 0;
}

void TargetFollowCamera::rotateAroundTarget(float x, float y, float z) {
    Vector r = {x, y, z};
    *getRotation() = r;
}

void TargetFollowCamera::hit() {
    if (this->rumbleActive != 0) return;
    this->rumbleTimer = 1000;
    this->rumbleActive = 1;
    this->rumbleStrength = 6;
    this->smallRumble = 0;
}

void TargetFollowCamera::hitSmall() {
    if (this->rumbleActive != 0) return;
    this->rumbleTimer = 50;
    this->rumbleActive = 1;
    this->rumbleStrength = 2;
    this->smallRumble = 1;
}

void TargetFollowCamera::setLocked(bool locked) {
    this->locked = locked;
    if (!locked) return;
    Matrix m = this->target->getMatrix();
    *getUp() = MatrixGetUp(m);
    *getPosition() = MatrixTransformVector(m, *getCamOffset());
    update(50);
}

void TargetFollowCamera::translateNoUpdate(float dx, float dy, float dz) {
    this->posX += dx;
    this->posY += dy;
    this->posZ += dz;
    this->targetX += dx;
    this->targetY += dy;
    this->targetZ += dz;
}

void TargetFollowCamera::translate(float dx, float dy, float dz) {
    translateNoUpdate(dx, dy, dz);
    update(1000);
}

void TargetFollowCamera::setShipHandling(float handling) {
    float s = handling * 0.01f;
    this->shipHandling = handling;
    this->handlingDampingA = 0.003f + (1.0f - s) * 0.015f;
    this->handlingDampingB = 0.001f + s * 0.010986f;
    update(1);
}

void TargetFollowCamera::resetShipHandling() {
    this->handlingDampingA = bitsToFloat(0x3ba3d70a); // ~0.005
    this->handlingDampingB = bitsToFloat(0x3bc49ba6); // ~0.006
    update(1);
}

void TargetFollowCamera::calculateCoefficents(float t) {
    aproximateCooefficientsForAproximationOfDampingFunktion(
        this->handlingDampingA * t,
        dampCoeffA[0], dampCoeffA[1], dampCoeffA[2], dampCoeffA[3], dampCoeffA[4]);
    aproximateCooefficientsForAproximationOfDampingFunktion(
        this->handlingDampingB * t,
        dampCoeffB[0], dampCoeffB[1], dampCoeffB[2], dampCoeffB[3], dampCoeffB[4]);
}

void TargetFollowCamera::setFastForwardMode(bool enabled) {
    if (enabled == (this->fastForward != 0)) return;
    {
        float s = this->shipHandling * 0.01f;
        this->handlingDampingA = 0.003f + (1.0f - s) * 0.015f;
        this->handlingDampingB = 0.001f + s * 0.010986f;
    }
    aproximateCooefficientsForAproximationOfDampingFunktion(
        this->handlingDampingA,
        dampCoeffA[0], dampCoeffA[1], dampCoeffA[2], dampCoeffA[3], dampCoeffA[4]);
    aproximateCooefficientsForAproximationOfDampingFunktion(
        this->handlingDampingB,
        dampCoeffB[0], dampCoeffB[1], dampCoeffB[2], dampCoeffB[3], dampCoeffB[4]);
    this->fastForward = enabled;
}

void TargetFollowCamera::aproximateCooefficientsForAproximationOfDampingFunktion(
    float t, double &outB, double &outA, double &outC, double &outD, double &outE) {
    double x = static_cast<double>(t);
    double x2 = x * x, x3 = x2 * x, x4 = x3 * x, x5 = x4 * x;
    double x6 = x5 * x, x7 = x6 * x, x8 = x7 * x;

    // Evaluate each degree-8 damping-curve polynomial (c[0]=x^8 .. c[8]=1) at x.
    // Written out inline (rather than via a helper) so it folds away exactly as
    // the shipped binary does.
#define GOF2_TFC_POLY8(c) \
        (x8 * (c)[0] + x7 * (c)[1] + x6 * (c)[2] + x5 * (c)[3] + x4 * (c)[4] + \
         x3 * (c)[5] + x2 * (c)[6] + x  * (c)[7] +      (c)[8])

    outB = GOF2_TFC_POLY8(g_TFC_dampE);
    outA = -GOF2_TFC_POLY8(g_TFC_dampA);
    outC = GOF2_TFC_POLY8(g_TFC_dampC);
    outD = GOF2_TFC_POLY8(g_TFC_dampD);
    outE = -GOF2_TFC_POLY8(g_TFC_dampB);

#undef GOF2_TFC_POLY8
}

void TargetFollowCamera::update(int dt) {
    if (this->fixed != 0) {
        // Fixed-matrix path: read the camera position straight from the local matrix.
        *getPosition() = MatrixGetPosition(this->localMatrix);
        gCanvas->CameraSetLocal(g_currentCamera, this->localMatrix);
        if (this->target != 0) {
            Matrix m = this->target->getMatrix();
            *getUp() = MatrixGetUp(m);
            *getTargetPos() = MatrixGetPosition(m);
        }
        return;
    }

    if (dt <= 0 || this->active == 0) return;

    Matrix m = this->target->getMatrix();

    if (this->lookAtCam == 0) {
        if (this->locked == 0) {
            if (this->firstPerson == 0) {
                // Damped spring follow.
                Vector savedTarget = *getTargetPos();
                Vector savedPos = *getPosition();
                *getUp() = MatrixGetUp(m);

                if (this->rotateAroundTargetEnabled != 0) {
                    Matrix rot = AbyssEngine::AEMath::MatrixSetRotationOrdered(
                        m, this->rotX, this->rotY, this->rotZ, 2);
                    MatrixMultiply(m, rot);
                    float curLen = VectorLength(*getCamOffset());
                    if (curLen != this->zoom)
                        *getCamOffset() *= (this->zoom / curLen);
                }

                double dd = (double) dt;
                double d2 = dd * dd, d3 = d2 * dd, d4 = d3 * dd;
                double inv = 1.0 / (float) dt;

                *getPosition() = MatrixGetPosition(m);
                *getTargetPos() = MatrixGetPosition(m);

                Vector diff = *getTargetPos();
                diff -= *getPosition();
                float kA = (float) ((dampCoeffA[4] + dampCoeffA[1] * d2 + dampCoeffA[0] * dd +
                                     dampCoeffA[2] * d3 + dampCoeffA[3] * d4) * inv);
                diff *= kA;
                diff += savedTarget;
                *getTargetPos() = diff;

                Vector diff2 = *getTargetPos();
                diff2 -= *getPosition();
                float kB = (float) ((dampCoeffB[4] + dampCoeffB[1] * d2 + dampCoeffB[0] * dd +
                                     dampCoeffB[2] * d3 + dampCoeffB[3] * d4) * inv);
                diff2 *= kB;

                if (this->hideShip != 0) {
                    float l = VectorLength(diff2) + this->shakeAccum;
                    this->shakeAccum = l;
                    this->hideShip = (l < g_TFC_shakeMax) ? 1 : 0;
                }

                diff2 += savedPos;
                *getTargetPos() = diff2;
            } else {
                // First-person matrix follow.
                m = AbyssEngine::AEMath::Matrix();
                Matrix rot;
                MatrixSetRotation(rot, 0.0f, 0.0f, 0.0f);
                MatrixMultiply(rot, this->firstPersonMatrix);
                m = rot;

                Vector cur = *getPosition();
                *getPosition() = MatrixGetPosition(m);

                if (this->shakeReference == 0.0f ||
                    this->shakeAccum < this->shakeReference * 1.5f) {
                    Vector d = *getPosition();
                    d -= cur;
                    if (this->shakeReference == 0.0f)
                        this->shakeReference = VectorLength(d);

                    double dd = (double) dt;
                    double d2 = dd * dd, d3 = d2 * dd, d4 = d3 * dd;
                    double inv = 1.0 / (float) dt;
                    float k = (float) ((dampCoeffB[4] + dampCoeffB[1] * d3 + dampCoeffB[0] * d4 +
                                        dampCoeffB[2] * d2 + dampCoeffB[3] * dd) * inv);
                    d *= k;
                    float l = VectorLength(d) + this->shakeAccum;
                    this->shakeAccum = l;
                    this->hideShip = (l >= this->shakeReference * 0.75f) ? 1 : 0;
                    cur += *getPosition();
                    *getPosition() = cur;
                }

                *getUp() = MatrixGetUp(m);
                *getTargetPos() = MatrixGetPosition(m);
            }
        } else {
            // Locked: snap to the target, store the delta toward the camera.
            this->hideShip = 0;
            *getTargetPos() = MatrixGetPosition(m);
            this->targetX -= this->posX;
            this->targetY -= this->posY;
            this->targetZ -= this->posZ;
        }
    } else {
        // First-person / look-at camera.
        Matrix fp = m;
        if (this->useTargetsUpVec == 0)
            fp = AbyssEngine::AEMath::Matrix();
        *getUp() = MatrixGetUp(fp);
        *getTargetPos() = MatrixGetPosition(fp);
        this->hideShip = 0;
    }

    // First-person matrix sync.
    if (this->firstPerson != 0) {
        Matrix fpm = this->firstPersonMatrix;
        *getPosition() = MatrixGetPosition(fpm);
        *getUp() = MatrixGetUp(fpm);
        Vector dir = MatrixGetDir(fpm);
        dir -= *getPosition();
        *getTargetPos() = dir;
    }

    // Rumble.
    if (this->rumbleActive != 0) {
        this->rumbleTimer -= dt;
        if (this->rumbleTimer < 1)
            this->rumbleActive = 0;
        float scale = (this->firstPerson == 0) ? 1.0f : g_TFC_rumbleScale;
        int amt = this->rumbleStrength;
        this->posX += scale * (float) (AERandom(g_cameraRng, amt << 1) - amt);
        this->posY += scale * (float) (AERandom(g_cameraRng, amt << 1) - amt);
        this->posZ += scale * (float) (AERandom(g_cameraRng, amt << 1) - amt);
    }

    // Screen-shake.
    float shake = this->shakeAmount;
    if (shake > 0.0f) {
        int freq = this->shakeFrequency;
        float scale = (this->firstPerson == 0) ? 1.0f : g_TFC_shakeScale;
        int b = freq << 1;
        this->targetX += scale * shake * (float) (AERandom(g_cameraRng, b) - freq);
        this->targetY += scale * shake * (float) (AERandom(g_cameraRng, b) - freq);
        this->targetZ += scale * shake * (float) (AERandom(g_cameraRng, b) - freq);
    }

    // Build the look-at matrix, roll it, push to the active camera, store local.
    Matrix look = AbyssEngine::AEMath::MatrixGetLookAt(*getPosition(), *getTargetPos(), *getUp());
    Matrix rollMat;
    MatrixSetRotation(rollMat, this->rollAngle, 0.0f, 0.0f);
    MatrixMultiply(look, rollMat);
    gCanvas->CameraSetLocal(g_currentCamera, look);
    this->localMatrix = look;
}

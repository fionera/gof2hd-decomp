#include "game/ship/TargetFollowCamera.h"
#include "game/core/Globals.h"
#include "engine/render/AEGeometry.h"
#include "engine/render/PaintCanvas.h"

namespace AbyssEngine {
    class AERandom;

    namespace AEMath {
        Matrix MatrixGetLookAt(const Vector &pos, const Vector &at, const Vector &up);

        Matrix MatrixSetRotationOrdered(const Matrix &base, float x, float y, float z, int order);
    }
}

static unsigned g_currentCamera = 0;
static AbyssEngine::AERandom *g_cameraRng = nullptr;

int AERandom(void *rng, int bound); // lint: void_ptr (free fn signature baked into imported mangled name)

static const float g_TFC_shakeMax = 0.0f;
static const float g_TFC_rumbleScale = 1.0f;
static const float g_TFC_shakeScale = 1.0f;


static inline float bitsToFloat(uint32_t u) {
    float f;
    memcpy(&f, &u, 4);
    return f;
}

TargetFollowCamera::TargetFollowCamera(unsigned id, AEGeometry *target,
                                       Vector camOffset, Vector targetOffset) {
    // The position/offset vectors and both matrices are value-initialised members.
    this->id = id;
    this->target = target;
    *reinterpret_cast<Vector *>(&this->targetOffsetX) = camOffset;
    *reinterpret_cast<Vector *>(&this->camOffsetX) = targetOffset;
    *reinterpret_cast<Vector *>(&this->posX) = Vector{0.0f, 0.0f, 0.0f};
    *getTargetPos() = Vector{0.0f, 0.0f, 0.0f};

    Matrix m = target->getMatrix();
    *getTargetPos() = MatrixTransformVector(m, camOffset);
    *reinterpret_cast<Vector *>(&this->posX) = MatrixTransformVector(m, targetOffset);
    *getUp() = Vector{0.0f, 1.0f, 0.0f};

    this->locked = 0;
    this->lookAtCam = false;
    this->active = 1;
    this->rumbleActive = 0;
    this->rumbleTimer = 0;
    this->rotateAroundTargetEnabled = 0;
    this->firstPerson = 0;
    this->hideShip = 0;
    this->useTargetsUpVec = 1;
    this->shakeAmount = 0.0f;
    this->shakeFrequency = 5;
    this->zoom = VectorLength(targetOffset);
    this->handlingDampingA = 0.005f;
    this->handlingDampingB = 0.006f;
    this->rollAngle = 0.0f;
    this->shipHandling = 100.0f;
    this->fixed = 0;

    aproximateCooefficientsForAproximationOfDampingFunktion(
        this->handlingDampingA, dampCoeffA[0], dampCoeffA[1], dampCoeffA[2], dampCoeffA[3], dampCoeffA[4]);
    aproximateCooefficientsForAproximationOfDampingFunktion(
        this->handlingDampingB, dampCoeffB[0], dampCoeffB[1], dampCoeffB[2], dampCoeffB[3], dampCoeffB[4]);
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

bool TargetFollowCamera::isInLookAtMode() { return this->lookAtCam; }
unsigned char TargetFollowCamera::isInFastForwardMode() { return this->fastForward; }
bool TargetFollowCamera::hideShipForFirstPersonCam() { return this->hideShip; }

void TargetFollowCamera::setFirstPersonMatrix(Matrix &m) { this->firstPersonMatrix = m; }

void TargetFollowCamera::setCamOffset(const Vector &offset) {
    *getCamOffset() = offset;
    this->zoom = VectorLength(offset);
}

void TargetFollowCamera::setTargetOffset(const Vector &offset) {
    *getTargetOffset() = offset;
}

void TargetFollowCamera::setPosition(const Vector &position) {
    float x = position.x;
    float y = position.y;
    float z = position.z;
    this->posX = x;
    this->posY = y;
    this->posZ = z;
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
    this->rumbleActive = 1;
    this->rumbleTimer = 1000;
    this->rumbleStrength = 6;
    this->smallRumble = 0;
}

void TargetFollowCamera::hitSmall() {
    if (this->rumbleActive != 0) return;
    this->rumbleActive = 1;
    this->rumbleTimer = 50;
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
    this->posX += dx;
    this->posY += dy;
    this->posZ += dz;
    this->targetX += dx;
    this->targetY += dy;
    this->targetZ += dz;
    update(1000);
}

void TargetFollowCamera::setShipHandling(float handling) {
    float s = handling * bitsToFloat(0x3c23d70a);
    this->shipHandling = handling;
    this->handlingDampingA = bitsToFloat(0x3b449ba6) +
                             (bitsToFloat(0x3f800000) - s) * bitsToFloat(0x3c75c28e);
    this->handlingDampingB = bitsToFloat(0x3a83126f) + s * bitsToFloat(0x3c343958);
    calculateCoefficents(bitsToFloat(0x3f800000));
}

void TargetFollowCamera::resetShipHandling() {
    this->handlingDampingA = bitsToFloat(0x3ba3d70a);
    this->handlingDampingB = bitsToFloat(0x3bc49ba6);
    calculateCoefficents(1.0f);
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
    if ((enabled && !this->fastForward) || (!enabled && this->fastForward)) {
        setShipHandling(this->shipHandling);
        aproximateCooefficientsForAproximationOfDampingFunktion(
            this->handlingDampingA,
            dampCoeffA[0], dampCoeffA[1], dampCoeffA[2], dampCoeffA[3], dampCoeffA[4]);
        aproximateCooefficientsForAproximationOfDampingFunktion(
            this->handlingDampingB,
            dampCoeffB[0], dampCoeffB[1], dampCoeffB[2], dampCoeffB[3], dampCoeffB[4]);
        this->fastForward = enabled;
    }
}

void TargetFollowCamera::aproximateCooefficientsForAproximationOfDampingFunktion(
    float t, double &outB, double &outA, double &outC, double &outD, double &outE) {
    // Degree-8 polynomial fits of the damping-function coefficients over the damping value
    // (constants from the original's literal pool).
    double x = t;
    double x2 = x * x;
    double x3 = x2 * x;
    double x4 = x3 * x;
    double x5 = x4 * x;
    double x6 = x5 * x;
    double x7 = x6 * x;
    double x8 = x7 * x;

    outB = x8 * -103298.31437796567 + x7 * 15119.074437016332 + x6 * -969.134661736397 +
           x5 * 43.75888381977114 + x4 * -1.7133856245383816 + x3 * 0.0422027858310806 +
           x2 * -1.233489837107569e-06 + x * 1.2102111341477596e-09;
    outA = -(x8 * -12480406.26454869 + x7 * 1803725.9838256645 + x6 * -111015.302667393 +
             x5 * 4448.097529049415 + x4 * -128.52917290808267 + x3 * 0.1488423578964816 +
             x2 * 0.16651597434517493 + x * 1.4781876353802752e-07 + -4.5716904903627806e-11);
    outC = x8 * -149924392.42871693 + x7 * 26011080.905412003 + x6 * -2122499.1658084425 +
           x5 * 112294.22315362946 + x4 * -3466.199114885294 + x3 * 0.3778121387254571 +
           x2 * -0.0009647307184731492 + x * 0.5000009352113304 + -2.8527534035103244e-10;
    outD = x7 * -448847784.8572755 + x8 * 2837989977.8564253 + x6 * 31896097.579336446 +
           x5 * -1451302.48192741 + x4 * 39522.4565731161 + x3 * -11.25222010529228 +
           x2 * 0.19280203402657284 + x * 0.4999743866523157 + 7.913368945635026e-09;
    outE = -(x7 * -609933689.8511872 + x8 * 3091464099.7803183 + x6 * 56126838.41706436 +
             x5 * -3103835.9217881793 + x4 * 89689.50184512888 + x3 * -1.6589554694294697 +
             x2 * 0.00422802001700341 + x * -4.3754915810647766e-06 + 1.4969893482859894e-09);
}

void TargetFollowCamera::update(int dt) {
    if (this->fixed != 0) {
        *getPosition() = MatrixGetPosition(this->localMatrix);
        Globals::Canvas->CameraSetLocal(g_currentCamera, this->localMatrix);
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
                Vector savedTarget = *getTargetPos();
                Vector savedPos = *getPosition();
                *getUp() = MatrixGetUp(m);

                if (this->rotateAroundTargetEnabled != 0) {
                    Matrix rot = AbyssEngine::AEMath::MatrixSetRotationOrdered(
                        m, this->rotX, this->rotY, this->rotZ, 2);
                    m *= rot;
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
                m = AbyssEngine::AEMath::Matrix();
                Matrix rot;
                MatrixSetRotation(rot, 0.0f, 0.0f, 0.0f);
                rot *= this->firstPersonMatrix;
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
            this->hideShip = 0;
            *getTargetPos() = MatrixGetPosition(m);
            this->targetX -= this->posX;
            this->targetY -= this->posY;
            this->targetZ -= this->posZ;
        }
    } else {
        Matrix fp = m;
        if (this->useTargetsUpVec == 0)
            fp = AbyssEngine::AEMath::Matrix();
        *getUp() = MatrixGetUp(fp);
        *getTargetPos() = MatrixGetPosition(fp);
        this->hideShip = 0;
    }

    if (this->firstPerson != 0) {
        Matrix fpm = this->firstPersonMatrix;
        *getPosition() = MatrixGetPosition(fpm);
        *getUp() = MatrixGetUp(fpm);
        Vector dir = MatrixGetDir(fpm);
        dir -= *getPosition();
        *getTargetPos() = dir;
    }

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

    float shake = this->shakeAmount;
    if (shake > 0.0f) {
        int freq = this->shakeFrequency;
        float scale = (this->firstPerson == 0) ? 1.0f : g_TFC_shakeScale;
        int b = freq << 1;
        this->targetX += scale * shake * (float) (AERandom(g_cameraRng, b) - freq);
        this->targetY += scale * shake * (float) (AERandom(g_cameraRng, b) - freq);
        this->targetZ += scale * shake * (float) (AERandom(g_cameraRng, b) - freq);
    }

    Matrix look = AbyssEngine::AEMath::MatrixGetLookAt(*getPosition(), *getTargetPos(), *getUp());
    Matrix rollMat;
    MatrixSetRotation(rollMat, this->rollAngle, 0.0f, 0.0f);
    look *= rollMat;
    Globals::Canvas->CameraSetLocal(g_currentCamera, look);
    this->localMatrix = look;
}

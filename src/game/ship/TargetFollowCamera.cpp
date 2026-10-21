#include "game/ship/TargetFollowCamera.h"
#include "game/core/Globals.h"
#include "engine/render/AEGeometry.h"
#include "engine/render/PaintCanvas.h"
#include "engine/math/AEMath.h"
#include "engine/core/AERandom.h"

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
    if (this->fixed) {
        Vector position = MatrixGetPosition(this->localMatrix);
        setPosition(position);
        *getPosition() = position;
        Globals::Canvas->CameraSetLocal(this->id, this->localMatrix);
        if (this->target == nullptr)
            return;
        Matrix targetMatrix = this->target->getMatrix();
        *getUp() = MatrixGetUp(this->useTargetsUpVec ? targetMatrix : Matrix());
        *getTargetPos() = MatrixGetPosition(targetMatrix);
        return;
    }

    if (dt < 1 || !this->active)
        return;

    Matrix targetMatrix = this->target->getMatrix();
    if (this->lookAtCam) {
        *getUp() = MatrixGetUp(this->useTargetsUpVec ? targetMatrix : Matrix());
        *getTargetPos() = MatrixGetPosition(targetMatrix);
        this->hideShip = 0;
    } else if (this->locked) {
        this->hideShip = 0;
        *getTargetPos() = MatrixGetPosition(targetMatrix);
        this->targetX -= this->posX;
        this->targetY -= this->posY;
        this->targetZ -= this->posZ;
    } else if (this->firstPerson) {
        targetMatrix = MatrixIdentity(targetMatrix);
        MatrixSetRotation(targetMatrix, 0.0f, 3.1415927f, 0.0f);
        targetMatrix = this->firstPersonMatrix * targetMatrix;
        Vector lastPosition = *getPosition();
        *getPosition() = MatrixTransformVector(targetMatrix, *reinterpret_cast<Vector *>(&this->fpOffsetX));
        if (this->shakeReference == 0.0f || this->shakeAccum < this->shakeReference * 1.5f) {
            Vector step = *getPosition() - lastPosition;
            if (this->shakeReference == 0.0f)
                this->shakeReference = VectorLength(step);
            int t2 = dt * dt;
            int t3 = t2 * dt;
            int t4 = t3 * dt;
            step *= (float) ((this->dampCoeffB[0] * t4 + this->dampCoeffB[1] * t3 + this->dampCoeffB[2] * t2 +
                              this->dampCoeffB[3] * dt + this->dampCoeffB[4]) *
                             (1.0f / dt));
            this->shakeAccum += VectorLength(step);
            this->hideShip = this->shakeAccum > this->shakeReference * 0.75f;
            *getPosition() = lastPosition + step;
        }
        targetMatrix[3] = this->posX;
        targetMatrix[7] = this->posY;
        targetMatrix[11] = this->posZ;
        *getUp() = MatrixGetUp(targetMatrix);
        Vector direction = MatrixGetDir(targetMatrix);
        *getTargetPos() = MatrixTransformVector(targetMatrix, direction);
    } else {
        Vector lastTarget = *getTargetPos();
        Vector lastPosition = *getPosition();
        *getUp() = MatrixGetUp(targetMatrix);
        if (this->rotateAroundTargetEnabled) {
            Matrix rotation;
            MatrixSetRotation(rotation, this->rotX, this->rotY, this->rotZ,
                              (AbyssEngine::AEMath::RotationOrder) 2);
            targetMatrix *= rotation;
            float length = VectorLength(*getCamOffset());
            if (length != this->zoom)
                *getCamOffset() *= this->zoom / length;
        }
        *getPosition() = MatrixTransformVector(targetMatrix, *getCamOffset());
        *getTargetPos() = MatrixTransformVector(targetMatrix, *getTargetOffset());

        Vector targetStep = *getTargetPos() - lastTarget;
        long long t2 = (long long) dt * dt;
        long long t3 = t2 * dt;
        long long t4 = t3 * dt;
        double x4 = (double) t4;
        double x3 = (double) t3;
        double x2 = (double) t2;
        double x1 = (double) dt;
        double inverse = 1.0f / dt;
        targetStep *= (float) ((this->dampCoeffA[0] * x4 + this->dampCoeffA[1] * x3 + this->dampCoeffA[2] * x2 +
                                this->dampCoeffA[3] * x1 + this->dampCoeffA[4]) *
                               inverse);
        *getTargetPos() = lastTarget + targetStep;

        Vector positionStep = *getPosition() - lastPosition;
        positionStep *= (float) ((this->dampCoeffB[0] * x4 + this->dampCoeffB[1] * x3 + this->dampCoeffB[2] * x2 +
                                  this->dampCoeffB[3] * x1 + this->dampCoeffB[4]) *
                                 inverse);
        if (this->hideShip) {
            this->shakeAccum += VectorLength(positionStep);
            this->hideShip = this->shakeAccum < 800.0f;
        }
        *getPosition() = lastPosition + positionStep;
    }

    if (this->firstPerson) {
        *getPosition() = MatrixGetPosition(targetMatrix);
        *getUp() = MatrixGetUp(targetMatrix);
        *getTargetPos() = *getPosition() - MatrixGetDir(targetMatrix);
    }

    if (this->rumbleActive) {
        this->rumbleTimer -= dt;
        if (this->rumbleTimer < 1)
            this->rumbleActive = 0;
        float scale = this->firstPerson ? 0.001f : 1.0f;
        this->posX += scale * (float) (-this->rumbleStrength + Globals::rnd->nextInt(this->rumbleStrength * 2));
        this->posY += scale * (float) (-this->rumbleStrength + Globals::rnd->nextInt(this->rumbleStrength * 2));
        this->posZ += scale * (float) (-this->rumbleStrength + Globals::rnd->nextInt(this->rumbleStrength * 2));
    }

    if (this->shakeAmount > 0.0f) {
        int frequency = this->shakeFrequency;
        float scale = this->firstPerson ? 0.0015f : 1.0f;
        this->targetX += scale * (this->shakeAmount * (float) (Globals::rnd->nextInt(frequency * 2) - frequency));
        this->targetY += scale * (this->shakeAmount * (float) (Globals::rnd->nextInt(frequency * 2) - frequency));
        this->targetZ += scale * (this->shakeAmount * (float) (Globals::rnd->nextInt(frequency * 2) - frequency));
    }

    targetMatrix = MatrixGetLookAt(*getPosition(), *getTargetPos(), *getUp());
    Matrix roll;
    MatrixSetRotation(roll, 0.0f, 0.0f, this->rollAngle);
    targetMatrix *= roll;
    Globals::Canvas->CameraSetLocal(this->id, targetMatrix);
    this->localMatrix = targetMatrix;
}

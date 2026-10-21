#include "game/mission/Explosion.h"
#include "game/ship/TargetFollowCamera.h"
#include "engine/render/AEGeometry.h"
#include "engine/audio/FModSound.h"
#include "engine/math/Transform.h"
#include "engine/core/AERandom.h"
#include "engine/render/PaintCanvas.h"
#include "game/core/Globals.h"

using AbyssEngine::Matrix;
using AbyssEngine::Transform;

static inline void MatrixGetPosition(Vector *out, const Matrix *matrix) { *out = AbyssEngine::AEMath::MatrixGetPosition(*matrix); }

static PaintCanvas **Explosion_canvas = &Globals::Canvas;
static AbyssEngine::AERandom **Explosion_random = &Globals::rnd;

static inline PaintCanvas *explosionCanvas() {
    return *Explosion_canvas;
}

static inline AbyssEngine::AERandom *explosionRandom() {
    return *Explosion_random;
}

void Explosion::reset() {
    ((Transform *) Globals::Canvas->TransformGetTransform(this->primaryMesh->transform))->SetAnimationState(
        (AbyssEngine::AnimationMode) 3, 0);
    ((Transform *) Globals::Canvas->TransformGetTransform(this->primaryMesh->transform))->SetAnimationState(
        (AbyssEngine::AnimationMode) 1, 0);

    if (this->primaryMesh->childTransform != 0xffffffffu) {
        ((Transform *) Globals::Canvas->TransformGetTransform(this->primaryMesh->childTransform))->SetAnimationState(
            (AbyssEngine::AnimationMode) 3, 0);
        ((Transform *) Globals::Canvas->TransformGetTransform(this->primaryMesh->childTransform))->SetAnimationState(
            (AbyssEngine::AnimationMode) 1, 0);
    }

    if (this->secondaryMesh != 0) {
        ((Transform *) Globals::Canvas->TransformGetTransform(this->secondaryMesh->transform))->SetAnimationState(
            (AbyssEngine::AnimationMode) 3, 0);
        ((Transform *) Globals::Canvas->TransformGetTransform(this->secondaryMesh->transform))->SetAnimationState(
            (AbyssEngine::AnimationMode) 1, 0);
    }

    if (this->type == 6) {
        ((Transform *) Globals::Canvas->TransformGetTransform(this->primaryMesh->transform))->SetAnimationRangeInTime(
            0x8fcLL, 10000000LL);
        ((Transform *) Globals::Canvas->TransformGetTransform(this->secondaryMesh->transform))->SetAnimationRangeInTime(
            0x8fcLL, 10000000LL);
    }

    if (this->fireStreaks != 0) {
        for (uint32_t i = 0; i < this->fireStreaks->size(); i++) {
            AEGeometry *geometry = (*this->fireStreaks)[i];
            ((Transform *) Globals::Canvas->TransformGetTransform(geometry->transform))->SetAnimationState(
                (AbyssEngine::AnimationMode) 3, 0);
            ((Transform *) Globals::Canvas->TransformGetTransform(geometry->transform))->SetAnimationState(
                (AbyssEngine::AnimationMode) 1, 0);
        }
    }

    this->elapsed = 0;
    this->playing = 0;
}

Explosion::~Explosion() {
    if (this->primaryMesh != 0) {
        delete this->primaryMesh;
    }
    this->primaryMesh = 0;

    if (this->secondaryMesh != 0) {
        delete this->secondaryMesh;
    }
    this->secondaryMesh = 0;

    if (this->fireStreaks != 0) {
        ArrayReleaseClasses(*this->fireStreaks);
        delete this->fireStreaks;
    }
    this->fireStreaks = 0;
}

uint8_t Explosion::isPlaying() {
    return this->playing;
}

void Explosion::setWeaponIndex(int index) {
    this->weaponIndex = index;
}

void Explosion::translate(const Vector &v) {
    this->primaryMesh->translate(v);
    AEGeometry *secondary = this->secondaryMesh;
    if (secondary != 0) {
        return secondary->translate(v);
    }
}

void Explosion::setScaling(float scale) {
    this->scale = scale;
    this->primaryMesh->setScaling(scale, scale, scale);

    AEGeometry *secondary = this->secondaryMesh;
    if (secondary != 0) {
        secondary->setScaling(scale, scale, scale);
    }

    float speed = 1.0f;
    if (scale < 1.0f) {
        speed = (1.0f - scale) * 3.0f + 1.0f;
    }

    int type = this->type;
    if (type == 0xb) {
        speed = speed * 0.5f;
    }
    if ((uint32_t)(type - 8) < 3) {
        speed = 0.7f + (float) explosionRandom()->nextInt(0x3c) * 0.01f;
    }

    PaintCanvas **canvas = Explosion_canvas;
    ((Transform *) (*canvas)->TransformGetTransform(this->primaryMesh->transform))->SetAnimationSpeed(speed);

    secondary = this->secondaryMesh;
    if (secondary != 0) {
        ((Transform *) (*canvas)->TransformGetTransform(secondary->transform))->SetAnimationSpeed(speed);
    }

    if (this->fireStreaks != 0) {
        for (uint32_t i = 0; i < this->fireStreaks->size(); i++) {
            AEGeometry *geometry = (*this->fireStreaks)[i];
            ((Transform *) (*canvas)->TransformGetTransform(geometry->transform))->SetAnimationSpeed(speed);
        }
    }

    this->duration = (long long) ((float) this->duration / speed);
}

bool Explosion::peakReached() {
    return this->elapsed > 0x8fcLL;
}

void Explosion::start(const Vector &position, const Vector &direction) {
    this->primaryMesh->setPosition(position);
    ((Transform *) Globals::Canvas->TransformGetTransform(this->primaryMesh->transform))->animating = true;
    if (this->primaryMesh->childTransform != 0xffffffffu) {
        ((Transform *) Globals::Canvas->TransformGetTransform(this->primaryMesh->childTransform))->animating = true;
    }
    if (this->secondaryMesh != 0) {
        this->secondaryMesh->setPosition(position);
        ((Transform *) Globals::Canvas->TransformGetTransform(this->secondaryMesh->transform))->animating = true;
    }

    switch (this->type) {
        case 8:
        case 9:
        case 10:
            AbyssEngine::AEMath::MatrixSetRotation(this->rotation, 0.0f, 0.0f,
                                                   (float) Globals::rnd->nextInt(3141) / 1000.0f);
            this->setScaling(0.6f + (float) Globals::rnd->nextInt(40) * 0.01f);
            break;
        case 11:
            this->primaryMesh->setDirection(direction, Vector{0.0f, 1.0f, 0.0f});
            this->secondaryMesh->setDirection(direction, Vector{0.0f, 1.0f, 0.0f});
            break;
    }

    if (this->fireStreaks != 0) {
        for (uint32_t i = 0; i < this->fireStreaks->size(); i++) {
            AEGeometry *geometry = (*this->fireStreaks)[i];
            geometry->setPosition(position);
            ((Transform *) Globals::Canvas->TransformGetTransform(geometry->transform))->animating = true;
        }
    }

    this->playing = 1;
    Vector soundPosition = position;
    this->playSound(&soundPosition);
}

void Explosion::update(int dt, TargetFollowCamera *camera) {
    if (this->playing == 0) {
        return;
    }

    ((Transform *) Globals::Canvas->TransformGetTransform(this->primaryMesh->transform))->Update(dt, 0);
    if (this->primaryMesh->childTransform != 0xffffffffu) {
        ((Transform *) Globals::Canvas->TransformGetTransform(this->primaryMesh->childTransform))->Update(dt, 0);
    }
    if (this->secondaryMesh != 0) {
        ((Transform *) Globals::Canvas->TransformGetTransform(this->secondaryMesh->transform))->Update(dt, 0);
    }
    if (this->fireStreaks != 0) {
        for (uint32_t i = 0; i < this->fireStreaks->size(); i++) {
            ((Transform *) Globals::Canvas->TransformGetTransform((*this->fireStreaks)[i]->transform))->Update(dt, 0);
        }
    }

    if (camera != 0 && (uint32_t) this->type < 2) {
        float distance = AbyssEngine::AEMath::VectorLength(
            this->primaryMesh->getPosition() -
            AbyssEngine::AEMath::MatrixGetPosition(
                *(const Matrix *) Globals::Canvas->CameraGetLocal(Globals::Canvas->CameraGetCurrent())));

        int anim = (int) ((Transform *) Globals::Canvas->TransformGetTransform(this->primaryMesh->transform))->currentTime;
        if (anim <= 2000) {
            float capped = 30000.0f;
            if (distance < 30000.0f) {
                capped = distance;
            }
            camera->setRumblePercentage((1.0f - capped / 30000.0f) * ((float) anim / -2000.0f + 1.0f), 50);
        }
    }

    this->elapsed += dt;
    if (this->duration < this->elapsed) {
        this->reset();
        if (camera != 0) {
            camera->setRumblePercentage(0.0f, 0);
        }
    }
}

Explosion::Explosion(int type) {
    this->scale = 1.0f;
    this->type = type;
    this->primaryMesh = 0;
    this->secondaryMesh = 0;
    this->fireStreaks = 0;

    switch (type) {
        case 0:
        case 6: {
            this->primaryMesh = new AEGeometry((uint16_t) 0x41b5, Globals::Canvas, false);
            AEGeometry *geometry = new AEGeometry((uint16_t) 0x41b4, Globals::Canvas, false);
            this->primaryMesh->addChild(geometry->transform);
            delete geometry;
            break;
        }
        default:
            if (type == 13) {
                this->primaryMesh = new AEGeometry((uint16_t) 0x41a9, Globals::Canvas, false);
                this->setScaling(0.25f);
                ((Transform *) Globals::Canvas->TransformGetTransform(this->primaryMesh->transform))->SetAnimationSpeed(1.0f);
            } else {
                this->primaryMesh = new AEGeometry((uint16_t) 0x4213, Globals::Canvas, false);
                this->secondaryMesh = new AEGeometry((uint16_t) 0x4211, Globals::Canvas, false);
            }
            break;
        case 3:
            this->primaryMesh = new AEGeometry((uint16_t) 0x4213, Globals::Canvas, false);
            this->secondaryMesh = new AEGeometry((uint16_t) 0x421d, Globals::Canvas, false);
            break;
        case 4:
            this->primaryMesh = new AEGeometry((uint16_t) 0x420d, Globals::Canvas, false);
            this->secondaryMesh = new AEGeometry((uint16_t) 0x420c, Globals::Canvas, false);
            break;
        case 5:
            this->primaryMesh = new AEGeometry((uint16_t) 0x4999, Globals::Canvas, false);
            this->secondaryMesh = new AEGeometry((uint16_t) 0x4998, Globals::Canvas, false);
            break;
        case 7:
            this->primaryMesh = new AEGeometry((uint16_t) 0x41a5, Globals::Canvas, false);
            break;
        case 8:
            this->primaryMesh = new AEGeometry((uint16_t) 0x41a6, Globals::Canvas, false);
            break;
        case 9:
            this->primaryMesh = new AEGeometry((uint16_t) 0x41a7, Globals::Canvas, false);
            break;
        case 10:
            this->primaryMesh = new AEGeometry((uint16_t) 0x41a8, Globals::Canvas, false);
            break;
        case 11:
            this->primaryMesh = new AEGeometry((uint16_t) 0x4a34, Globals::Canvas, false);
            this->secondaryMesh = new AEGeometry((uint16_t) 0x4a33, Globals::Canvas, false);
            break;
        case 12:
            this->primaryMesh = new AEGeometry((uint16_t) 0x4a7e, Globals::Canvas, false);
            break;
    }

    ((Transform *) Globals::Canvas->TransformGetTransform(this->primaryMesh->transform))->SetAnimationState(
        (AbyssEngine::AnimationMode) 1, 0);

    if (this->secondaryMesh != 0) {
        ((Transform *) Globals::Canvas->TransformGetTransform(this->secondaryMesh->transform))->SetAnimationState(
            (AbyssEngine::AnimationMode) 1, 0);
        ((Transform *) Globals::Canvas->TransformGetTransform(this->secondaryMesh->transform))->boundingRadius = 10000.0f;
    }

    long long primaryLength =
        ((Transform *) Globals::Canvas->TransformGetTransform(this->primaryMesh->transform))->animationLength;
    long long secondaryLength = 0;
    if (this->secondaryMesh != 0) {
        secondaryLength =
            ((Transform *) Globals::Canvas->TransformGetTransform(this->secondaryMesh->transform))->animationLength;
    }
    if (secondaryLength < primaryLength) {
        this->duration =
            ((Transform *) Globals::Canvas->TransformGetTransform(this->primaryMesh->transform))->animationLength;
    } else if (this->secondaryMesh != 0) {
        this->duration =
            ((Transform *) Globals::Canvas->TransformGetTransform(this->secondaryMesh->transform))->animationLength;
    } else {
        this->duration = 0;
    }

    ((Transform *) Globals::Canvas->TransformGetTransform(this->primaryMesh->transform))->boundingRadius = 10000.0f;
    this->weaponIndex = -1;
    this->reset();
}

void Explosion::playSound(Vector *pos) {
    int cue;
    if (this->weaponIndex >= 0) {
        switch (this->weaponIndex) {
            case 41: cue = 15; break;
            case 42: cue = 16; break;
            case 43: cue = 17; break;
            case 44: cue = 14; break;
            case 45: cue = 13; break;
            case 46: cue = 12; break;
            case 60:
            case 61:
            case 62:
            case 176:
            case 177:
            case 178:
                cue = 22;
                break;
            case 179: cue = 12; break;
            case 197:
            case 221:
                cue = 2253;
                break;
            case 232: cue = 2279; break;
            default:
                return;
        }
        Globals::sound->play(cue, Globals::options[0xf] != 0 ? pos : 0, 0, 0.0f);
    } else {
        switch (this->type) {
            case 2:
            case 3:
            case 4:
            case 5:
                Globals::sound->play(21, Globals::options[0xf] != 0 ? pos : 0, 0, 0.0f);
                break;
            case 0:
                Globals::sound->play(Globals::rnd->nextInt(2) == 0 ? 18 : 19,
                                     Globals::options[0xf] != 0 ? pos : 0, 0, 0.0f);
                break;
        }
    }
}

void Explosion::start(const Matrix &matrix) {
    if (this->type == 6 || this->type == 0) {
        this->primaryMesh->setPosition(AbyssEngine::AEMath::MatrixGetPosition(matrix));
        this->secondaryMesh->setPosition(AbyssEngine::AEMath::MatrixGetPosition(matrix));
    } else {
        this->primaryMesh->setMatrix(matrix);
        if (this->secondaryMesh != 0) {
            this->secondaryMesh->setMatrix(matrix);
        }
    }

    if (this->fireStreaks != 0) {
        for (uint32_t i = 0; i < this->fireStreaks->size(); i++) {
            AEGeometry *geometry = (*this->fireStreaks)[i];
            geometry->setPosition(AbyssEngine::AEMath::MatrixGetPosition(matrix));
            ((Transform *) Globals::Canvas->TransformGetTransform(geometry->transform))->animating = true;
        }
    }

    ((Transform *) Globals::Canvas->TransformGetTransform(this->primaryMesh->transform))->animating = true;
    if (this->primaryMesh->childTransform != 0xffffffffu) {
        ((Transform *) Globals::Canvas->TransformGetTransform(this->primaryMesh->childTransform))->animating = true;
    }
    if (this->secondaryMesh != 0) {
        ((Transform *) Globals::Canvas->TransformGetTransform(this->secondaryMesh->transform))->animating = true;
    }

    this->playing = 1;
    Vector position = AbyssEngine::AEMath::MatrixGetPosition(matrix);
    this->playSound(&position);
}

void Explosion::render() {
    if (this->playing == 0) {
        return;
    }

    switch (this->type) {
        case 7:
        case 8:
        case 9:
        case 10:
        case 13:
            break;
        default:
            if (this->secondaryMesh != 0) {
                this->secondaryMesh->render();
            }
            break;
    }

    Matrix matrix = *(const Matrix *) Globals::Canvas->CameraGetLocal(Globals::Canvas->CameraGetCurrent());
    Vector position = this->primaryMesh->getPosition();

    if ((uint32_t) this->type <= 12 && ((1u << this->type) & 0x1804u) != 0) {
        AbyssEngine::AEMath::MatrixSetTranslation(matrix, position.x, position.y, position.z);
    } else {
        Vector cameraPosition = AbyssEngine::AEMath::MatrixGetPosition(matrix);
        Vector up = AbyssEngine::AEMath::MatrixGetUp(matrix);
        matrix = AbyssEngine::AEMath::MatrixGetLookAt(cameraPosition, position, up);
    }

    AbyssEngine::AEMath::MatrixSetScaling(matrix, this->scale, this->scale, this->scale);
    if ((uint32_t) (this->type - 8) <= 2) {
        matrix *= this->rotation;
    }

    this->primaryMesh->setMatrix(matrix);
    this->primaryMesh->setPosition(position);

    matrix = *(const Matrix *) Globals::Canvas->CameraGetLocal(Globals::Canvas->CameraGetCurrent());
    this->primaryMesh->setDirection(AbyssEngine::AEMath::MatrixGetDir(matrix),
                                    AbyssEngine::AEMath::MatrixGetUp(matrix));
    this->primaryMesh->render();

    if (this->fireStreaks != 0) {
        for (uint32_t i = 0; i < this->fireStreaks->size(); i++) {
            (*this->fireStreaks)[i]->render();
        }
    }
}

void Explosion::update(int dt, const Vector &position) {
    if (this->playing == 0) {
        return;
    }

    ((Transform *) Globals::Canvas->TransformGetTransform(this->primaryMesh->transform))->Update(dt, 0);
    if (this->primaryMesh->childTransform != 0xffffffffu) {
        ((Transform *) Globals::Canvas->TransformGetTransform(this->primaryMesh->childTransform))->Update(dt, 0);
    }
    if (this->secondaryMesh != 0) {
        ((Transform *) Globals::Canvas->TransformGetTransform(this->secondaryMesh->transform))->Update(dt, 0);
    }
    if (this->fireStreaks != 0) {
        for (uint32_t i = 0; i < this->fireStreaks->size(); i++) {
            AEGeometry *geometry = (*this->fireStreaks)[i];
            geometry->setPosition(position);
            ((Transform *) Globals::Canvas->TransformGetTransform(geometry->transform))->Update(dt, 0);
        }
    }

    this->elapsed += dt;
    if (this->duration < this->elapsed) {
        this->reset();
    }
}

void Explosion::addFireStreaks() {
    if (this->fireStreaks != 0) {
        return;
    }

    this->fireStreaks = new Array<AEGeometry *>();
    ArraySetLength(Globals::rnd->nextInt(7) + 3, *this->fireStreaks);

    for (uint32_t i = 0; i < this->fireStreaks->size(); i++) {
        (*this->fireStreaks)[i] = new AEGeometry((uint16_t) 0x37d4, Globals::Canvas, false);
        ((Transform *) Globals::Canvas->TransformGetTransform((*this->fireStreaks)[i]->transform))->SetAnimationState(
            (AbyssEngine::AnimationMode) 1, 0);
        ((Transform *) Globals::Canvas->TransformGetTransform((*this->fireStreaks)[i]->transform))->boundingRadius =
            10000.0f;
        (*this->fireStreaks)[i]->setRotation((float) Globals::rnd->nextInt(360) / 180.0f * 3.1415927f,
                                             (float) Globals::rnd->nextInt(360) / 180.0f * 3.1415927f,
                                             (float) Globals::rnd->nextInt(360) / 180.0f * 3.1415927f);
        float scale = (float) (Globals::rnd->nextInt(50) + 50) / 100.0f;
        (*this->fireStreaks)[i]->setScaling(scale, scale, scale);
    }
}

#include "gof2/Explosion.h"
#include "gof2/AEGeometry.h"
#include "gof2/FModSound.h"
#include "gof2/Transform.h"


extern "C" uint32_t PaintCanvas_TransformGetTransform32(int canvas, int transform);
extern "C" long long PaintCanvas_TransformGetTransform(int canvas, int transform);
extern "C" void *AEGeometry_dtor(AEGeometry *self);
extern "C" void operator_delete(void *ptr);
extern "C" void ArrayReleaseClasses_AEGeometryPtr(Array<AEGeometry *> *self);
extern "C" void *Array_AEGeometryPtr_dtor(Array<AEGeometry *> *self);
extern "C" void AEGeometry_translate_vec(AEGeometry *self, const Vector *v);
extern "C" void Explosion_tail_translate(void *geometry, const Vector *v);
extern "C" void AEGeometry_setScaling3(AEGeometry *self, float x, float y, float z);
extern "C" int AERandom_nextInt(void *self, int bound);
void MatrixSetRotation(Matrix *out, Matrix *base, int zero1, int zero2, float angle);
extern "C" void Transform_Update32(uint32_t transform, uint32_t high, long long elapsed, uint32_t zero);
extern "C" int PaintCanvas_CameraGetCurrent(int canvas);
extern "C" void *PaintCanvas_CameraGetLocal(int canvas, int current);
extern "C" void Vector_sub(Vector *out, const Vector *a, const Vector *b);
float VectorLength(const Vector *self);
extern "C" void TargetFollowCamera_setRumblePercentage(TargetFollowCamera *self, float value, int duration);
extern "C" void Matrix_ctor(Matrix *self);
extern "C" void *operator_new(uint32_t size);
extern "C" void AEGeometry_ctor(AEGeometry *self, uint16_t mesh, int canvas, bool flag);
void MatrixGetPosition(Vector *out, const Matrix *matrix);
extern "C" void AEGeometry_setMatrix(AEGeometry *self, const Matrix *matrix);
extern "C" void *__aeabi_memcpy(void *dst, const void *src, uint32_t n);
void MatrixSetTranslation(Matrix *out, Matrix *base, float x, float y, float z);
extern "C" void MatrixSetScaling(Matrix *out, Matrix *base, float x, float y, float z);
void MatrixGetUp(Vector *out, const Matrix *matrix);
void MatrixGetDir(Vector *out, const Matrix *matrix);
void MatrixGetLookAt(Matrix *out, const Vector *position, const Vector *target, const Vector *up);
extern "C" void Matrix_assign(Matrix *self, const Matrix *other);
extern "C" void Matrix_mul_assign(Matrix *self, const Matrix *other);
extern "C" void Explosion_reset_tail(Explosion *self);
extern "C" void Array_AEGeometryPtr_ctor(Array<AEGeometry *> *self);
extern "C" void ArraySetLength_AEGeometryPtr(int length, Array<AEGeometry *> *self);
extern "C" void AEGeometry_setRotation3(AEGeometry *self, float x, float y, float z);

// ---- reset_a7fb0.cpp ----
extern int Explosion_paintCanvas;


void Explosion::reset() {
    Explosion *self = this;
    int *canvas = &Explosion_paintCanvas;

    ((AbyssEngine::Transform *)(PaintCanvas_TransformGetTransform32(*canvas, I(self->primaryMesh, 0xc))))->SetAnimationState((AbyssEngine::AnimationMode)3, 0);
    ((AbyssEngine::Transform *)(PaintCanvas_TransformGetTransform32(*canvas, I(self->primaryMesh, 0xc))))->SetAnimationState((AbyssEngine::AnimationMode)1, 0);

    int lodTransform = I(self->primaryMesh, 0x14);
    if (lodTransform != -1) {
        ((AbyssEngine::Transform *)(PaintCanvas_TransformGetTransform32(*canvas, lodTransform)))->SetAnimationState((AbyssEngine::AnimationMode)3, 0);
        ((AbyssEngine::Transform *)(PaintCanvas_TransformGetTransform32(*canvas, I(self->primaryMesh, 0x14))))->SetAnimationState((AbyssEngine::AnimationMode)1, 0);
    }

    AEGeometry *secondary = self->secondaryMesh;
    if (secondary != 0) {
        ((AbyssEngine::Transform *)(PaintCanvas_TransformGetTransform32(*canvas, I(secondary, 0xc))))->SetAnimationState((AbyssEngine::AnimationMode)3, 0);
        ((AbyssEngine::Transform *)(PaintCanvas_TransformGetTransform32(*canvas, I(self->secondaryMesh, 0xc))))->SetAnimationState((AbyssEngine::AnimationMode)1, 0);
    }

    if (self->type == 6) {
        ((AbyssEngine::Transform *)(PaintCanvas_TransformGetTransform(*canvas, I(self->primaryMesh, 0xc))))->SetAnimationRangeInTime(0x8fcLL, 10000000LL);
        ((AbyssEngine::Transform *)(PaintCanvas_TransformGetTransform(*canvas, I(self->secondaryMesh, 0xc))))->SetAnimationRangeInTime(0x8fcLL, 10000000LL);
    }

    Array<AEGeometry *> *streaks = self->fireStreaks;
    if (streaks != 0) {
        for (uint32_t i = 0; i < streaks->size(); i++) {
            AEGeometry *geometry = (*streaks)[i];
            ((AbyssEngine::Transform *)(PaintCanvas_TransformGetTransform32(*canvas, I(geometry, 0xc))))->SetAnimationState((AbyssEngine::AnimationMode)3, 0);
            ((AbyssEngine::Transform *)(PaintCanvas_TransformGetTransform32(*canvas, I(geometry, 0xc))))->SetAnimationState((AbyssEngine::AnimationMode)1, 0);
            streaks = self->fireStreaks;
        }
    }

    self->elapsed = 0;
    self->playing = 0;
}

// ---- _Explosion_a80d8.cpp ----
Explosion *_ZN9ExplosionD2Ev(Explosion *self)
{
    AEGeometry *geometry = self->primaryMesh;
    if (geometry != 0) {
        operator_delete(AEGeometry_dtor(geometry));
    }
    self->primaryMesh = 0;

    geometry = self->secondaryMesh;
    if (geometry != 0) {
        operator_delete(AEGeometry_dtor(geometry));
    }
    self->secondaryMesh = 0;

    Array<AEGeometry *> *streaks = self->fireStreaks;
    if (streaks != 0) {
        ArrayReleaseClasses_AEGeometryPtr(streaks);
        streaks = self->fireStreaks;
        if (streaks != 0) {
            operator_delete(Array_AEGeometryPtr_dtor(streaks));
        }
    }
    self->fireStreaks = 0;
    return self;
}

// ---- isPlaying_a8916.cpp ----
uint8_t Explosion::isPlaying() {
    Explosion *self = this;
    return self->playing;
}

// ---- translate_a8aa0.cpp ----
void Explosion::translate(const Vector *v) {
    Explosion *self = this;
    AEGeometry_translate_vec(self->primaryMesh, v);
    void *secondary = self->secondaryMesh;
    if (secondary != 0) {
        return Explosion_tail_translate(secondary, v);
    }
}

// ---- setScaling_a7e90.cpp ----
extern int Explosion_paintCanvas;
extern void *Explosion_random;


void Explosion::setScaling(float scale) {
    Explosion *self = this;
    self->scale = scale;
    AEGeometry_setScaling3(self->primaryMesh, scale, scale, scale);

    AEGeometry *secondary = self->secondaryMesh;
    if (secondary != 0) {
        AEGeometry_setScaling3(secondary, scale, scale, scale);
    }

    float speed = 1.0f;
    if (scale < 1.0f) {
        speed = (1.0f - scale) * 3.0f + 1.0f;
    }

    int type = self->type;
    if (type == 0xb) {
        speed = speed * 0.5f;
    }
    if ((uint32_t)(type - 8) < 3) {
        speed = 0.7f + (float)AERandom_nextInt(Explosion_random, 0x3c) * 0.01f;
    }

    int *canvas = &Explosion_paintCanvas;
    ((AbyssEngine::Transform *)(PaintCanvas_TransformGetTransform32(*canvas, I(self->primaryMesh, 0xc))))->SetAnimationSpeed(speed);

    secondary = self->secondaryMesh;
    if (secondary != 0) {
        ((AbyssEngine::Transform *)(PaintCanvas_TransformGetTransform32(*canvas, I(secondary, 0xc))))->SetAnimationSpeed(speed);
    }

    Array<AEGeometry *> *streaks = self->fireStreaks;
    if (streaks != 0) {
        for (uint32_t i = 0; i < streaks->size(); i++) {
            AEGeometry *geometry = (*streaks)[i];
            ((AbyssEngine::Transform *)(PaintCanvas_TransformGetTransform32(*canvas, I(geometry, 0xc))))->SetAnimationSpeed(speed);
            streaks = self->fireStreaks;
        }
    }

    self->duration = (long long)((float)self->duration / speed);
}

// ---- peakReached_a8900.cpp ----
bool Explosion::peakReached() {
    Explosion *self = this;
    return self->elapsed > 0x8fcLL;
}

// ---- start_a83b0.cpp ----
extern int Explosion_paintCanvas;
extern void *Explosion_random;


void Explosion::start(const Vector *position, const Vector *direction) {
    Explosion *self = this;
    ((AEGeometry *)(self->primaryMesh))->setPosition(*position);

    int *canvas = &Explosion_paintCanvas;
    UC((void *)PaintCanvas_TransformGetTransform32(*canvas, I(self->primaryMesh, 0xc)), 0xed) = 1;

    int lodTransform = I(self->primaryMesh, 0x14);
    if (lodTransform != -1) {
        UC((void *)PaintCanvas_TransformGetTransform32(*canvas, lodTransform), 0xed) = 1;
    }

    AEGeometry *secondary = self->secondaryMesh;
    if (secondary != 0) {
        ((AEGeometry *)(secondary))->setPosition(*position);
        UC((void *)PaintCanvas_TransformGetTransform32(*canvas, I(self->secondaryMesh, 0xc)), 0xed) = 1;
    }

    int type = self->type;
    if ((uint32_t)(type - 8) < 3) {
        Matrix rotation;
        float angle = (float)AERandom_nextInt(Explosion_random, 0xc45) / 1000.0f;
        MatrixSetRotation(&rotation, &self->rotation, 0, 0, angle);

        float scale = 0.6f + (float)AERandom_nextInt(Explosion_random, 0x28) * 0.01f;
        ((Explosion *)(self))->setScaling(scale);
    } else if (type == 0xb) {
        Vector up;
        up.x = 0.0f;
        up.y = 1.0f;
        up.z = 0.0f;
        ((AEGeometry *)(self->primaryMesh))->setDirection(*direction, up);
        up.x = 0.0f;
        up.y = 1.0f;
        up.z = 0.0f;
        ((AEGeometry *)(self->secondaryMesh))->setDirection(*direction, up);
    }

    Array<AEGeometry *> *streaks = self->fireStreaks;
    if (streaks != 0) {
        for (uint32_t i = 0; i < streaks->size(); i++) {
            AEGeometry *geometry = (*streaks)[i];
            ((AEGeometry *)(geometry))->setPosition(*position);
            UC((void *)PaintCanvas_TransformGetTransform32(*canvas, I(geometry, 0xc)), 0xed) = 1;
            streaks = self->fireStreaks;
        }
    }

    self->playing = 1;
    Vector soundPosition = *position;
    ((Explosion *)(self))->playSound(&soundPosition);
}

// ---- update_a8668.cpp ----
extern int Explosion_paintCanvas;


void Explosion::update_camera(int dt, TargetFollowCamera *camera) {
    Explosion *self = this;
    if (self->playing == 0) {
        return;
    }

    int *canvas = &Explosion_paintCanvas;
    long long delta = (long long)dt;
    ((AbyssEngine::Transform *)(PaintCanvas_TransformGetTransform(*canvas, I(self->primaryMesh, 0xc))))->Update(delta, 0);

    int lodTransform = I(self->primaryMesh, 0x14);
    if (lodTransform != -1) {
        ((AbyssEngine::Transform *)(PaintCanvas_TransformGetTransform(*canvas, lodTransform)))->Update(delta, 0);
    }

    AEGeometry *secondary = self->secondaryMesh;
    if (secondary != 0) {
        Transform_Update32(PaintCanvas_TransformGetTransform32(*canvas, I(secondary, 0xc)), 0, delta, 0);
    }

    Array<AEGeometry *> *streaks = self->fireStreaks;
    if (streaks != 0) {
        for (uint32_t i = 0; i < streaks->size(); i++) {
            AEGeometry *geometry = (*streaks)[i];
            ((AbyssEngine::Transform *)(PaintCanvas_TransformGetTransform(*canvas, I(geometry, 0xc))))->Update(delta, 0);
            streaks = self->fireStreaks;
        }
    }

    if (camera != 0 && (uint32_t)self->type < 2) {
        char vectors[36];
        Vector *cameraPosition = (Vector *)(vectors + 0);
        Vector *position = (Vector *)(vectors + 12);
        Vector *diff = (Vector *)(vectors + 24);
        ((AEGeometry *)(position))->getPosition();

        int canvasValue = *canvas;
        int current = PaintCanvas_CameraGetCurrent(canvasValue);
        MatrixGetPosition(cameraPosition, (const Matrix *)PaintCanvas_CameraGetLocal(canvasValue, current));
        Vector_sub(diff, position, cameraPosition);
        float distance = VectorLength(diff);

        uint32_t transform = PaintCanvas_TransformGetTransform32(*canvas, I(self->primaryMesh, 0xc));
        int anim = I((void *)transform, 0x110);
        if (anim <= 0x7d0) {
            float capped = 30000.0f;
            if (distance < 30000.0f) {
                capped = distance;
            }
            float value = (1.0f - capped / 30000.0f) * ((float)anim / -2000.0f + 1.0f);
            TargetFollowCamera_setRumblePercentage(camera, value, 0x32);
        }
    }

    long long elapsed = self->elapsed + delta;
    self->elapsed = elapsed;
    if (self->duration < elapsed) {
        ((Explosion *)(self))->reset();
        if (camera != 0) {
            TargetFollowCamera_setRumblePercentage(camera, 0.0f, 0);
        }
    }
}

// ---- Explosion_a7af4.cpp ----
extern int Explosion_paintCanvas;


static inline __attribute__((always_inline)) AEGeometry *make_geometry(uint16_t mesh, int canvas)
{
    AEGeometry *geometry = (AEGeometry *)operator_new(0xc0);
    AEGeometry_ctor(geometry, mesh, canvas, false);
    return geometry;
}

Explosion *_ZN9ExplosionC2Ei(Explosion *self, int type)
{
    Matrix_ctor(&self->rotation);
    self->scale = 1.0f;
    self->type = type;
    self->primaryMesh = 0;
    self->secondaryMesh = 0;
    self->fireStreaks = 0;

    int *canvas = &Explosion_paintCanvas;
    AEGeometry *geometry;

    switch (type) {
    case 0:
    case 6:
        self->primaryMesh = make_geometry(0x41b5, *canvas);
        geometry = make_geometry(0x41b4, *canvas);
        ((AEGeometry *)(self->primaryMesh))->addChild(U(geometry, 0xc));
        operator_delete(AEGeometry_dtor(geometry));
        break;
    default:
        if (type == 13) {
            self->primaryMesh = make_geometry(0x41a9, *canvas);
            ((Explosion *)(self))->setScaling(0.25f);
            ((AbyssEngine::Transform *)(PaintCanvas_TransformGetTransform32(*canvas, I(self->primaryMesh, 0xc))))->SetAnimationSpeed(1.0f);
        } else {
            self->primaryMesh = make_geometry(0x4213, *canvas);
            self->secondaryMesh = make_geometry(0x4211, *canvas);
        }
        break;
    case 3:
        self->primaryMesh = make_geometry(0x4213, *canvas);
        self->secondaryMesh = make_geometry(0x421d, *canvas);
        break;
    case 4:
        self->primaryMesh = make_geometry(0x420d, *canvas);
        self->secondaryMesh = make_geometry(0x420c, *canvas);
        break;
    case 5:
        self->primaryMesh = make_geometry(0x4999, *canvas);
        self->secondaryMesh = make_geometry(0x4998, *canvas);
        break;
    case 7:
        self->primaryMesh = make_geometry(0x41a5, *canvas);
        break;
    case 8:
        self->primaryMesh = make_geometry(0x41a6, *canvas);
        break;
    case 9:
        self->primaryMesh = make_geometry(0x41a7, *canvas);
        break;
    case 10:
        self->primaryMesh = make_geometry(0x41a8, *canvas);
        break;
    case 11:
        self->primaryMesh = make_geometry(0x4a34, *canvas);
        self->secondaryMesh = make_geometry(0x4a33, *canvas);
        break;
    case 12:
        self->primaryMesh = make_geometry(0x4a7e, *canvas);
        break;
    }

    ((AbyssEngine::Transform *)(PaintCanvas_TransformGetTransform32(*canvas, I(self->primaryMesh, 0xc))))->SetAnimationState((AbyssEngine::AnimationMode)1, 0);

    if (self->secondaryMesh != 0) {
        ((AbyssEngine::Transform *)(PaintCanvas_TransformGetTransform32(*canvas, I(self->secondaryMesh, 0xc))))->SetAnimationState((AbyssEngine::AnimationMode)1, 0);
        F<float>((void *)PaintCanvas_TransformGetTransform32(*canvas, I(self->secondaryMesh, 0xc)), 0xe0) =
            10000.0f;
    }

    uint64_t primaryDuration =
        *(uint64_t *)((char *)(void *)PaintCanvas_TransformGetTransform32(*canvas, I(self->primaryMesh, 0xc)) + 0xf8);
    uint64_t duration = 0;
    if (self->secondaryMesh != 0) {
        uint64_t secondaryDuration =
            *(uint64_t *)((char *)(void *)PaintCanvas_TransformGetTransform32(*canvas, I(self->secondaryMesh, 0xc)) + 0xf8);
        duration = secondaryDuration < primaryDuration ? primaryDuration : secondaryDuration;
    } else if (primaryDuration != 0) {
        duration = primaryDuration;
    }
    self->duration = duration;

    F<float>((void *)PaintCanvas_TransformGetTransform32(*canvas, I(self->primaryMesh, 0xc)), 0xe0) = 10000.0f;
    self->weaponIndex = -1;
    ((Explosion *)(self))->reset();
    return self;
}

// ---- playSound_a82a8.cpp ----
extern int Explosion_soundDefault;
extern int Explosion_soundFallback;
extern int Explosion_soundSpecial;
extern char Explosion_soundSettings[];
extern void *Explosion_random;


void Explosion::playSound(Vector *pos) {
    Explosion *self = this;
    Vector *soundPos = pos;
    int soundId = self->weaponIndex;
    int cue;
    int sound;
    int enabled;

    if (soundId >= 0) {
        uint32_t offset = (uint32_t)(soundId - 0x29);
        if (offset <= 0x15) {
            cue = 0xf;
            switch (soundId) {
            case 0x29:
                break;
            case 0x2a:
                cue = 0x10;
                break;
            case 0x2b:
                cue = 0x11;
                break;
            case 0x2c:
                cue = 0xe;
                break;
            case 0x2d:
                cue = 0xd;
                break;
            case 0x2e:
                cue = 0xc;
                break;
            case 0x2f:
            case 0x30:
            case 0x31:
            case 0x32:
            case 0x33:
            case 0x34:
            case 0x35:
            case 0x36:
            case 0x37:
            case 0x38:
            case 0x39:
            case 0x3a:
            case 0x3b:
                return;
            case 0x3c:
            case 0x3d:
            case 0x3e:
                cue = 0x16;
                break;
            }
        } else {
            offset = (uint32_t)(soundId - 0xb0);
            if (offset < 3) {
                cue = 0x16;
            } else if (soundId == 0xb3) {
                cue = 0xc;
            } else if (soundId == 0xc5 || soundId == 0xdd) {
                cue = 0x8cd;
            } else {
                if (soundId != 0xe8) {
                    return;
                }
                cue = 0x8e7;
            }
        }
        sound = Explosion_soundDefault;
        enabled = Explosion_soundSettings[0xf];
    } else {
        int type = self->type;
        uint32_t typeOffset = (uint32_t)(type - 2);
        if (typeOffset < 4) {
            sound = Explosion_soundSpecial;
            enabled = Explosion_soundSettings[0xf];
            if (enabled == 0) {
                soundPos = 0;
            }
            ((FModSound *)(sound))->play(0x15, soundPos, 0, 0);
            return;
        }
        if (type != 0) {
            return;
        }
        sound = Explosion_soundFallback;
        int random = AERandom_nextInt(Explosion_random, 2);
        enabled = Explosion_soundSettings[0xf];
        cue = 0x13;
        if (random == 0) {
            cue = 0x12;
        }
    }

    if (enabled == 0) {
        soundPos = 0;
    }
    ((FModSound *)(sound))->play(cue, soundPos, 0, 0);
}

// ---- start_a853c.cpp ----
extern int Explosion_paintCanvas;


void Explosion::start_matrix(const Matrix *matrix) {
    Explosion *self = this;
    char positionStorage[12];
    Vector *position = (Vector *)positionStorage;

    int type = self->type;
    if (type == 6 || type == 0) {
        MatrixGetPosition(position, matrix);
        ((AEGeometry *)(self->primaryMesh))->setPosition(*position);
        MatrixGetPosition(position, matrix);
        ((AEGeometry *)(self->secondaryMesh))->setPosition(*position);
    } else {
        AEGeometry_setMatrix(self->primaryMesh, matrix);
        AEGeometry *secondary = self->secondaryMesh;
        if (secondary != 0) {
            AEGeometry_setMatrix(secondary, matrix);
        }
    }

    int *canvas = &Explosion_paintCanvas;
    Array<AEGeometry *> *streaks = self->fireStreaks;
    if (streaks != 0) {
        for (uint32_t i = 0; i < streaks->size(); i++) {
            AEGeometry *geometry = (*streaks)[i];
            MatrixGetPosition(position, matrix);
            ((AEGeometry *)(geometry))->setPosition(*position);
            UC((void *)PaintCanvas_TransformGetTransform32(*canvas, I(geometry, 0xc)), 0xed) = 1;
            streaks = self->fireStreaks;
        }
    }

    UC((void *)PaintCanvas_TransformGetTransform32(*canvas, I(self->primaryMesh, 0xc)), 0xed) = 1;
    int lodTransform = I(self->primaryMesh, 0x14);
    if (lodTransform != -1) {
        UC((void *)PaintCanvas_TransformGetTransform32(*canvas, lodTransform), 0xed) = 1;
    }
    AEGeometry *secondary = self->secondaryMesh;
    if (secondary != 0) {
        UC((void *)PaintCanvas_TransformGetTransform32(*canvas, I(secondary, 0xc)), 0xed) = 1;
    }

    self->playing = 1;
    MatrixGetPosition(position, matrix);
    ((Explosion *)(self))->playSound(position);
}

// ---- render_a891c.cpp ----
extern int Explosion_paintCanvas;


void Explosion::render() {
    Explosion *self = this;
    Matrix cameraLocal;
    Matrix work;
    Vector position;
    Vector cameraPosition;
    Vector up;

    if (self->playing != 0) {
        uint32_t type = (uint32_t)self->type;
        if ((type > 0xd || ((1u << (type & 0xff)) & 0x2780u) == 0) && self->secondaryMesh != 0) {
            ((AEGeometry *)(self->secondaryMesh))->render();
        }

        int *canvas = &Explosion_paintCanvas;
        int canvasValue = *canvas;
        int current = PaintCanvas_CameraGetCurrent(canvasValue);
        __aeabi_memcpy(&cameraLocal, PaintCanvas_CameraGetLocal(canvasValue, current), 0x3c);

        ((AEGeometry *)(&position))->getPosition();

        if (type < 0xd && ((1u << (type & 0xff)) & 0x1804u) != 0) {
            MatrixSetTranslation(&work, &cameraLocal, position.x, position.y, position.z);
        } else {
            MatrixGetPosition(&cameraPosition, &cameraLocal);
            MatrixGetUp(&up, &cameraLocal);
            MatrixGetLookAt(&work, &cameraPosition, &position, &up);
            Matrix_assign(&cameraLocal, &work);
        }

        float scale = self->scale;
        MatrixSetScaling(&work, &cameraLocal, scale, scale, scale);

        if (type - 8 < 3) {
            Matrix_mul_assign(&cameraLocal, &self->rotation);
        }

        AEGeometry_setMatrix(self->primaryMesh, &cameraLocal);
        ((AEGeometry *)(self->primaryMesh))->setPosition(position);

        canvasValue = *canvas;
        current = PaintCanvas_CameraGetCurrent(canvasValue);
        Matrix_assign(&cameraLocal, (Matrix *)PaintCanvas_CameraGetLocal(canvasValue, current));
        Vector *direction = (Vector *)&work;
        MatrixGetDir(direction, &cameraLocal);
        MatrixGetUp(&cameraPosition, &cameraLocal);
        ((AEGeometry *)(self->primaryMesh))->setDirection(*direction, cameraPosition);
        ((AEGeometry *)(self->primaryMesh))->render();

        Array<AEGeometry *> *streaks = self->fireStreaks;
        if (streaks != 0) {
            for (uint32_t i = 0; i < streaks->size(); i++) {
                AEGeometry *geometry = (*streaks)[i];
                ((AEGeometry *)(geometry))->render();
                streaks = self->fireStreaks;
            }
        }
    }
}

// ---- update_a8820.cpp ----
extern int Explosion_paintCanvas;


void Explosion::update_vector(int dt, const Vector *position) {
    Explosion *self = this;
    if (self->playing == 0) {
        return;
    }

    int *canvas = &Explosion_paintCanvas;
    long long delta = (long long)dt;
    ((AbyssEngine::Transform *)(PaintCanvas_TransformGetTransform(*canvas, I(self->primaryMesh, 0xc))))->Update(delta, 0);

    int lodTransform = I(self->primaryMesh, 0x14);
    if (lodTransform != -1) {
        ((AbyssEngine::Transform *)(PaintCanvas_TransformGetTransform(*canvas, lodTransform)))->Update(delta, 0);
    }

    AEGeometry *secondary = self->secondaryMesh;
    if (secondary != 0) {
        Transform_Update32(PaintCanvas_TransformGetTransform32(*canvas, I(secondary, 0xc)), 0, delta, 0);
    }

    Array<AEGeometry *> *streaks = self->fireStreaks;
    if (streaks != 0) {
        for (uint32_t i = 0; i < streaks->size(); i++) {
            AEGeometry *geometry = (*streaks)[i];
            ((AEGeometry *)(geometry))->setPosition(*position);
            ((AbyssEngine::Transform *)(PaintCanvas_TransformGetTransform(*canvas, I(geometry, 0xc))))->Update(delta, 0);
            streaks = self->fireStreaks;
        }
    }

    long long elapsed = self->elapsed + delta;
    self->elapsed = elapsed;
    if (self->duration < elapsed) {
        return Explosion_reset_tail(self);
    }
}

// ---- addFireStreaks_a811c.cpp ----
extern int Explosion_paintCanvas;
extern void *Explosion_random;


void Explosion::addFireStreaks() {
    Explosion *self = this;
    if (self->fireStreaks != 0) {
        return;
    }

    Array<AEGeometry *> *streaks = new Array<AEGeometry *>();
    self->fireStreaks = streaks;

    int length = AERandom_nextInt(Explosion_random, 7) + 3;
    ArraySetLength_AEGeometryPtr(length, self->fireStreaks);

    int *canvas = &Explosion_paintCanvas;
    int (*nextInt)(void *, int) = AERandom_nextInt;

    for (uint32_t i = 0; i < self->fireStreaks->size(); i++) {
        AEGeometry *geometry = (AEGeometry *)operator_new(0xc0);
        AEGeometry_ctor(geometry, 0x37d4, *canvas, false);
        (*self->fireStreaks)[i] = geometry;

        geometry = (*self->fireStreaks)[i];
        ((AbyssEngine::Transform *)(PaintCanvas_TransformGetTransform32(*canvas, I(geometry, 0xc))))->SetAnimationState((AbyssEngine::AnimationMode)1, 0);

        geometry = (*self->fireStreaks)[i];
        F<float>((void *)PaintCanvas_TransformGetTransform32(*canvas, I(geometry, 0xc)), 0xe0) =
            10000.0f;

        geometry = (*self->fireStreaks)[i];
        float x = (float)nextInt(Explosion_random, 0x168);
        float y = (float)nextInt(Explosion_random, 0x168);
        float z = (float)nextInt(Explosion_random, 0x168);
        AEGeometry_setRotation3(geometry, (x / 180.0f) * 3.1415927f,
                                (y / 180.0f) * 3.1415927f,
                                (z / 180.0f) * 3.1415927f);

        int scaleInt = nextInt(Explosion_random, 0x32) + 0x32;
        float scale = (float)scaleInt / 100.0f;
        geometry = (*self->fireStreaks)[i];
        AEGeometry_setScaling3(geometry, scale, scale, scale);
    }
}

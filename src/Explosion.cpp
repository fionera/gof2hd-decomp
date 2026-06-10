#include "gof2/Explosion.h"


extern "C" uint32_t PaintCanvas_TransformGetTransform32(int canvas, int transform);
extern "C" long long PaintCanvas_TransformGetTransform(int canvas, int transform);
extern "C" void Transform_SetAnimationState(uint32_t transform, int state, int value);
extern "C" void Transform_SetAnimationRangeInTime(long long transform, long long end, long long start);
extern "C" void *AEGeometry_dtor(AEGeometry *self);
extern "C" void operator_delete(void *ptr);
extern "C" void ArrayReleaseClasses_AEGeometryPtr(Array<AEGeometry *> *self);
extern "C" void *Array_AEGeometryPtr_dtor(Array<AEGeometry *> *self);
extern "C" void AEGeometry_translate_vec(AEGeometry *self, const Vector *v);
extern "C" void Explosion_tail_translate(void *geometry, const Vector *v);
extern "C" void AEGeometry_setScaling3(AEGeometry *self, float x, float y, float z);
extern "C" int AERandom_nextInt(void *self, int bound);
extern "C" void Transform_SetAnimationSpeed(uint32_t transform, float speed);
extern "C" void AEGeometry_setPosition(AEGeometry *self, const Vector *position);
extern "C" void AEGeometry_setDirection(AEGeometry *self, const Vector *direction, const Vector *up);
extern "C" void MatrixSetRotation(Matrix *out, Matrix *base, int zero1, int zero2, float angle);
extern "C" void Transform_Update(long long transform, long long elapsed, int zero);
extern "C" void Transform_Update32(uint32_t transform, uint32_t high, long long elapsed, uint32_t zero);
extern "C" void AEGeometry_getPosition(Vector *out, AEGeometry *self);
extern "C" int PaintCanvas_CameraGetCurrent(int canvas);
extern "C" void *PaintCanvas_CameraGetLocal(int canvas, int current);
extern "C" void Vector_sub(Vector *out, const Vector *a, const Vector *b);
extern "C" float VectorLength(const Vector *self);
extern "C" void TargetFollowCamera_setRumblePercentage(TargetFollowCamera *self, float value, int duration);
extern "C" void Matrix_ctor(Matrix *self);
extern "C" void *operator_new(uint32_t size);
extern "C" void AEGeometry_ctor(AEGeometry *self, uint16_t mesh, int canvas, bool flag);
extern "C" void AEGeometry_addChild(AEGeometry *self, uint32_t child);
extern "C" void FModSound_play(int sound, int id, Vector *pos, int zero, int zero2);
extern "C" void MatrixGetPosition(Vector *out, const Matrix *matrix);
extern "C" void AEGeometry_setMatrix(AEGeometry *self, const Matrix *matrix);
extern "C" void *__aeabi_memcpy(void *dst, const void *src, uint32_t n);
extern "C" void AEGeometry_render(AEGeometry *self);
extern "C" void MatrixSetTranslation(Matrix *out, Matrix *base, float x, float y, float z);
extern "C" void MatrixSetScaling(Matrix *out, Matrix *base, float x, float y, float z);
extern "C" void MatrixGetUp(Vector *out, const Matrix *matrix);
extern "C" void MatrixGetDir(Vector *out, const Matrix *matrix);
extern "C" void MatrixGetLookAt(Matrix *out, const Vector *position, const Vector *target, const Vector *up);
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

    Transform_SetAnimationState(
        PaintCanvas_TransformGetTransform32(*canvas, I(self->field_0x4, 0xc)), 3, 0);
    Transform_SetAnimationState(
        PaintCanvas_TransformGetTransform32(*canvas, I(self->field_0x4, 0xc)), 1, 0);

    int lodTransform = I(self->field_0x4, 0x14);
    if (lodTransform != -1) {
        Transform_SetAnimationState(PaintCanvas_TransformGetTransform32(*canvas, lodTransform), 3, 0);
        Transform_SetAnimationState(
            PaintCanvas_TransformGetTransform32(*canvas, I(self->field_0x4, 0x14)), 1, 0);
    }

    AEGeometry *secondary = self->field_0x8;
    if (secondary != 0) {
        Transform_SetAnimationState(
            PaintCanvas_TransformGetTransform32(*canvas, I(secondary, 0xc)), 3, 0);
        Transform_SetAnimationState(
            PaintCanvas_TransformGetTransform32(*canvas, I(self->field_0x8, 0xc)), 1, 0);
    }

    if (self->field_0x0 == 6) {
        Transform_SetAnimationRangeInTime(
            PaintCanvas_TransformGetTransform(*canvas, I(self->field_0x4, 0xc)),
            0x8fcLL, 10000000LL);
        Transform_SetAnimationRangeInTime(
            PaintCanvas_TransformGetTransform(*canvas, I(self->field_0x8, 0xc)),
            0x8fcLL, 10000000LL);
    }

    Array<AEGeometry *> *streaks = self->field_0xc;
    if (streaks != 0) {
        for (uint32_t i = 0; i < streaks->size(); i++) {
            AEGeometry *geometry = (*streaks)[i];
            Transform_SetAnimationState(
                PaintCanvas_TransformGetTransform32(*canvas, I(geometry, 0xc)), 3, 0);
            Transform_SetAnimationState(
                PaintCanvas_TransformGetTransform32(*canvas, I(geometry, 0xc)), 1, 0);
            streaks = self->field_0xc;
        }
    }

    self->field_0x18 = 0;
    self->field_0x20 = 0;
}

// ---- _Explosion_a80d8.cpp ----
extern "C" Explosion *_ZN9ExplosionD2Ev(Explosion *self)
{
    AEGeometry *geometry = self->field_0x4;
    if (geometry != 0) {
        operator_delete(AEGeometry_dtor(geometry));
    }
    self->field_0x4 = 0;

    geometry = self->field_0x8;
    if (geometry != 0) {
        operator_delete(AEGeometry_dtor(geometry));
    }
    self->field_0x8 = 0;

    Array<AEGeometry *> *streaks = self->field_0xc;
    if (streaks != 0) {
        ArrayReleaseClasses_AEGeometryPtr(streaks);
        streaks = self->field_0xc;
        if (streaks != 0) {
            operator_delete(Array_AEGeometryPtr_dtor(streaks));
        }
    }
    self->field_0xc = 0;
    return self;
}

// ---- isPlaying_a8916.cpp ----
uint8_t Explosion::isPlaying() {
    Explosion *self = this;
    return self->field_0x20;
}

// ---- translate_a8aa0.cpp ----
void Explosion::translate(const Vector *v) {
    Explosion *self = this;
    AEGeometry_translate_vec(self->field_0x4, v);
    void *secondary = self->field_0x8;
    if (secondary != 0) {
        return Explosion_tail_translate(secondary, v);
    }
}

// ---- setScaling_a7e90.cpp ----
extern int Explosion_paintCanvas;
extern void *Explosion_random;


void Explosion::setScaling(float scale) {
    Explosion *self = this;
    self->field_0x24 = scale;
    AEGeometry_setScaling3(self->field_0x4, scale, scale, scale);

    AEGeometry *secondary = self->field_0x8;
    if (secondary != 0) {
        AEGeometry_setScaling3(secondary, scale, scale, scale);
    }

    float speed = 1.0f;
    if (scale < 1.0f) {
        speed = (1.0f - scale) * 3.0f + 1.0f;
    }

    int type = self->field_0x0;
    if (type == 0xb) {
        speed = speed * 0.5f;
    }
    if ((uint32_t)(type - 8) < 3) {
        speed = 0.7f + (float)AERandom_nextInt(Explosion_random, 0x3c) * 0.01f;
    }

    int *canvas = &Explosion_paintCanvas;
    Transform_SetAnimationSpeed(
        PaintCanvas_TransformGetTransform32(*canvas, I(self->field_0x4, 0xc)), speed);

    secondary = self->field_0x8;
    if (secondary != 0) {
        Transform_SetAnimationSpeed(
            PaintCanvas_TransformGetTransform32(*canvas, I(secondary, 0xc)), speed);
    }

    Array<AEGeometry *> *streaks = self->field_0xc;
    if (streaks != 0) {
        for (uint32_t i = 0; i < streaks->size(); i++) {
            AEGeometry *geometry = (*streaks)[i];
            Transform_SetAnimationSpeed(
                PaintCanvas_TransformGetTransform32(*canvas, I(geometry, 0xc)), speed);
            streaks = self->field_0xc;
        }
    }

    self->field_0x10 = (long long)((float)self->field_0x10 / speed);
}

// ---- peakReached_a8900.cpp ----
bool Explosion::peakReached() {
    Explosion *self = this;
    return self->field_0x18 > 0x8fcLL;
}

// ---- start_a83b0.cpp ----
extern int Explosion_paintCanvas;
extern void *Explosion_random;


void Explosion::start(const Vector *position, const Vector *direction) {
    Explosion *self = this;
    AEGeometry_setPosition(self->field_0x4, position);

    int *canvas = &Explosion_paintCanvas;
    UC((void *)PaintCanvas_TransformGetTransform32(*canvas, I(self->field_0x4, 0xc)), 0xed) = 1;

    int lodTransform = I(self->field_0x4, 0x14);
    if (lodTransform != -1) {
        UC((void *)PaintCanvas_TransformGetTransform32(*canvas, lodTransform), 0xed) = 1;
    }

    AEGeometry *secondary = self->field_0x8;
    if (secondary != 0) {
        AEGeometry_setPosition(secondary, position);
        UC((void *)PaintCanvas_TransformGetTransform32(*canvas, I(self->field_0x8, 0xc)), 0xed) = 1;
    }

    int type = self->field_0x0;
    if ((uint32_t)(type - 8) < 3) {
        Matrix rotation;
        float angle = (float)AERandom_nextInt(Explosion_random, 0xc45) / 1000.0f;
        MatrixSetRotation(&rotation, &self->field_0x2c, 0, 0, angle);

        float scale = 0.6f + (float)AERandom_nextInt(Explosion_random, 0x28) * 0.01f;
        ((Explosion *)(self))->setScaling(scale);
    } else if (type == 0xb) {
        Vector up;
        up.x = 0.0f;
        up.y = 1.0f;
        up.z = 0.0f;
        AEGeometry_setDirection(self->field_0x4, direction, &up);
        up.x = 0.0f;
        up.y = 1.0f;
        up.z = 0.0f;
        AEGeometry_setDirection(self->field_0x8, direction, &up);
    }

    Array<AEGeometry *> *streaks = self->field_0xc;
    if (streaks != 0) {
        for (uint32_t i = 0; i < streaks->size(); i++) {
            AEGeometry *geometry = (*streaks)[i];
            AEGeometry_setPosition(geometry, position);
            UC((void *)PaintCanvas_TransformGetTransform32(*canvas, I(geometry, 0xc)), 0xed) = 1;
            streaks = self->field_0xc;
        }
    }

    self->field_0x20 = 1;
    Vector soundPosition = *position;
    ((Explosion *)(self))->playSound(&soundPosition);
}

// ---- update_a8668.cpp ----
extern int Explosion_paintCanvas;


void Explosion::update_camera(int dt, TargetFollowCamera *camera) {
    Explosion *self = this;
    if (self->field_0x20 == 0) {
        return;
    }

    int *canvas = &Explosion_paintCanvas;
    long long delta = (long long)dt;
    Transform_Update(PaintCanvas_TransformGetTransform(*canvas, I(self->field_0x4, 0xc)), delta, 0);

    int lodTransform = I(self->field_0x4, 0x14);
    if (lodTransform != -1) {
        Transform_Update(PaintCanvas_TransformGetTransform(*canvas, lodTransform), delta, 0);
    }

    AEGeometry *secondary = self->field_0x8;
    if (secondary != 0) {
        Transform_Update32(PaintCanvas_TransformGetTransform32(*canvas, I(secondary, 0xc)), 0, delta, 0);
    }

    Array<AEGeometry *> *streaks = self->field_0xc;
    if (streaks != 0) {
        for (uint32_t i = 0; i < streaks->size(); i++) {
            AEGeometry *geometry = (*streaks)[i];
            Transform_Update(PaintCanvas_TransformGetTransform(*canvas, I(geometry, 0xc)), delta, 0);
            streaks = self->field_0xc;
        }
    }

    if (camera != 0 && (uint32_t)self->field_0x0 < 2) {
        char vectors[36];
        Vector *cameraPosition = (Vector *)(vectors + 0);
        Vector *position = (Vector *)(vectors + 12);
        Vector *diff = (Vector *)(vectors + 24);
        AEGeometry_getPosition(position, self->field_0x4);

        int canvasValue = *canvas;
        int current = PaintCanvas_CameraGetCurrent(canvasValue);
        MatrixGetPosition(cameraPosition, (const Matrix *)PaintCanvas_CameraGetLocal(canvasValue, current));
        Vector_sub(diff, position, cameraPosition);
        float distance = VectorLength(diff);

        uint32_t transform = PaintCanvas_TransformGetTransform32(*canvas, I(self->field_0x4, 0xc));
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

    long long elapsed = self->field_0x18 + delta;
    self->field_0x18 = elapsed;
    if (self->field_0x10 < elapsed) {
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

extern "C" Explosion *_ZN9ExplosionC2Ei(Explosion *self, int type)
{
    Matrix_ctor(&self->field_0x2c);
    self->field_0x24 = 1.0f;
    self->field_0x0 = type;
    self->field_0x4 = 0;
    self->field_0x8 = 0;
    self->field_0xc = 0;

    int *canvas = &Explosion_paintCanvas;
    AEGeometry *geometry;

    switch (type) {
    case 0:
    case 6:
        self->field_0x4 = make_geometry(0x41b5, *canvas);
        geometry = make_geometry(0x41b4, *canvas);
        AEGeometry_addChild(self->field_0x4, U(geometry, 0xc));
        operator_delete(AEGeometry_dtor(geometry));
        break;
    default:
        if (type == 13) {
            self->field_0x4 = make_geometry(0x41a9, *canvas);
            ((Explosion *)(self))->setScaling(0.25f);
            Transform_SetAnimationSpeed(
                PaintCanvas_TransformGetTransform32(*canvas, I(self->field_0x4, 0xc)), 1.0f);
        } else {
            self->field_0x4 = make_geometry(0x4213, *canvas);
            self->field_0x8 = make_geometry(0x4211, *canvas);
        }
        break;
    case 3:
        self->field_0x4 = make_geometry(0x4213, *canvas);
        self->field_0x8 = make_geometry(0x421d, *canvas);
        break;
    case 4:
        self->field_0x4 = make_geometry(0x420d, *canvas);
        self->field_0x8 = make_geometry(0x420c, *canvas);
        break;
    case 5:
        self->field_0x4 = make_geometry(0x4999, *canvas);
        self->field_0x8 = make_geometry(0x4998, *canvas);
        break;
    case 7:
        self->field_0x4 = make_geometry(0x41a5, *canvas);
        break;
    case 8:
        self->field_0x4 = make_geometry(0x41a6, *canvas);
        break;
    case 9:
        self->field_0x4 = make_geometry(0x41a7, *canvas);
        break;
    case 10:
        self->field_0x4 = make_geometry(0x41a8, *canvas);
        break;
    case 11:
        self->field_0x4 = make_geometry(0x4a34, *canvas);
        self->field_0x8 = make_geometry(0x4a33, *canvas);
        break;
    case 12:
        self->field_0x4 = make_geometry(0x4a7e, *canvas);
        break;
    }

    Transform_SetAnimationState(
        PaintCanvas_TransformGetTransform32(*canvas, I(self->field_0x4, 0xc)), 1, 0);

    if (self->field_0x8 != 0) {
        Transform_SetAnimationState(
            PaintCanvas_TransformGetTransform32(*canvas, I(self->field_0x8, 0xc)), 1, 0);
        F((void *)PaintCanvas_TransformGetTransform32(*canvas, I(self->field_0x8, 0xc)), 0xe0) =
            10000.0f;
    }

    uint64_t primaryDuration =
        *(uint64_t *)((char *)(void *)PaintCanvas_TransformGetTransform32(*canvas, I(self->field_0x4, 0xc)) + 0xf8);
    uint64_t duration = 0;
    if (self->field_0x8 != 0) {
        uint64_t secondaryDuration =
            *(uint64_t *)((char *)(void *)PaintCanvas_TransformGetTransform32(*canvas, I(self->field_0x8, 0xc)) + 0xf8);
        duration = secondaryDuration < primaryDuration ? primaryDuration : secondaryDuration;
    } else if (primaryDuration != 0) {
        duration = primaryDuration;
    }
    self->field_0x10 = duration;

    F((void *)PaintCanvas_TransformGetTransform32(*canvas, I(self->field_0x4, 0xc)), 0xe0) = 10000.0f;
    self->field_0x28 = -1;
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
    int soundId = self->field_0x28;
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
        int type = self->field_0x0;
        uint32_t typeOffset = (uint32_t)(type - 2);
        if (typeOffset < 4) {
            sound = Explosion_soundSpecial;
            enabled = Explosion_soundSettings[0xf];
            if (enabled == 0) {
                soundPos = 0;
            }
            FModSound_play(sound, 0x15, soundPos, 0, 0);
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
    FModSound_play(sound, cue, soundPos, 0, 0);
}

// ---- start_a853c.cpp ----
extern int Explosion_paintCanvas;


void Explosion::start_matrix(const Matrix *matrix) {
    Explosion *self = this;
    char positionStorage[12];
    Vector *position = (Vector *)positionStorage;

    int type = self->field_0x0;
    if (type == 6 || type == 0) {
        MatrixGetPosition(position, matrix);
        AEGeometry_setPosition(self->field_0x4, position);
        MatrixGetPosition(position, matrix);
        AEGeometry_setPosition(self->field_0x8, position);
    } else {
        AEGeometry_setMatrix(self->field_0x4, matrix);
        AEGeometry *secondary = self->field_0x8;
        if (secondary != 0) {
            AEGeometry_setMatrix(secondary, matrix);
        }
    }

    int *canvas = &Explosion_paintCanvas;
    Array<AEGeometry *> *streaks = self->field_0xc;
    if (streaks != 0) {
        for (uint32_t i = 0; i < streaks->size(); i++) {
            AEGeometry *geometry = (*streaks)[i];
            MatrixGetPosition(position, matrix);
            AEGeometry_setPosition(geometry, position);
            UC((void *)PaintCanvas_TransformGetTransform32(*canvas, I(geometry, 0xc)), 0xed) = 1;
            streaks = self->field_0xc;
        }
    }

    UC((void *)PaintCanvas_TransformGetTransform32(*canvas, I(self->field_0x4, 0xc)), 0xed) = 1;
    int lodTransform = I(self->field_0x4, 0x14);
    if (lodTransform != -1) {
        UC((void *)PaintCanvas_TransformGetTransform32(*canvas, lodTransform), 0xed) = 1;
    }
    AEGeometry *secondary = self->field_0x8;
    if (secondary != 0) {
        UC((void *)PaintCanvas_TransformGetTransform32(*canvas, I(secondary, 0xc)), 0xed) = 1;
    }

    self->field_0x20 = 1;
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

    if (self->field_0x20 != 0) {
        uint32_t type = (uint32_t)self->field_0x0;
        if ((type > 0xd || ((1u << (type & 0xff)) & 0x2780u) == 0) && self->field_0x8 != 0) {
            AEGeometry_render(self->field_0x8);
        }

        int *canvas = &Explosion_paintCanvas;
        int canvasValue = *canvas;
        int current = PaintCanvas_CameraGetCurrent(canvasValue);
        __aeabi_memcpy(&cameraLocal, PaintCanvas_CameraGetLocal(canvasValue, current), 0x3c);

        AEGeometry_getPosition(&position, self->field_0x4);

        if (type < 0xd && ((1u << (type & 0xff)) & 0x1804u) != 0) {
            MatrixSetTranslation(&work, &cameraLocal, position.x, position.y, position.z);
        } else {
            MatrixGetPosition(&cameraPosition, &cameraLocal);
            MatrixGetUp(&up, &cameraLocal);
            MatrixGetLookAt(&work, &cameraPosition, &position, &up);
            Matrix_assign(&cameraLocal, &work);
        }

        float scale = self->field_0x24;
        MatrixSetScaling(&work, &cameraLocal, scale, scale, scale);

        if (type - 8 < 3) {
            Matrix_mul_assign(&cameraLocal, &self->field_0x2c);
        }

        AEGeometry_setMatrix(self->field_0x4, &cameraLocal);
        AEGeometry_setPosition(self->field_0x4, &position);

        canvasValue = *canvas;
        current = PaintCanvas_CameraGetCurrent(canvasValue);
        Matrix_assign(&cameraLocal, (Matrix *)PaintCanvas_CameraGetLocal(canvasValue, current));
        Vector *direction = (Vector *)&work;
        MatrixGetDir(direction, &cameraLocal);
        MatrixGetUp(&cameraPosition, &cameraLocal);
        AEGeometry_setDirection(self->field_0x4, direction, &cameraPosition);
        AEGeometry_render(self->field_0x4);

        Array<AEGeometry *> *streaks = self->field_0xc;
        if (streaks != 0) {
            for (uint32_t i = 0; i < streaks->size(); i++) {
                AEGeometry *geometry = (*streaks)[i];
                AEGeometry_render(geometry);
                streaks = self->field_0xc;
            }
        }
    }
}

// ---- update_a8820.cpp ----
extern int Explosion_paintCanvas;


void Explosion::update_vector(int dt, const Vector *position) {
    Explosion *self = this;
    if (self->field_0x20 == 0) {
        return;
    }

    int *canvas = &Explosion_paintCanvas;
    long long delta = (long long)dt;
    Transform_Update(PaintCanvas_TransformGetTransform(*canvas, I(self->field_0x4, 0xc)), delta, 0);

    int lodTransform = I(self->field_0x4, 0x14);
    if (lodTransform != -1) {
        Transform_Update(PaintCanvas_TransformGetTransform(*canvas, lodTransform), delta, 0);
    }

    AEGeometry *secondary = self->field_0x8;
    if (secondary != 0) {
        Transform_Update32(PaintCanvas_TransformGetTransform32(*canvas, I(secondary, 0xc)), 0, delta, 0);
    }

    Array<AEGeometry *> *streaks = self->field_0xc;
    if (streaks != 0) {
        for (uint32_t i = 0; i < streaks->size(); i++) {
            AEGeometry *geometry = (*streaks)[i];
            AEGeometry_setPosition(geometry, position);
            Transform_Update(PaintCanvas_TransformGetTransform(*canvas, I(geometry, 0xc)), delta, 0);
            streaks = self->field_0xc;
        }
    }

    long long elapsed = self->field_0x18 + delta;
    self->field_0x18 = elapsed;
    if (self->field_0x10 < elapsed) {
        return Explosion_reset_tail(self);
    }
}

// ---- addFireStreaks_a811c.cpp ----
extern int Explosion_paintCanvas;
extern void *Explosion_random;


void Explosion::addFireStreaks() {
    Explosion *self = this;
    if (self->field_0xc != 0) {
        return;
    }

    Array<AEGeometry *> *streaks = new Array<AEGeometry *>();
    self->field_0xc = streaks;

    int length = AERandom_nextInt(Explosion_random, 7) + 3;
    ArraySetLength_AEGeometryPtr(length, self->field_0xc);

    int *canvas = &Explosion_paintCanvas;
    int (*nextInt)(void *, int) = AERandom_nextInt;

    for (uint32_t i = 0; i < self->field_0xc->size(); i++) {
        AEGeometry *geometry = (AEGeometry *)operator_new(0xc0);
        AEGeometry_ctor(geometry, 0x37d4, *canvas, false);
        (*self->field_0xc)[i] = geometry;

        geometry = (*self->field_0xc)[i];
        Transform_SetAnimationState(
            PaintCanvas_TransformGetTransform32(*canvas, I(geometry, 0xc)), 1, 0);

        geometry = (*self->field_0xc)[i];
        F((void *)PaintCanvas_TransformGetTransform32(*canvas, I(geometry, 0xc)), 0xe0) =
            10000.0f;

        geometry = (*self->field_0xc)[i];
        float x = (float)nextInt(Explosion_random, 0x168);
        float y = (float)nextInt(Explosion_random, 0x168);
        float z = (float)nextInt(Explosion_random, 0x168);
        AEGeometry_setRotation3(geometry, (x / 180.0f) * 3.1415927f,
                                (y / 180.0f) * 3.1415927f,
                                (z / 180.0f) * 3.1415927f);

        int scaleInt = nextInt(Explosion_random, 0x32) + 0x32;
        float scale = (float)scaleInt / 100.0f;
        geometry = (*self->field_0xc)[i];
        AEGeometry_setScaling3(geometry, scale, scale, scale);
    }
}

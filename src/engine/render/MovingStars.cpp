#include "gof2/engine/render/MovingStars.h"
#include "gof2/engine/render/PaintCanvas.h"
#include "gof2/platform/libc.h"

using AbyssEngine::PaintCanvas;
using AbyssEngine::AEMath::Vector;
using AbyssEngine::AEMath::Matrix;

extern "C" int MovingStars_kSpawnI_value;

MovingStars::~MovingStars()
{
    if (this->billboardIds) delete[] this->billboardIds;
    this->billboardIds = 0;
    if (this->transformHandles) delete[] this->transformHandles;
    this->transformHandles = 0;
    if (this->lifeArray) delete[] this->lifeArray;
    this->lifeArray = 0;
    if (this->velocityArray) delete[] this->velocityArray;
    this->velocityArray = 0;
}

typedef void *(*AllocFn)(int);

extern "C" int MovingStars_nextIntBounded(uint32_t rng, int bound);        // AERandom::nextInt(seed,bound)
extern "C" uint32_t MovingStars_createBillBoard(int globals, int p2, float a, float b,
                                                float c, float d, int p7);
extern "C" void MovingStars_TransformCreate(void *canvas, uint32_t *out);
extern "C" void MovingStars_TransformAddMeshId(void *canvas, uint32_t tf, uint32_t mesh);
extern "C" uint32_t MovingStars_TransformGetLocal(void *canvas, uint32_t tf);
extern "C" void MovingStars_MatrixSetTranslationFrom(void *out, const void *base,
                                                     float x, float y, float z);
extern "C" void MovingStars_TextureCreate(void *canvas, int id, void *flag, int b);

__attribute__((visibility("hidden"))) extern AllocFn g_MovingStars_alloc;
__attribute__((visibility("hidden"))) extern int *g_MovingStars_rng_ctor;
__attribute__((visibility("hidden"))) extern int *g_MovingStars_globals_ctor;
__attribute__((visibility("hidden"))) extern void **g_MovingStars_canvas_ctor;

static const float kBB0 = 1.0f, kBB1 = 2.0f, kBB2 = 3.0f, kBB3 = 4.0f;

MovingStars::MovingStars()
{
    char mat[60];

    AllocFn alloc = g_MovingStars_alloc;

    this->billboardIds = 0;
    this->transformHandles = 0;
    this->textureHandle = 0;
    this->lifeArray = 0;
    this->velocityArray = 0;

    this->billboardIds = (uint32_t *)alloc(200);
    this->transformHandles = (uint32_t *)alloc(200);
    this->velocityArray = (int *)alloc(200);
    int *arr = (int *)alloc(200);
    this->lifeArray = arr;
    for (int i = 0; i != 0x32; i = i + 1)
        arr[i] = 0xffffffff;

    int *rng = g_MovingStars_rng_ctor;
    int *globals = g_MovingStars_globals_ctor;
    void **canvas = g_MovingStars_canvas_ctor;

    for (int j = 0; j != 50; j = j + 1) {
        MovingStars_nextIntBounded(*rng, 4);
        uint32_t bb = MovingStars_createBillBoard(*globals, 0x46, kBB0, kBB1, kBB2, kBB3, 500);
        this->billboardIds[j] = bb;
        MovingStars_TransformCreate(*canvas, &this->transformHandles[j]);
        MovingStars_TransformAddMeshId(*canvas, this->transformHandles[j],
                                       this->billboardIds[j]);
        uint32_t loc = MovingStars_TransformGetLocal(*canvas, this->transformHandles[j]);
        MovingStars_MatrixSetTranslationFrom(mat, (const void *)loc, 0, 0, 0);
    }

    MovingStars_TextureCreate(*canvas, 0x2711, &this->textureHandle, 0);
    this->tickAccumulator = 0;
    this->animResetFlag = 0;
    this->animActiveFlag = 0;
}

namespace AbyssEngine { namespace AEMath {
Vector MatrixTransformVector(const Matrix &matrix, const Vector &vector);
} }

extern "C" {
float VectorSignedToFloat(int v, int mode);
void MovingStars_TransformSetLocal(void *canvas, void *matrix);        // PaintCanvas::TransformSetLocal
int MovingStars_nextInt(void *rng);                                    // AERandom::nextInt
void MovingStars_MatrixSetTranslation(void *m, float x, float y, float z);
void MovingStars_SetAnimVec(void *transform, uint32_t tf, int idx, float x, float y, float z);
}

__attribute__((visibility("hidden"))) extern void **g_MovingStars_canvas;
__attribute__((visibility("hidden"))) extern void **g_MovingStars_random;

// Animation-curve constants from the literal pool.
extern "C" {
extern float MovingStars_kSpawn, MovingStars_kZ, MovingStars_kZ2;
extern float MovingStars_kA, MovingStars_kB, MovingStars_kC, MovingStars_kD;
extern float MovingStars_kSpeed, MovingStars_kVel, MovingStars_kZSpawn;
}

void MovingStars::update(const Matrix &m, bool flag, float param19)
{
    int delta = (int)m.m[0];

    this->tickAccumulator += delta;

    void *canvas = *g_MovingStars_canvas;

    if (!flag) {
        this->animResetFlag = 0;
        if (this->animActiveFlag != 0) {
            this->animActiveFlag = 0;
            for (int i = 0; i != 50; i += 1) {
                uint32_t tf = this->billboardIds[i];
                MovingStars_SetAnimVec(canvas, tf, 0,  MovingStars_kA, 0, MovingStars_kB);
                MovingStars_SetAnimVec(canvas, tf, 1,  MovingStars_kA, 0, MovingStars_kC);
                MovingStars_SetAnimVec(canvas, tf, 2,  MovingStars_kD, 0, MovingStars_kB);
                MovingStars_SetAnimVec(canvas, tf, 3,  MovingStars_kD, 0, MovingStars_kC);
                MovingStars_SetAnimVec(canvas, tf, 4,  0, MovingStars_kA, MovingStars_kB);
                MovingStars_SetAnimVec(canvas, tf, 5,  0, MovingStars_kA, MovingStars_kC);
                MovingStars_SetAnimVec(canvas, tf, 6,  0, MovingStars_kD, MovingStars_kB);
                MovingStars_SetAnimVec(canvas, tf, 7,  0, MovingStars_kD, MovingStars_kC);
                MovingStars_SetAnimVec(canvas, tf, 8,  MovingStars_kA, MovingStars_kD, 0);
                MovingStars_SetAnimVec(canvas, tf, 9,  MovingStars_kA, MovingStars_kA, 0);
                MovingStars_SetAnimVec(canvas, tf, 10, MovingStars_kD, MovingStars_kD, 0);
                MovingStars_SetAnimVec(canvas, tf, 11, MovingStars_kD, MovingStars_kA, 0);
            }
        }
    } else {
        float fv = param19 * MovingStars_kSpeed;
        float a = VectorSignedToFloat(MovingStars_kSpawnI_value - (int)(param19 * MovingStars_kZ), 0);
        float b = VectorSignedToFloat((int)(param19 * MovingStars_kZ) + 500, 0);
        float c = VectorSignedToFloat((int)(param19 * -20.0f) + 0x46, 0);
        float d = VectorSignedToFloat(-0x46 - (int)(param19 * -20.0f), 0);
        for (int i = 0; i != 50; i += 1) {
            uint32_t tf = this->billboardIds[i];
            MovingStars_SetAnimVec(canvas, tf, 0,  d, 0, b);
            MovingStars_SetAnimVec(canvas, tf, 1,  d, 0, a);
            MovingStars_SetAnimVec(canvas, tf, 2,  c, 0, b);
            MovingStars_SetAnimVec(canvas, tf, 3,  c, 0, a);
            MovingStars_SetAnimVec(canvas, tf, 4,  0, d, b);
            MovingStars_SetAnimVec(canvas, tf, 5,  0, d, a);
            MovingStars_SetAnimVec(canvas, tf, 6,  0, c, b);
            MovingStars_SetAnimVec(canvas, tf, 7,  0, c, a);
            MovingStars_SetAnimVec(canvas, tf, 8,  d, c, 0);
            MovingStars_SetAnimVec(canvas, tf, 9,  d, d, 0);
            MovingStars_SetAnimVec(canvas, tf, 10, c, c, 0);
            MovingStars_SetAnimVec(canvas, tf, 11, c, d, 0);
            this->velocityArray[i] = (int)fv + 1000;
        }
        this->animResetFlag = 0x01;
        this->animActiveFlag = 0x01;
    }

    param19 = param19 * MovingStars_kVel;
    bool respawned = false;

    char localMatrix[0x3c];
    for (int i = 0; i != 0x32; i++) {
        int life = this->lifeArray[i];
        if ((life > 0 || respawned) || (!flag && (int)this->tickAccumulator < 0x29)) {
            this->lifeArray[i] = life - delta;
            memcpy(localMatrix, (const void *)MovingStars_TransformGetLocal(canvas, 0), 0x3c);
            float f = VectorSignedToFloat(this->velocityArray[i], 0);
            // pos -= velocity * f  (three rows of the translation column)
            *(float *)(localMatrix + 0x24) -= *(float *)(localMatrix + 0x18) * f;
            *(float *)(localMatrix + 0x28) -= *(float *)(localMatrix + 0x1c) * f;
            *(float *)(localMatrix + 0x2c) -= *(float *)(localMatrix + 0x20) * f;
            MovingStars_TransformSetLocal(canvas, (void *)this->transformHandles[i]);
        } else {
            this->tickAccumulator = 0;
            void *rng = *g_MovingStars_random;
            int r0 = MovingStars_nextInt(rng);
            int r1 = MovingStars_nextInt(rng);
            Vector spawn;
            spawn.x = VectorSignedToFloat(r0 - 10000, 0);
            spawn.y = VectorSignedToFloat(r1 - 9000, 0);
            spawn.z = MovingStars_kZSpawn;
            *(Vector *)localMatrix =
                AEMath::MatrixTransformVector(*(const Matrix *)localMatrix, spawn);
            spawn = *(const Vector *)localMatrix;
            MovingStars_TransformSetLocal(canvas, (void *)this->transformHandles[i]);
            MovingStars_TransformGetLocal(canvas, 0);
            MovingStars_MatrixSetTranslation(localMatrix, spawn.z, 0.0f, 0.0f);

            int newLife;
            int newTimer;
            if (!flag) {
                newLife = MovingStars_nextInt(rng) + 500;
                newTimer = 2000;
            } else {
                newTimer = 500;
                newLife = (int)param19 + 1000;
            }
            respawned = true;
            this->velocityArray[i] = newLife;
            this->lifeArray[i] = newTimer;
        }
    }
}

// MovingStars::translate(Vector const&)
//   for each of 50 transforms: pos = MatrixGetPosition(local); pos += v;
//   MatrixSetTranslation(out, local, pos.x, pos.y, pos.z)
extern "C" void MovingStars_MatrixGetPosition(void *out, const void *m);
extern "C" void MovingStars_VectorAddAssign(void *self, const Vector &other);

__attribute__((visibility("hidden"))) extern void **g_MovingStars_canvas_translate;

void MovingStars::translate(const Vector &v)
{
    char pos[12];
    char out[60];
    void **canvas = g_MovingStars_canvas_translate;
    for (int i = 0; i != 0x32; i = i + 1) {
        uint32_t tf = this->transformHandles[i];
        uint32_t loc = MovingStars_TransformGetLocal(*canvas, tf);
        MovingStars_MatrixGetPosition(pos, (const void *)loc);
        MovingStars_VectorAddAssign(pos, v);
        uint32_t tf2 = this->transformHandles[i];
        uint32_t base = MovingStars_TransformGetLocal(*canvas, tf2);
        MovingStars_MatrixSetTranslationFrom(out, (const void *)base,
                                             *(float *)(pos + 0), *(float *)(pos + 4),
                                             *(float *)(pos + 8));
    }
}

__attribute__((visibility("hidden"))) extern void **g_MovingStars_canvas_render;

void MovingStars::render()
{
    void **canvas = g_MovingStars_canvas_render;
    ((PaintCanvas*)*canvas)->SetTexture(this->textureHandle, 0xffffffff);
    ((PaintCanvas*)*canvas)->SetBlendMode(1);
    for (int i = 0; i != 0x32; i = i + 1) {
        ((PaintCanvas*)*canvas)->DrawTransform(this->transformHandles[i], (const float*)0);
    }
}

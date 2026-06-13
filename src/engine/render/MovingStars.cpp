#include "gof2/engine/render/MovingStars.h"
#include "gof2/engine/render/PaintCanvas.h"

using AbyssEngine::PaintCanvas;

extern "C" int MovingStars_kSpawnI_value;

// ---- _MovingStars_146190.cpp ----
// MovingStars::~MovingStars() -> delete[] four arrays in order 0x0,0x4,0xc,0x10; returns this.
MovingStars::~MovingStars()
{
    if (pp(this, 0x0)) operator delete[](pp(this, 0x0));
    pp(this, 0x0) = 0;
    if (pp(this, 0x4)) operator delete[](pp(this, 0x4));
    pp(this, 0x4) = 0;
    if (pp(this, 0xc)) operator delete[](pp(this, 0xc));
    pp(this, 0xc) = 0;
    if (pp(this, 0x10)) operator delete[](pp(this, 0x10));
    pp(this, 0x10) = 0;
}

// ---- MovingStars_146050.cpp ----
using AbyssEngine::AEMath::Matrix;

// MovingStars::MovingStars()
typedef void *(*AllocFn)(int);

extern "C" int MovingStars_nextIntBounded(uint32_t rng, int bound);        // AERandom::nextInt(seed,bound)
extern "C" uint32_t MovingStars_createBillBoard(int globals, int p2, float a, float b,
                                                float c, float d, int p7); // 0x76d74
extern "C" void MovingStars_TransformCreate(void *canvas, uint32_t *out);  // 0x720ac
extern "C" void MovingStars_TransformAddMeshId(void *canvas, uint32_t tf, uint32_t mesh); // 0x73030
extern "C" uint32_t MovingStars_TransformGetLocal(void *canvas, uint32_t tf);             // 0x720c4
extern "C" void MovingStars_MatrixSetTranslationFrom(void *out, const void *base,
                                                     float x, float y, float z);           // 0x6f820
extern "C" void MovingStars_TextureCreate(void *canvas, int id, void *flag, int b);        // 0x6fbb0

__attribute__((visibility("hidden"))) extern AllocFn g_MovingStars_alloc;
__attribute__((visibility("hidden"))) extern int *g_MovingStars_rng_ctor;
__attribute__((visibility("hidden"))) extern int *g_MovingStars_globals_ctor;
__attribute__((visibility("hidden"))) extern void **g_MovingStars_canvas_ctor;

static const float kBB0 = 1.0f, kBB1 = 2.0f, kBB2 = 3.0f, kBB3 = 4.0f;

MovingStars::MovingStars()
{
    char mat[60];

    AllocFn alloc = g_MovingStars_alloc;

    u32(this, 0x0) = 0;
    u32(this, 0x4) = 0;
    u32(this, 0x8) = 0;
    u32(this, 0xc) = 0;
    u32(this, 0x10) = 0;

    u32(this, 0x0) = (uint32_t)(uintptr_t)alloc(200);
    u32(this, 0x4) = (uint32_t)(uintptr_t)alloc(200);
    u32(this, 0x10) = (uint32_t)(uintptr_t)alloc(200);
    int *arr = (int *)alloc(200);
    u32(this, 0xc) = (uint32_t)(uintptr_t)arr;
    for (int i = 0; i != 0x32; i = i + 1)
        arr[i] = 0xffffffff;

    int *rng = g_MovingStars_rng_ctor;
    int *globals = g_MovingStars_globals_ctor;
    void **canvas = g_MovingStars_canvas_ctor;

    for (int j = 0; j != 200; j = j + 4) {
        MovingStars_nextIntBounded(*rng, 4);
        uint32_t bb = MovingStars_createBillBoard(*globals, 0x46, kBB0, kBB1, kBB2, kBB3, 500);
        *(uint32_t *)(u32(this, 0x0) + j) = bb;
        MovingStars_TransformCreate(*canvas, (uint32_t *)(u32(this, 0x4) + j));
        MovingStars_TransformAddMeshId(*canvas, *(uint32_t *)(u32(this, 0x4) + j),
                                       *(uint32_t *)(u32(this, 0x0) + j));
        uint32_t loc = MovingStars_TransformGetLocal(*canvas, *(uint32_t *)(u32(this, 0x4) + j));
        MovingStars_MatrixSetTranslationFrom(mat, (const void *)loc, 0, 0, 0);
    }

    MovingStars_TextureCreate(*canvas, 0x2711, (char *)this + 8, 0);
    u32(this, 0x18) = 0;
    u16(this, 0x14) = 0;
}

// ---- update_1461c8.cpp ----
using AbyssEngine::AEMath::Vector;
using AbyssEngine::AEMath::Matrix;

namespace AbyssEngine { namespace AEMath {
Vector MatrixTransformVector(const Matrix &matrix, const Vector &vector);
} }

extern "C" {
float VectorSignedToFloat(int v, int mode);
uint32_t MovingStars_TransformGetLocal(void *canvas, uint32_t tf);     // PaintCanvas::TransformGetLocal
void MovingStars_TransformSetLocal(void *canvas, void *matrix);        // PaintCanvas::TransformSetLocal
int MovingStars_nextInt(void *rng);                                    // AERandom::nextInt
void MovingStars_MatrixSetTranslation(void *m, float x, float y, float z);
void MovingStars_SetAnimVec(void *transform, uint32_t tf, int idx, float x, float y, float z);
}

__attribute__((visibility("hidden"))) extern void **g_MovingStars_canvas;
__attribute__((visibility("hidden"))) extern void **g_MovingStars_random;

// Animation-curve constants from the literal pool (resolved during matching).
extern "C" {
extern float MovingStars_kSpawn, MovingStars_kZ, MovingStars_kZ2;
extern float MovingStars_kA, MovingStars_kB, MovingStars_kC, MovingStars_kD;
extern float MovingStars_kSpeed, MovingStars_kVel, MovingStars_kZSpawn;
}

// Spawn-window base constant (literal-pool integer).

void MovingStars::update(const Matrix &m, bool flag, float param19)
{
    int *self = (int *)this;
    int delta = (int)m.m[0]; // param_2: delta time (passed in r1 ahead of the matrix)

    self[6] += delta;

    void *canvas = *g_MovingStars_canvas;

    if (!flag) {
        u8(self, 0x14) = 0;
        if (u8(self, 0x15) != 0) {
            u8(self, 0x15) = 0;
            for (int i = 0; i != 200; i += 4) {
                uint32_t tf = *(uint32_t *)(self[0] + i);
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
        for (int i = 0; i != 200; i += 4) {
            uint32_t tf = *(uint32_t *)(self[0] + i);
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
            *(int *)(self[4] + i) = (int)fv + 1000;
        }
        u16(self, 0x14) = 0x101;
    }

    param19 = param19 * MovingStars_kVel;
    bool respawned = false;

    char localMatrix[0x3c];
    for (int i = 0; i != 0x32; i++) {
        int life = *(int *)(self[3] + i * 4);
        if ((life > 0 || respawned) || (!flag && self[6] < 0x29)) {
            *(int *)(self[3] + i * 4) = life - delta;
            __aeabi_memcpy(localMatrix, (const void *)MovingStars_TransformGetLocal(canvas, 0), 0x3c);
            float f = VectorSignedToFloat(*(int *)(self[4] + i * 4), 0);
            // pos -= velocity * f  (three rows of the translation column)
            *(float *)(localMatrix + 0x24) -= *(float *)(localMatrix + 0x18) * f;
            *(float *)(localMatrix + 0x28) -= *(float *)(localMatrix + 0x1c) * f;
            *(float *)(localMatrix + 0x2c) -= *(float *)(localMatrix + 0x20) * f;
            MovingStars_TransformSetLocal(canvas, *(void **)(self[1] + i * 4));
        } else {
            self[6] = 0;
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
            MovingStars_TransformSetLocal(canvas, *(void **)(self[1] + i * 4));
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
            *(int *)(self[4] + i * 4) = newLife;
            *(int *)(self[3] + i * 4) = newTimer;
        }
    }
}

// ---- translate_14675c.cpp ----
using AbyssEngine::AEMath::Vector;
using AbyssEngine::AEMath::Matrix;

// MovingStars::translate(Vector const&)
//   for each of 50 transforms: pos = MatrixGetPosition(local); pos += v;
//   MatrixSetTranslation(out, local, pos.x, pos.y, pos.z)
extern "C" uint32_t MovingStars_TransformGetLocal(void *canvas, uint32_t tf);     // 0x720c4
extern "C" void MovingStars_MatrixGetPosition(void *out, const void *m);          // 0x6f16c
extern "C" void MovingStars_VectorAddAssign(void *self, const Vector &other);     // 0x73534
extern "C" void MovingStars_MatrixSetTranslationFrom(void *out, const void *base,
                                                     float x, float y, float z);  // 0x6f820

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

// ---- render_146714.cpp ----
using AbyssEngine::AEMath::Matrix;

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

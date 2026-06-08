#include "class.h"

using AbyssEngine::AEMath::Vector;
using AbyssEngine::AEMath::Matrix;

extern "C" {
float VectorSignedToFloat(int v, int mode);
uint32_t MovingStars_TransformGetLocal(void *canvas, uint32_t tf);     // PaintCanvas::TransformGetLocal
void MovingStars_TransformSetLocal(void *canvas, void *matrix);        // PaintCanvas::TransformSetLocal
int MovingStars_nextInt(void *rng);                                    // AERandom::nextInt
void MovingStars_MatrixTransformVector(void *m, Vector *v);            // AEMath::MatrixTransformVector
void MovingStars_VectorAssign(Vector *dst, const Vector *src);         // AEMath::Vector::operator=
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
extern "C" int MovingStars_kSpawnI_value;

struct MovingStars {
    void update(const Matrix &m, bool flag, float param19);
};

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
            MovingStars_MatrixTransformVector(localMatrix, &spawn);
            MovingStars_VectorAssign(&spawn, (Vector *)localMatrix);
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

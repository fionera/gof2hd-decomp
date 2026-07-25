#include "engine/render/MovingStars.h"
#include "engine/render/PaintCanvas.h"
#include "engine/core/AERandom.h"
#include "game/core/Globals.h"

static int MovingStars_kSpawnI_value = 0;

MovingStars::~MovingStars() {
    if (this->billboardIds) delete[] this->billboardIds;
    this->billboardIds = 0;
    if (this->transformHandles) delete[] this->transformHandles;
    this->transformHandles = 0;
    if (this->lifeArray) delete[] this->lifeArray;
    this->lifeArray = 0;
    if (this->velocityArray) delete[] this->velocityArray;
    this->velocityArray = 0;
}


static const float kBB0 = 1.0f, kBB1 = 2.0f, kBB2 = 3.0f, kBB3 = 4.0f;

MovingStars::MovingStars() {
    this->billboardIds = 0;
    this->transformHandles = 0;
    this->textureHandle = 0;
    this->lifeArray = 0;
    this->velocityArray = 0;

    this->billboardIds = new uint32_t[50];
    this->transformHandles = new uint32_t[50];
    this->velocityArray = new int[50];
    int *arr = new int[50];
    this->lifeArray = arr;
    for (int i = 0; i != 0x32; i = i + 1)
        arr[i] = 0xffffffff;

    // Reload the canvas/rnd/globals pointers through their globals each use; caching the
    // values in locals hoists the loads and diverges from the original per-iteration reloads.
    PaintCanvas **canvas = &Globals::Canvas;

    for (int j = 0; j != 50; j = j + 1) {
        Globals::rnd->nextInt(4);
        uint32_t bb = Globals::globals
                ->createBillBoard(0x46, 500, kBB0, kBB1, kBB2, kBB3, 20052);
        this->billboardIds[j] = bb;
        (*canvas)->TransformCreate(this->transformHandles[j]);
        (*canvas)->TransformAddMeshId(this->transformHandles[j], this->billboardIds[j]);
        Matrix mat = AbyssEngine::AEMath::MatrixSetTranslation(
                *(Matrix *) (*canvas)->TransformGetLocal(this->transformHandles[j]), 0, 0, 0);
        (void) mat;
    }

    (*canvas)->TextureCreate((unsigned short)0x2711, this->textureHandle, false);
    this->tickAccumulator = 0;
    this->animResetFlag = 0;
    this->animActiveFlag = 0;
}

namespace AbyssEngine {
    namespace AEMath {
        Vector MatrixTransformVector(const Matrix &matrix, const Vector &vector);
    }
}

float VectorSignedToFloat(int v, int mode);

static float MovingStars_kSpawn = 0, MovingStars_kZ = 0, MovingStars_kZ2 = 0;
static float MovingStars_kA = 0, MovingStars_kB = 0, MovingStars_kC = 0, MovingStars_kD = 0;
static float MovingStars_kSpeed = 0, MovingStars_kVel = 0, MovingStars_kZSpawn = 0;

void MovingStars::update(int param1, Matrix m, bool flag, float param19) {
    (void) param1;
    int delta = (int) m.m[0];

    this->tickAccumulator += delta;

    PaintCanvas *canvas = Globals::Canvas;

    if (!flag) {
        this->animResetFlag = 0;
        if (this->animActiveFlag != 0) {
            this->animActiveFlag = 0;
            for (int i = 0; i != 50; i += 1) {
                uint32_t tf = this->billboardIds[i];
                canvas->SpriteSystemSetPosition(tf, 0, MovingStars_kA, 0, MovingStars_kB);
                canvas->SpriteSystemSetPosition(tf, 1, MovingStars_kA, 0, MovingStars_kC);
                canvas->SpriteSystemSetPosition(tf, 2, MovingStars_kD, 0, MovingStars_kB);
                canvas->SpriteSystemSetPosition(tf, 3, MovingStars_kD, 0, MovingStars_kC);
                canvas->SpriteSystemSetPosition(tf, 4, 0, MovingStars_kA, MovingStars_kB);
                canvas->SpriteSystemSetPosition(tf, 5, 0, MovingStars_kA, MovingStars_kC);
                canvas->SpriteSystemSetPosition(tf, 6, 0, MovingStars_kD, MovingStars_kB);
                canvas->SpriteSystemSetPosition(tf, 7, 0, MovingStars_kD, MovingStars_kC);
                canvas->SpriteSystemSetPosition(tf, 8, MovingStars_kA, MovingStars_kD, 0);
                canvas->SpriteSystemSetPosition(tf, 9, MovingStars_kA, MovingStars_kA, 0);
                canvas->SpriteSystemSetPosition(tf, 10, MovingStars_kD, MovingStars_kD, 0);
                canvas->SpriteSystemSetPosition(tf, 11, MovingStars_kD, MovingStars_kA, 0);
            }
        }
    } else {
        float fv = param19 * MovingStars_kSpeed;
        float a = VectorSignedToFloat(MovingStars_kSpawnI_value - (int) (param19 * MovingStars_kZ), 0);
        float b = VectorSignedToFloat((int) (param19 * MovingStars_kZ) + 500, 0);
        float c = VectorSignedToFloat((int) (param19 * -20.0f) + 0x46, 0);
        float d = VectorSignedToFloat(-0x46 - (int) (param19 * -20.0f), 0);
        for (int i = 0; i != 50; i += 1) {
            uint32_t tf = this->billboardIds[i];
            canvas->SpriteSystemSetPosition(tf, 0, d, 0, b);
            canvas->SpriteSystemSetPosition(tf, 1, d, 0, a);
            canvas->SpriteSystemSetPosition(tf, 2, c, 0, b);
            canvas->SpriteSystemSetPosition(tf, 3, c, 0, a);
            canvas->SpriteSystemSetPosition(tf, 4, 0, d, b);
            canvas->SpriteSystemSetPosition(tf, 5, 0, d, a);
            canvas->SpriteSystemSetPosition(tf, 6, 0, c, b);
            canvas->SpriteSystemSetPosition(tf, 7, 0, c, a);
            canvas->SpriteSystemSetPosition(tf, 8, d, c, 0);
            canvas->SpriteSystemSetPosition(tf, 9, d, d, 0);
            canvas->SpriteSystemSetPosition(tf, 10, c, c, 0);
            canvas->SpriteSystemSetPosition(tf, 11, c, d, 0);
            this->velocityArray[i] = (int) fv + 1000;
        }
        this->animResetFlag = 0x01;
        this->animActiveFlag = 0x01;
    }

    param19 = param19 * MovingStars_kVel;
    bool respawned = false;

    char localMatrix[0x3c];
    for (int i = 0; i != 0x32; i++) {
        int life = this->lifeArray[i];
        if ((life > 0 || respawned) || (!flag && (int) this->tickAccumulator < 0x29)) {
            this->lifeArray[i] = life - delta;
            memcpy(localMatrix, (const Matrix *) canvas->TransformGetLocal(0), 0x3c);
            float f = AbyssEngine::AEMath::VectorSignedToFloat(this->velocityArray[i], 0);

            *(float *) (localMatrix + 0x24) -= *(float *) (localMatrix + 0x18) * f;
            *(float *) (localMatrix + 0x28) -= *(float *) (localMatrix + 0x1c) * f;
            *(float *) (localMatrix + 0x2c) -= *(float *) (localMatrix + 0x20) * f;
            canvas->TransformSetLocal(this->transformHandles[i], *(const Matrix *) localMatrix);
        } else {
            this->tickAccumulator = 0;
            AERandom *rng = Globals::rnd;
            int r0 = rng->nextInt();
            int r1 = rng->nextInt();
            Vector spawn;
            spawn.x = VectorSignedToFloat(r0 - 10000, 0);
            spawn.y = VectorSignedToFloat(r1 - 9000, 0);
            spawn.z = MovingStars_kZSpawn;
            *(Vector *) localMatrix =
                    AbyssEngine::AEMath::MatrixTransformVector(*(const Matrix *) localMatrix, spawn);
            spawn = *(const Vector *) localMatrix;
            canvas->TransformSetLocal(this->transformHandles[i], *(const Matrix *) localMatrix);
            canvas->TransformGetLocal(0);
            AbyssEngine::AEMath::MatrixSetTranslation(*(Matrix *) localMatrix, spawn.z, 0.0f, 0.0f);

            int newLife;
            int newTimer;
            if (!flag) {
                newLife = rng->nextInt() + 500;
                newTimer = 2000;
            } else {
                newTimer = 500;
                newLife = (int) param19 + 1000;
            }
            respawned = true;
            this->velocityArray[i] = newLife;
            this->lifeArray[i] = newTimer;
        }
    }
}

void MovingStars::translate(const Vector &v) {
    PaintCanvas **canvas = &Globals::Canvas;
    for (int i = 0; i != 0x32; i = i + 1) {
        uint32_t tf = this->transformHandles[i];
        Vector pos = AbyssEngine::AEMath::MatrixGetPosition(
                *(const Matrix *) (*canvas)->TransformGetLocal(tf));
        pos += v;
        uint32_t tf2 = this->transformHandles[i];
        Matrix moved = AbyssEngine::AEMath::MatrixSetTranslation(
                *(Matrix *) (*canvas)->TransformGetLocal(tf2), pos.x, pos.y, pos.z);
        (void) moved;
    }
}


void MovingStars::render() {
    PaintCanvas **canvas = &Globals::Canvas;
    (*canvas)->SetTexture(this->textureHandle, 0xffffffff);
    (*canvas)->SetBlendMode(AbyssEngine::BlendMode_1);
    for (int i = 0; i != 0x32; i = i + 1) {
        (*canvas)->DrawTransform(this->transformHandles[i], nullptr);
    }
}

#include "engine/render/MovingStars.h"
#include "engine/render/PaintCanvas.h"
#include "engine/core/AERandom.h"
#include "game/core/Globals.h"

static int MovingStars_kSpawnI_value = -500;

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

static float MovingStars_kSpawn = 0, MovingStars_kZ = 4500.0f, MovingStars_kZ2 = 0;
static float MovingStars_kA = -70.0f, MovingStars_kB = 500.0f;
static float MovingStars_kC = -500.0f, MovingStars_kD = 70.0f;
static float MovingStars_kSpeed = 1000.0f, MovingStars_kVel = 1000.0f;
static float MovingStars_kZSpawn = 20000.0f;

void MovingStars::update(int param1, Matrix m, bool flag, float param19) {
    int delta = param1;

    this->tickAccumulator += delta;

    PaintCanvas **canvas = &Globals::Canvas;

    if (!flag) {
        this->animResetFlag = 0;
        if (this->animActiveFlag != 0) {
            this->animActiveFlag = 0;
            for (int i = 0; i != 50; i += 1) {
                (*canvas)->SpriteSystemSetPosition(this->billboardIds[i], 0, MovingStars_kA, 0, MovingStars_kB);
                (*canvas)->SpriteSystemSetPosition(this->billboardIds[i], 1, MovingStars_kA, 0, MovingStars_kC);
                (*canvas)->SpriteSystemSetPosition(this->billboardIds[i], 2, MovingStars_kD, 0, MovingStars_kB);
                (*canvas)->SpriteSystemSetPosition(this->billboardIds[i], 3, MovingStars_kD, 0, MovingStars_kC);
                (*canvas)->SpriteSystemSetPosition(this->billboardIds[i], 4, 0, MovingStars_kA, MovingStars_kB);
                (*canvas)->SpriteSystemSetPosition(this->billboardIds[i], 5, 0, MovingStars_kA, MovingStars_kC);
                (*canvas)->SpriteSystemSetPosition(this->billboardIds[i], 6, 0, MovingStars_kD, MovingStars_kB);
                (*canvas)->SpriteSystemSetPosition(this->billboardIds[i], 7, 0, MovingStars_kD, MovingStars_kC);
                (*canvas)->SpriteSystemSetPosition(this->billboardIds[i], 8, MovingStars_kA, MovingStars_kD, 0);
                (*canvas)->SpriteSystemSetPosition(this->billboardIds[i], 9, MovingStars_kA, MovingStars_kA, 0);
                (*canvas)->SpriteSystemSetPosition(this->billboardIds[i], 10, MovingStars_kD, MovingStars_kD, 0);
                (*canvas)->SpriteSystemSetPosition(this->billboardIds[i], 11, MovingStars_kD, MovingStars_kA, 0);
            }
        }
    } else {
        float fv = param19 * MovingStars_kSpeed;
        float a = (float) (MovingStars_kSpawnI_value - (int) (param19 * MovingStars_kZ));
        float b = (float) ((int) (param19 * MovingStars_kZ) + 500);
        float c = (float) ((int) (param19 * -20.0f) + 0x46);
        float d = (float) (-0x46 - (int) (param19 * -20.0f));
        for (int i = 0; i != 50; i += 1) {
            (*canvas)->SpriteSystemSetPosition(this->billboardIds[i], 0, d, 0, b);
            (*canvas)->SpriteSystemSetPosition(this->billboardIds[i], 1, d, 0, a);
            (*canvas)->SpriteSystemSetPosition(this->billboardIds[i], 2, c, 0, b);
            (*canvas)->SpriteSystemSetPosition(this->billboardIds[i], 3, c, 0, a);
            (*canvas)->SpriteSystemSetPosition(this->billboardIds[i], 4, 0, d, b);
            (*canvas)->SpriteSystemSetPosition(this->billboardIds[i], 5, 0, d, a);
            (*canvas)->SpriteSystemSetPosition(this->billboardIds[i], 6, 0, c, b);
            (*canvas)->SpriteSystemSetPosition(this->billboardIds[i], 7, 0, c, a);
            (*canvas)->SpriteSystemSetPosition(this->billboardIds[i], 8, d, c, 0);
            (*canvas)->SpriteSystemSetPosition(this->billboardIds[i], 9, d, d, 0);
            (*canvas)->SpriteSystemSetPosition(this->billboardIds[i], 10, c, c, 0);
            (*canvas)->SpriteSystemSetPosition(this->billboardIds[i], 11, c, d, 0);
            this->velocityArray[i] = (int) fv + 1000;
        }
        this->animResetFlag = 0x01;
        this->animActiveFlag = 0x01;
    }

    param19 = param19 * MovingStars_kVel;
    bool respawned = false;

    for (int i = 0; i != 0x32; i++) {
        int life = this->lifeArray[i];
        if ((life > 0 || respawned) || (!flag && (int) this->tickAccumulator < 0x29)) {
            this->lifeArray[i] = life - delta;
            Matrix localMatrix =
                    *(const Matrix *) (*canvas)->TransformGetLocal(this->transformHandles[i]);
            float f = (float) this->velocityArray[i];

            localMatrix.m[3] -= localMatrix.m[2] * f;
            localMatrix.m[7] -= localMatrix.m[6] * f;
            localMatrix.m[11] -= localMatrix.m[10] * f;
            (*canvas)->TransformSetLocal(this->transformHandles[i], localMatrix);
        } else {
            this->tickAccumulator = 0;
            AERandom *rng = Globals::rnd;
            int r0 = rng->nextInt(20000);
            int r1 = rng->nextInt(18000);
            Vector spawn;
            spawn.x = (float) (r0 - 10000);
            spawn.y = (float) (r1 - 9000);
            spawn.z = MovingStars_kZSpawn;
            spawn = AbyssEngine::AEMath::MatrixTransformVector(m, spawn);
            (*canvas)->TransformSetLocal(this->transformHandles[i], m);
            (void) AbyssEngine::AEMath::MatrixSetTranslation(
                    *(Matrix *) (*canvas)->TransformGetLocal(this->transformHandles[i]),
                    spawn.x, spawn.y, spawn.z);

            int newLife;
            int newTimer;
            if (!flag) {
                newLife = rng->nextInt(500) + 500;
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

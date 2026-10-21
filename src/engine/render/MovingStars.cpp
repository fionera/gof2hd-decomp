#include "engine/render/MovingStars.h"
#include "engine/render/PaintCanvas.h"
#include "engine/core/AERandom.h"
#include "game/core/Globals.h"


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
                ->createBillBoard(0x46, 500, 0.876953f, 0.248047f, 0.935547f, 0.189453f, 20052);
        this->billboardIds[j] = bb;
        (*canvas)->TransformCreate(this->transformHandles[j]);
        (*canvas)->TransformAddMeshId(this->transformHandles[j], this->billboardIds[j]);
        AbyssEngine::AEMath::MatrixSetTranslation(
                *(Matrix *) (*canvas)->TransformGetLocal(this->transformHandles[j]),
                100000.0f, 100000.0f, 100000.0f);
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

void MovingStars::update(int param1, Matrix m, bool flag, float param19) {
    int delta = param1;

    this->tickAccumulator += delta;

    PaintCanvas **canvas = &Globals::Canvas;

    if (flag) {
        float fv = param19 * 1000.0f;
        float b = (float) ((int) (param19 * 4500.0f) + 500);
        float a = (float) (-500 - (int) (param19 * 4500.0f));
        float d = (float) (-0x46 - (int) (param19 * -20.0f));
        float c = (float) ((int) (param19 * -20.0f) + 0x46);
        for (int i = 0; i != 50; i += 1) {
            (*canvas)->MeshSetPoint(this->billboardIds[i], 0, d, 0, b);
            (*canvas)->MeshSetPoint(this->billboardIds[i], 1, d, 0, a);
            (*canvas)->MeshSetPoint(this->billboardIds[i], 2, c, 0, b);
            (*canvas)->MeshSetPoint(this->billboardIds[i], 3, c, 0, a);
            (*canvas)->MeshSetPoint(this->billboardIds[i], 4, 0, d, b);
            (*canvas)->MeshSetPoint(this->billboardIds[i], 5, 0, d, a);
            (*canvas)->MeshSetPoint(this->billboardIds[i], 6, 0, c, b);
            (*canvas)->MeshSetPoint(this->billboardIds[i], 7, 0, c, a);
            (*canvas)->MeshSetPoint(this->billboardIds[i], 8, d, c, 0);
            (*canvas)->MeshSetPoint(this->billboardIds[i], 9, d, d, 0);
            (*canvas)->MeshSetPoint(this->billboardIds[i], 10, c, c, 0);
            (*canvas)->MeshSetPoint(this->billboardIds[i], 11, c, d, 0);
            this->velocityArray[i] = (int) fv + 1000;
        }
        this->animResetFlag = 0x01;
        this->animActiveFlag = 0x01;
    } else {
        this->animResetFlag = 0;
        if (this->animActiveFlag != 0) {
            this->animActiveFlag = 0;
            for (int i = 0; i != 50; i += 1) {
                (*canvas)->MeshSetPoint(this->billboardIds[i], 0, -70.0f, 0, 500.0f);
                (*canvas)->MeshSetPoint(this->billboardIds[i], 1, -70.0f, 0, -500.0f);
                (*canvas)->MeshSetPoint(this->billboardIds[i], 2, 70.0f, 0, 500.0f);
                (*canvas)->MeshSetPoint(this->billboardIds[i], 3, 70.0f, 0, -500.0f);
                (*canvas)->MeshSetPoint(this->billboardIds[i], 4, 0, -70.0f, 500.0f);
                (*canvas)->MeshSetPoint(this->billboardIds[i], 5, 0, -70.0f, -500.0f);
                (*canvas)->MeshSetPoint(this->billboardIds[i], 6, 0, 70.0f, 500.0f);
                (*canvas)->MeshSetPoint(this->billboardIds[i], 7, 0, 70.0f, -500.0f);
                (*canvas)->MeshSetPoint(this->billboardIds[i], 8, -70.0f, 70.0f, 0);
                (*canvas)->MeshSetPoint(this->billboardIds[i], 9, -70.0f, -70.0f, 0);
                (*canvas)->MeshSetPoint(this->billboardIds[i], 10, 70.0f, 70.0f, 0);
                (*canvas)->MeshSetPoint(this->billboardIds[i], 11, 70.0f, -70.0f, 0);
            }
        }
    }

    param19 = param19 * 1000.0f;
    bool respawned = false;

    for (int i = 0; i != 0x32; i++) {
        int life = this->lifeArray[i];
        if (life <= 0 && !respawned && (flag || (int) this->tickAccumulator >= 41)) {
            this->tickAccumulator = 0;
            int spawnX = Globals::rnd->nextInt(20000) - 10000;
            int spawnY = Globals::rnd->nextInt(18000) - 9000;
            Vector spawn;
            spawn.x = (float) spawnX;
            spawn.y = (float) spawnY;
            spawn.z = 20000.0f;
            spawn = AbyssEngine::AEMath::MatrixTransformVector(m, spawn);
            (*canvas)->TransformSetLocal(this->transformHandles[i], m);
            AbyssEngine::AEMath::MatrixSetTranslation(
                    *(Matrix *) (*canvas)->TransformGetLocal(this->transformHandles[i]),
                    spawn.x, spawn.y, spawn.z);

            int newSpeed;
            int newLife;
            if (flag) {
                newLife = 500;
                newSpeed = (int) param19 + 1000;
            } else {
                newSpeed = Globals::rnd->nextInt(500) + 500;
                newLife = 2000;
            }
            respawned = true;
            this->velocityArray[i] = newSpeed;
            this->lifeArray[i] = newLife;
        } else {
            this->lifeArray[i] = life - delta;
            Matrix localMatrix =
                    *(const Matrix *) (*canvas)->TransformGetLocal(this->transformHandles[i]);
            float f = (float) this->velocityArray[i];

            localMatrix.m[3] -= localMatrix.m[2] * f;
            localMatrix.m[7] -= localMatrix.m[6] * f;
            localMatrix.m[11] -= localMatrix.m[10] * f;
            (*canvas)->TransformSetLocal(this->transformHandles[i], localMatrix);
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
        AbyssEngine::AEMath::MatrixSetTranslation(
                *(Matrix *) (*canvas)->TransformGetLocal(this->transformHandles[i]), pos.x, pos.y, pos.z);
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

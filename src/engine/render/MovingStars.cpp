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


int MovingStars_nextIntBounded(uint32_t rng, int bound);

void MovingStars_TransformCreate(PaintCanvas * canvas, uint32_t * out);

void MovingStars_TransformAddMeshId(PaintCanvas *canvas, uint32_t tf, uint32_t mesh);

uint32_t MovingStars_TransformGetLocal(PaintCanvas *canvas, uint32_t tf);

void MovingStars_MatrixSetTranslationFrom(Matrix *out, const Matrix *base,
                                          float x, float y, float z);

void MovingStars_TextureCreate(PaintCanvas *canvas, int id, uint32_t *flag, int b);





static const float kBB0 = 1.0f, kBB1 = 2.0f, kBB2 = 3.0f, kBB3 = 4.0f;

MovingStars::MovingStars() {
    char mat[60];

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

    int *rng = (int *) &Globals::rnd;
    int *globals = (int *) &Globals::globals;
    PaintCanvas **canvas = &Globals::Canvas;

    for (int j = 0; j != 50; j = j + 1) {
        MovingStars_nextIntBounded(*rng, 4);
        uint32_t bb = reinterpret_cast<Globals *>(*globals)
                ->createBillBoard(0, 0x46, kBB0, kBB1, kBB2, kBB3, 500);
        this->billboardIds[j] = bb;
        MovingStars_TransformCreate(*canvas, &this->transformHandles[j]);
        MovingStars_TransformAddMeshId(*canvas, this->transformHandles[j],
                                       this->billboardIds[j]);
        uint32_t loc = MovingStars_TransformGetLocal(*canvas, this->transformHandles[j]);
        MovingStars_MatrixSetTranslationFrom((Matrix *) mat, (const Matrix *) loc, 0, 0, 0);
    }

    MovingStars_TextureCreate(*canvas, 0x2711, &this->textureHandle, 0);
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

void MovingStars_TransformSetLocal(PaintCanvas *canvas, uint32_t transformHandle);

static inline int MovingStars_nextInt(AERandom *rng) { return rng->nextInt(); }

void MovingStars_MatrixSetTranslation(Matrix *m, float x, float y, float z);

void MovingStars_SetAnimVec(PaintCanvas *transform, uint32_t tf, int idx, float x, float y, float z);



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
                MovingStars_SetAnimVec(canvas, tf, 0, MovingStars_kA, 0, MovingStars_kB);
                MovingStars_SetAnimVec(canvas, tf, 1, MovingStars_kA, 0, MovingStars_kC);
                MovingStars_SetAnimVec(canvas, tf, 2, MovingStars_kD, 0, MovingStars_kB);
                MovingStars_SetAnimVec(canvas, tf, 3, MovingStars_kD, 0, MovingStars_kC);
                MovingStars_SetAnimVec(canvas, tf, 4, 0, MovingStars_kA, MovingStars_kB);
                MovingStars_SetAnimVec(canvas, tf, 5, 0, MovingStars_kA, MovingStars_kC);
                MovingStars_SetAnimVec(canvas, tf, 6, 0, MovingStars_kD, MovingStars_kB);
                MovingStars_SetAnimVec(canvas, tf, 7, 0, MovingStars_kD, MovingStars_kC);
                MovingStars_SetAnimVec(canvas, tf, 8, MovingStars_kA, MovingStars_kD, 0);
                MovingStars_SetAnimVec(canvas, tf, 9, MovingStars_kA, MovingStars_kA, 0);
                MovingStars_SetAnimVec(canvas, tf, 10, MovingStars_kD, MovingStars_kD, 0);
                MovingStars_SetAnimVec(canvas, tf, 11, MovingStars_kD, MovingStars_kA, 0);
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
            MovingStars_SetAnimVec(canvas, tf, 0, d, 0, b);
            MovingStars_SetAnimVec(canvas, tf, 1, d, 0, a);
            MovingStars_SetAnimVec(canvas, tf, 2, c, 0, b);
            MovingStars_SetAnimVec(canvas, tf, 3, c, 0, a);
            MovingStars_SetAnimVec(canvas, tf, 4, 0, d, b);
            MovingStars_SetAnimVec(canvas, tf, 5, 0, d, a);
            MovingStars_SetAnimVec(canvas, tf, 6, 0, c, b);
            MovingStars_SetAnimVec(canvas, tf, 7, 0, c, a);
            MovingStars_SetAnimVec(canvas, tf, 8, d, c, 0);
            MovingStars_SetAnimVec(canvas, tf, 9, d, d, 0);
            MovingStars_SetAnimVec(canvas, tf, 10, c, c, 0);
            MovingStars_SetAnimVec(canvas, tf, 11, c, d, 0);
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
            memcpy(localMatrix, (const Matrix *) MovingStars_TransformGetLocal(canvas, 0), 0x3c);
            float f = VectorSignedToFloat(this->velocityArray[i], 0);

            *(float *) (localMatrix + 0x24) -= *(float *) (localMatrix + 0x18) * f;
            *(float *) (localMatrix + 0x28) -= *(float *) (localMatrix + 0x1c) * f;
            *(float *) (localMatrix + 0x2c) -= *(float *) (localMatrix + 0x20) * f;
            MovingStars_TransformSetLocal(canvas, this->transformHandles[i]);
        } else {
            this->tickAccumulator = 0;
            AERandom *rng = Globals::rnd;
            int r0 = MovingStars_nextInt(rng);
            int r1 = MovingStars_nextInt(rng);
            Vector spawn;
            spawn.x = VectorSignedToFloat(r0 - 10000, 0);
            spawn.y = VectorSignedToFloat(r1 - 9000, 0);
            spawn.z = MovingStars_kZSpawn;
            *(Vector *) localMatrix =
                    AbyssEngine::AEMath::MatrixTransformVector(*(const Matrix *) localMatrix, spawn);
            spawn = *(const Vector *) localMatrix;
            MovingStars_TransformSetLocal(canvas, this->transformHandles[i]);
            MovingStars_TransformGetLocal(canvas, 0);
            MovingStars_MatrixSetTranslation((Matrix *) localMatrix, spawn.z, 0.0f, 0.0f);

            int newLife;
            int newTimer;
            if (!flag) {
                newLife = MovingStars_nextInt(rng) + 500;
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

void MovingStars_MatrixGetPosition(Vector *out, const Matrix *m);

void MovingStars_VectorAddAssign(Vector *self, const Vector &other);


void MovingStars::translate(const Vector &v) {
    char pos[12];
    char out[60];
    PaintCanvas **canvas = &Globals::Canvas;
    for (int i = 0; i != 0x32; i = i + 1) {
        uint32_t tf = this->transformHandles[i];
        uint32_t loc = MovingStars_TransformGetLocal(*canvas, tf);
        MovingStars_MatrixGetPosition((Vector *) pos, (const Matrix *) loc);
        MovingStars_VectorAddAssign((Vector *) pos, v);
        uint32_t tf2 = this->transformHandles[i];
        uint32_t base = MovingStars_TransformGetLocal(*canvas, tf2);
        MovingStars_MatrixSetTranslationFrom((Matrix *) out, (const Matrix *) base,
                                             *(float *) (pos + 0), *(float *) (pos + 4),
                                             *(float *) (pos + 8));
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

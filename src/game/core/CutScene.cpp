#include "game/core/CutScene.h"
#include "engine/core/AERandom.h"
#include "game/ship/TargetFollowCamera.h"
#include "game/core/PaintCanvasClass.h"
#include "game/mission/Status.h"
#include "game/core/Globals.h"
#include "game/world/SolarSystem.h"
#include "engine/render/AEGeometry.h"
#include "game/mission/Item.h"
#include "engine/render/LODManager.h"
#include "engine/math/Transform.h"
#include "engine/core/ApplicationManager.h"
#include "game/world/Level.h"
#include "game/world/LevelScript.h"   // PlayerEgo.h references LevelScript but omits its include
#include "game/ship/PlayerEgo.h"
#include "game/ship/PlayerFighter.h"
#include "game/ship/Ship.h"
#include "platform/libc.h"

using AbyssEngine::AERandom;
using AbyssEngine::AnimationMode;
using AbyssEngine::AEMath::Matrix;

// PaintCanvas singleton declared locally: this TU uses the lean PaintCanvasClass.h
// (full PaintCanvas.h clashes with the Transform/Mesh definitions pulled in here),
// so the canonical extern is mirrored to match the definition in PaintCanvas.cpp.
extern PaintCanvas* gCanvas;

// Tuning constants recovered from the literal pool (named globals; exact values
// are resolved in a later matching pass).
extern float CutScene_proc_tx0, CutScene_proc_tz0;
extern float CutScene_proc_rx0, CutScene_proc_rz0, CutScene_proc_rz1;
extern float CutScene_fogColorMode17, CutScene_fogDensityMode17;
extern float CutScene_fogColorMode4, CutScene_fogDensityMode4;
extern float CutScene_initStartXRotMode2;
extern float CutScene_persp_fov, CutScene_persp_znear, CutScene_persp_zfar;
extern float CutScene_fogDensity_mode17;
extern float CutScene_fogDensity_mode4;
extern float CutScene_fogColor;
extern float CutScene_persp_fov_mode17;
extern float CutScene_persp_fov_mode4;
extern float CutScene_turret_tx, CutScene_turret_ty, CutScene_turret_tz;
extern float CutScene_turret_rx, CutScene_turret_ry, CutScene_turret_rz;
extern float CutScene_turret_rx2, CutScene_turret_ry2, CutScene_turret_rz2;
extern int CutScene_shipBankTable[];

// Fixed-point signed -> float scaler used throughout the cinematic motion code.
extern "C" float VectorSignedToFloat(int v, int mode);
extern "C" void MatrixSetRotation(void *m, float x, float y, float z);
extern "C" void MatrixSetTranslation(void *m, float x, float y, float z);
extern "C" int Station_getIndex(void *station);
extern "C" void *Globals_getShipGroup(void *globals, int type, int slot, bool flag);

CutScene::CutScene(int mode)
{
    this->field_0x18 = 0;
    this->field_0x1c = 0;
    this->mode = mode;
    this->level = nullptr;
    this->turretGeom = nullptr;
    this->followCamera = nullptr;
    this->cameraId6c = (uint32_t)-1;
    this->cameraId70 = (uint32_t)-1;
    this->initialized = 0;
    this->geometries = nullptr;
    this->vec8 = Vector{0.0f, 0.0f, 0.0f};
    this->field_0x14 = 0.0f;
    this->geom28 = nullptr;
    this->geom2c = nullptr;
    this->geom30 = nullptr;
    this->geom34 = nullptr;
    // rotationSpeed is initialized from the literal-pool bit pattern 0x3851b717.
    uint32_t bits = 0x3851b717u;
    __builtin_memcpy(&this->rotationSpeed, &bits, sizeof bits);
}

CutScene::~CutScene()
{
    delete this->rootGeom;
    this->rootGeom = nullptr;

    delete this->turretGeom;
    this->turretGeom = nullptr;

    delete this->level;
    this->level = nullptr;

    gCanvas->FogEnable(0, 1);

    delete this->geom28;
    this->geom28 = nullptr;
    delete this->geom2c;
    this->geom2c = nullptr;
    delete this->geom30;
    this->geom30 = nullptr;
    delete this->geom34;
    this->geom34 = nullptr;

    if (this->geometries != nullptr) {
        for (AEGeometry *e : *this->geometries)
            delete e;
        this->geometries->clear();
        delete this->geometries;
    }
    this->geometries = nullptr;
}

uint8_t CutScene::isInitialized()
{
    return this->initialized;
}

void CutScene::update()
{
    this->update_tail();
}

void CutScene::render3D()
{
    if (this->level != nullptr) {
        uint32_t t = (uint32_t)gAppManager->GetElapsedTimeMillis();
        this->frameDelta = t;
        this->level->update((long long)(int)t, 0u, 0);
        this->level->render(this->frameDelta);
    }
    if (this->geom28 != nullptr) this->geom28->render();
    if (this->geom2c != nullptr) this->geom2c->render();
    if (this->geom30 != nullptr) this->geom30->render();
    if (this->geom34 != nullptr) this->geom34->render();

    if (this->geometries != nullptr) {
        for (AEGeometry *g : *this->geometries)
            g->render();
    }
}

void CutScene::render2D()
{
    this->render2D_tail(this->level);
}

void CutScene::process(int /*delta*/)
{
    if (this->initialized == 0)
        return;

    unsigned int now = (unsigned int)gAppManager->GetCurrentTimeMillis();
    unsigned int prev = this->prevTimeLo;
    unsigned int dt = now - prev;
    // 64-bit elapsed accumulator.
    unsigned int lo = this->accumLo;
    this->accumLo = lo + dt;
    this->accumHi = this->accumHi + ((int)dt >> 31) + (lo + dt < lo ? 1u : 0u);
    this->frameDelta = dt;
    this->field_0x40 = now & 0xffff;
    this->field_0x44 = 0;
    this->prevTimeLo = now & 0xffff;
    this->field_0x4c = 0;

    if (this->followCamera != nullptr)
        this->followCamera->update((int)this->frameDelta);

    PaintCanvas *canvas = gCanvas;

    if (this->mode == 2) {
        float ft = VectorSignedToFloat((int)this->frameDelta, 0);
        this->cameraRotX = this->cameraRotX + this->rotationSpeed * ft;
        char tmp[0x3c];
        memcpy(tmp, canvas->CameraGetLocal(0), 0x3c);
        char mtx[0x3c];
        MatrixSetRotation(mtx, 0.0f, 0.0f, 0.0f);
        canvas->CameraSetLocal(0, *(const Matrix *)(uintptr_t)this->cameraId74);

        long long pt = gStatus->getPlayingTime();
        Array<KIPlayer *> *enemies = this->level->getEnemies();
        if (pt != 0 && enemies != nullptr && enemies->size() > 1) {
            unsigned int n = enemies->size();
            void *e0 = (*enemies)[n - 2];
            // The KIPlayer entry's embedded AEGeometry* lives at +8.
            if (e0 != nullptr && *(int *)((char *)e0 + 8) != 0) {
                AEGeometry *g0 = *(AEGeometry **)((char *)e0 + 8);
                float f = VectorSignedToFloat(this->frameDelta, 0);
                g0->translate(f * CutScene_proc_tx0, 0.0f, 0.0f);
                float f1 = VectorSignedToFloat(this->frameDelta, 0);
                float f2 = VectorSignedToFloat(-(int)this->frameDelta, 0);
                g0->rotate(f1 * CutScene_proc_rx0, 0.0f, f2 * CutScene_proc_rz0);
            }
            void *e1 = (*enemies)[n - 1];
            if (e1 != nullptr && *(int *)((char *)e1 + 8) != 0) {
                AEGeometry *g1 = *(AEGeometry **)((char *)e1 + 8);
                float f = VectorSignedToFloat(this->frameDelta, 0);
                g1->translate(f * CutScene_proc_tx0, 0.0f, 0.0f);
                float f1 = VectorSignedToFloat(this->frameDelta, 0);
                float f2 = VectorSignedToFloat(-(int)this->frameDelta, 0);
                g1->rotate(f1 * CutScene_proc_rx0, 0.0f, f2 * CutScene_proc_rz1);
            }
        }
    } else if (this->mode == 0x17) {
        unsigned int kind;
        void *st = gStatus->getStation();
        if (Station_getIndex(st) == 0x65) {
            kind = 10;
        } else {
            st = gStatus->getStation();
            if (Station_getIndex(st) == 100) {
                kind = 7;
            } else {
                kind = (unsigned int)((SolarSystem *)(long)gStatus->getSystem())->getRace() | 2;
            }
        }
        this->fogTimer84 = this->frameDelta + this->fogTimer84;
        if (((SolarSystem *)(long)gStatus->getSystem())->getRace() == 1) {
            canvas->FogSetParameter(0x2601, 0, CutScene_fogColorMode17, 1.0f, CutScene_fogDensityMode17);
        } else if (kind == 2 && this->geometries != nullptr) {
            unsigned int n = this->geometries->size();
            for (unsigned int i = 0; i < n; i++) {
                AbyssEngine::Transform *t = (AbyssEngine::Transform *)canvas->TransformGetTransform(0);
                t->Update((long long)(unsigned)this->frameDelta, (bool)(unsigned char)this->frameDelta);
                n = this->geometries->size();
            }
            if (this->fogTimer84 > 3000) {
                this->fogTimer84 = 0;
                AERandom *rng = gRandom;
                for (unsigned int i = 0; i < n; i++) {
                    if (rng->nextInt() < 0x14) {
                        AbyssEngine::Transform *t = (AbyssEngine::Transform *)canvas->TransformGetTransform(0);
                        if (t->IsRunning() == 0) {
                            AEGeometry *c0 = (*this->geometries)[i];
                            AbyssEngine::Transform *tr;
                            tr = (AbyssEngine::Transform *)((PaintCanvas *)(uintptr_t)c0->transform)->TransformGetTransform(0);
                            tr->SetAnimationState((AnimationMode)3, nullptr);
                            tr = (AbyssEngine::Transform *)((PaintCanvas *)(uintptr_t)c0->transform)->TransformGetTransform(0);
                            tr->SetAnimationState((AnimationMode)1, nullptr);
                            tr = (AbyssEngine::Transform *)((PaintCanvas *)(uintptr_t)c0->childTransform)->TransformGetTransform(0);
                            tr->SetAnimationState((AnimationMode)3, nullptr);
                            tr = (AbyssEngine::Transform *)((PaintCanvas *)(uintptr_t)c0->childTransform)->TransformGetTransform(0);
                            tr->SetAnimationState((AnimationMode)1, nullptr);
                        }
                    }
                    n = this->geometries->size();
                }
            }
        }

        char tmp[0x3c];
        memcpy(tmp, canvas->TransformGetLocal(0), 0x3c);
        char mtx[0x3c];
        MatrixSetRotation(mtx, 0.0f, 0.0f, 0.0f);
        Array<KIPlayer *> *enemies = this->level->getEnemies();
        // The lead enemy's geometry (KIPlayer+0x8) carries the camera transform at +0xc.
        void *leadGeom = *(void **)((char *)(*enemies)[0] + 8);
        canvas->TransformSetLocal(0, *(const Matrix *)(*(void **)((char *)leadGeom + 0xc)));
        Array<KIPlayer *> *arr = this->level->getEnemies();
        for (unsigned int i = 0; i < arr->size(); i++) {
            Array<KIPlayer *> *en = this->level->getEnemies();
            if ((*en)[i] != nullptr) {
                AbyssEngine::Transform *t = (AbyssEngine::Transform *)canvas->TransformGetTransform(0);
                t->Update((long long)(unsigned)this->frameDelta, (bool)(unsigned char)this->frameDelta);
            }
            arr = this->level->getEnemies();
        }
    } else if (this->mode == 4) {
        Array<KIPlayer *> *arr = this->level->getEnemies();
        for (unsigned int i = 0; i < arr->size(); i++) {
            Array<KIPlayer *> *en = this->level->getEnemies();
            if ((*en)[i] != nullptr) {
                AbyssEngine::Transform *t = (AbyssEngine::Transform *)canvas->TransformGetTransform(0);
                t->Update((long long)(unsigned)this->frameDelta, (bool)(unsigned char)this->frameDelta);
            }
            arr = this->level->getEnemies();
        }

        int race = ((SolarSystem *)(long)gStatus->getSystem())->getRace();
        if (race == 0) {
            AbyssEngine::Transform *t = (AbyssEngine::Transform *)canvas->TransformGetTransform(0);
            t->Update((long long)(unsigned)this->frameDelta, (bool)(unsigned char)this->frameDelta);
        } else {
            int race1 = ((SolarSystem *)(long)gStatus->getSystem())->getRace();
            if (race1 == 1) {
                canvas->FogSetParameter(0x2601, 0, CutScene_fogColorMode4, 1.0f, CutScene_fogDensityMode4);
                canvas->FogEnable(1, 1);
                this->level->getEnemies();
                this->level->getEnemies();
                AbyssEngine::Transform *t = (AbyssEngine::Transform *)canvas->TransformGetTransform(0);
                t->Update((long long)(unsigned)this->frameDelta, (bool)(unsigned char)this->frameDelta);
                if (this->geom30 != nullptr) {
                    AbyssEngine::Transform *t2 = (AbyssEngine::Transform *)canvas->TransformGetTransform(0);
                    t2->Update((long long)(unsigned)this->frameDelta, (bool)(unsigned char)this->frameDelta);
                    int acc = this->frameDelta + this->animTimer7c;
                    this->animTimer7c = acc;
                    if (acc > 20000) {
                        this->animTimer7c = 0;
                        if (gRandom->nextInt() < 100) {
                            AbyssEngine::Transform *a = (AbyssEngine::Transform *)canvas->TransformGetTransform(0);
                            a->SetAnimationState((AnimationMode)3, nullptr);
                            a = (AbyssEngine::Transform *)canvas->TransformGetTransform(0);
                            a->SetAnimationState((AnimationMode)1, nullptr);
                        }
                    }
                }
                if (this->geom34 != nullptr) {
                    AbyssEngine::Transform *t2 = (AbyssEngine::Transform *)canvas->TransformGetTransform(0);
                    t2->Update((long long)(unsigned)this->frameDelta, (bool)(unsigned char)this->frameDelta);
                    int acc = this->frameDelta + this->animTimer80;
                    this->animTimer80 = acc;
                    if (acc > 22000) {
                        this->animTimer80 = 0;
                        if (gRandom->nextInt() < 100) {
                            AbyssEngine::Transform *a = (AbyssEngine::Transform *)canvas->TransformGetTransform(0);
                            a->SetAnimationState((AnimationMode)3, nullptr);
                            canvas->TransformGetTransform(0);
                            this->processAux();
                            return;
                        }
                    }
                }
                this->processAux();
                return;
            }
            int race3 = ((SolarSystem *)(long)gStatus->getSystem())->getRace();
            if (race3 == 3) {
                this->level->getEnemies();
                this->level->getEnemies();
                AbyssEngine::Transform *t = (AbyssEngine::Transform *)canvas->TransformGetTransform(0);
                t->Update((long long)(unsigned)this->frameDelta, (bool)(unsigned char)this->frameDelta);
                if (this->geom28 != nullptr) {
                    AbyssEngine::Transform *t2 = (AbyssEngine::Transform *)canvas->TransformGetTransform(0);
                    t2->Update((long long)(unsigned)this->frameDelta, (bool)(unsigned char)this->frameDelta);
                }
                AbyssEngine::Transform *t3 = (AbyssEngine::Transform *)canvas->TransformGetTransform(0);
                t3->Update((long long)(unsigned)this->frameDelta, (bool)(unsigned char)this->frameDelta);
                int acc7c = this->animTimer7c + this->frameDelta;
                int acc80 = this->frameDelta + this->animTimer80;
                this->animTimer7c = acc7c;
                this->animTimer80 = acc80;
                if (acc7c > 1000 && this->geom28 != nullptr) {
                    this->animTimer7c = 0;
                    if (gRandom->nextInt() < 0x28) {
                        AbyssEngine::Transform *a = (AbyssEngine::Transform *)canvas->TransformGetTransform(0);
                        a->SetAnimationState((AnimationMode)3, nullptr);
                        a = (AbyssEngine::Transform *)canvas->TransformGetTransform(0);
                        a->SetAnimationState((AnimationMode)1, nullptr);
                    }
                    acc80 = this->animTimer80;
                }
                if (acc80 > 2000) {
                    this->animTimer80 = 0;
                    if (gRandom->nextInt() < 0x1e) {
                        AbyssEngine::Transform *a = (AbyssEngine::Transform *)canvas->TransformGetTransform(0);
                        a->SetAnimationState((AnimationMode)3, nullptr);
                        canvas->TransformGetTransform(0);
                        this->processAux();
                        return;
                    }
                }
                this->processAux();
                return;
            }
            if (((SolarSystem *)(long)gStatus->getSystem())->getRace() != 2)
                return;
            this->processAux();
            this->processAux();
            AbyssEngine::Transform *t = (AbyssEngine::Transform *)canvas->TransformGetTransform(0);
            t->Update((long long)(unsigned)this->frameDelta, (bool)(unsigned char)this->frameDelta);
            this->processAux();
            this->processAux();
            AbyssEngine::Transform *t2 = (AbyssEngine::Transform *)canvas->TransformGetTransform(0);
            t2->Update((long long)(unsigned)this->frameDelta, (bool)(unsigned char)this->frameDelta);
        }
    }
}

void CutScene::renderBG()
{
    this->renderBG_tail(this->level, this->frameDelta);
}

void CutScene::replacePlayerShip(int /*a*/, int b)
{
    if (this->level->getEnemies() == nullptr)
        return;

    Array<KIPlayer *> *enemies = this->level->getEnemies();
    // The lead enemy is element [0]; its embedded AEGeometry* lives at KIPlayer+0x8.
    AEGeometry *oldGeom = *(AEGeometry **)((char *)(*enemies)[0] + 8);
    if (oldGeom != nullptr) {
        if (this->turretGeom != nullptr) {
            PaintCanvas *canvas = gCanvas;
            Array<KIPlayer *> *en2 = this->level->getEnemies();
            AEGeometry *lead = *(AEGeometry **)((char *)(*en2)[0] + 8);
            canvas->TransformRemoveChild(lead->childTransform, this->turretGeom->childTransform);
        }

        char matrix[0x3c];
        memcpy(matrix, &oldGeom->getMatrix(), 0x3c);

        void *grp = Globals_getShipGroup(gGlobals, b, 0, false);

        Array<KIPlayer *> *en3 = this->level->getEnemies();
        *(void **)((char *)(*en3)[0] + 8) = grp;

        Array<KIPlayer *> *en4 = this->level->getEnemies();
        // The matrix is the old ship's local transform saved into `matrix` above.
        (*(AEGeometry **)((char *)(*en4)[0] + 8))->setMatrix(*(const Matrix *)matrix);

        Array<KIPlayer *> *en5 = this->level->getEnemies();
        void *ship = (*en5)[0];
        float bank = VectorSignedToFloat(CutScene_shipBankTable[b], 0);
        void *obj = *(void **)ship;
        typedef void (*BankFn)(void *, int, float, int);
        BankFn fn = *(BankFn *)(*(char **)obj + 0x48);
        fn(obj, 0, bank, 0);

        Array<KIPlayer *> *en6 = this->level->getEnemies();
        ((PlayerFighter *)((*en6)[0]))->setExhaustVisible(false);

        ((LODManager *)(*(void **)this))->removeObject(oldGeom);
        delete oldGeom;
    }

    this->checkForTurret();
}

void CutScene::initialize()
{
    if (this->level == nullptr)
        this->level = new Level(this->mode);
    while (this->level->init() == 0) {
    }

    this->player = (PlayerEgo *)(intptr_t)this->level->getPlayer();
    if (this->player != nullptr)
        this->player->setActive(true);

    this->level->initParticleSystems();

    char localMatrix[0x3c];
    PaintCanvas *canvas = gCanvas;

    if (this->mode == 2) {
        canvas->CameraCreate(&this->cameraId74);
        canvas->CameraSetPerspective(0, CutScene_persp_fov, CutScene_persp_znear, CutScene_persp_zfar);
        canvas->CameraSetCurrent(this->cameraId74);
        char tmp[0x3c];
        memcpy(tmp, canvas->CameraGetLocal(0), 0x3c);

        AERandom *rng = gRandom;
        int rx = rng->nextInt();
        int ry = rng->nextInt();
        float tx = VectorSignedToFloat(rx - 20000, 0);
        float ty = VectorSignedToFloat(ry + 40000, 0);
        MatrixSetTranslation(localMatrix, tx, ty, 0.0f);
        this->cameraRotX = CutScene_initStartXRotMode2;
        MatrixSetRotation(localMatrix, 0.0f, 0.0f, 0.0f);
        canvas->CameraSetLocal(0, *(const Matrix *)(uintptr_t)this->cameraId74);

        long long pt = gStatus->getPlayingTime();
        Array<KIPlayer *> *enemies = this->level->getEnemies();
        if (pt != 0 && enemies != nullptr && enemies->size() > 1) {
            unsigned int n = enemies->size();
            void *e0 = (*enemies)[n - 2];
            if (e0 != nullptr && (*enemies)[n - 1] != nullptr) {
                float v[3];
                v[0] = VectorSignedToFloat(rx - 24000, 0);
                v[1] = 0.0f;
                v[2] = VectorSignedToFloat(ry + 0x9a4c, 0);
                (**(void (***)(void *, void *))e0)(e0, v);
                v[0] = VectorSignedToFloat(rx - 0x5b68, 0);
                v[2] = VectorSignedToFloat(ry + 0x96c8, 0);
                void *e1 = (*enemies)[n - 1];
                typedef void (*SetFn)(void *, void *);
                SetFn fn = *(SetFn *)(*(char **)e1 + 0x44);
                fn(e1, v);
            }
        }
    } else if (this->mode == 0x17) {
        this->turretGeom = nullptr;
        canvas->CameraCreate(&this->cameraId70);
        canvas->CameraSetPerspective(0, CutScene_persp_fov, CutScene_persp_znear, CutScene_persp_zfar);
        canvas->CameraSetCurrent(this->cameraId70);
        char tmp[0x3c];
        memcpy(tmp, canvas->CameraGetLocal(0), 0x3c);
        MatrixSetRotation(localMatrix, 0.0f, 0.0f, 0.0f);
        MatrixSetTranslation(localMatrix, 0.0f, 0.0f, 0.0f);
        canvas->CameraSetLocal(0, *(const Matrix *)(uintptr_t)this->cameraId70);
        canvas->TransformCreate(&this->transformId78);
        this->level->getEnemies();
        ((AEGeometry *)nullptr)->getPosition();
        canvas->TransformGetLocal(0);
        MatrixSetTranslation(localMatrix, 0.0f, 0.0f, 0.0f);
        canvas->TransformAddChild(this->transformId78, this->cameraId70);
        this->resetCamera();
        this->checkForTurret();
    } else if (this->mode == 4) {
        canvas->CameraCreate(&this->cameraId6c);
        canvas->CameraSetPerspective(0, CutScene_persp_fov, CutScene_persp_znear, CutScene_persp_zfar);
        if (this->followCamera != nullptr) {
            delete this->followCamera;
            this->followCamera = nullptr;
        }
        canvas->CameraSetCurrent(this->cameraId6c);
        this->resetCamera();
        int race = ((SolarSystem *)(long)gStatus->getSystem())->getRace();
        if (race == 3) {
            this->geom2c = new AEGeometry((uint16_t)0x36d6, canvas, false);
            AbyssEngine::Transform *t = (AbyssEngine::Transform *)canvas->TransformGetTransform(0);
            t->SetAnimationState((AnimationMode)0, nullptr);
        } else {
            if (((SolarSystem *)(long)gStatus->getSystem())->getRace() == 0) {
                this->geom30 = new AEGeometry((uint16_t)0x37c8, canvas, false);
                this->geom34 = new AEGeometry((uint16_t)0x37c7, canvas, false);
                this->geom30->addChild(this->geom34->childTransform);
                delete this->geom34;
                this->geom34 = nullptr;
            } else {
                ((SolarSystem *)(long)gStatus->getSystem())->getRace();
            }
        }
    }

    this->rootGeom = new AEGeometry(canvas);
    this->rootGeom->setRotationOrder(2);

    this->accumLo = 0;
    this->accumHi = 0;
    this->animTimer7c = 0;
    this->animTimer80 = 0;
    this->fogTimer84 = 0;

    unsigned int now = (unsigned int)gAppManager->GetCurrentTimeMillis();
    this->initialized = 1;
    this->field_0x40 = now & 0xffff;
    this->field_0x44 = 0;
    this->prevTimeLo = now & 0xffff;
    this->field_0x4c = 0;
}

void CutScene::resetCamera()
{
    PaintCanvas *canvas = gCanvas;

    if (this->mode == 0x17) {
        if (((SolarSystem *)(long)gStatus->getSystem())->getRace() == 1) {
            canvas->FogSetParameter(0x2601, 0, CutScene_fogColor, 1.0f, CutScene_fogDensity_mode17);
            canvas->FogEnable(1, 1);
        }
        canvas->CameraSetCurrent(this->cameraId70);
        canvas->CameraSetPerspective(0, CutScene_persp_fov_mode17, CutScene_persp_znear, CutScene_persp_zfar);
        // Tail: notify the lead enemy ship.
        Array<KIPlayer *> *enemies = this->level->getEnemies();
        void *lead = (*enemies)[0];
        (void)lead;
        return;
    }

    if (this->mode != 4)
        return;

    if (((SolarSystem *)(long)gStatus->getSystem())->getRace() == 1) {
        canvas->FogSetParameter(0x2601, 0, CutScene_fogColor, 1.0f, CutScene_fogDensity_mode4);
        canvas->FogEnable(1, 1);
    }
    canvas->CameraSetCurrent(this->cameraId6c);
    canvas->CameraSetPerspective(0, CutScene_persp_fov_mode4, CutScene_persp_znear, CutScene_persp_zfar);
}

// Minimal local view of the stateless *.bin loader. The full FileRead.h cannot be
// included here: its include chain (AEFile.h -> game/core/String.h) redefines
// AbyssEngine::String, which collides with the canonical definition in common.h.
// Only loadWeaponPositions() is used; the real (trivial) ctor/dtor link from
// FileRead.cpp.
class FileRead {
public:
    FileRead();
    ~FileRead();
    Array<Array<Vector *> *> *loadWeaponPositions(int32_t id);
};

void CutScene::checkForTurret()
{
    PaintCanvas *canvas = gCanvas;

    if (this->turretGeom != nullptr) {
        Array<KIPlayer *> *enemies = this->level->getEnemies();
        AEGeometry *lead = *(AEGeometry **)((char *)(*enemies)[0] + 8);
        canvas->TransformRemoveChild(lead->childTransform, this->turretGeom->childTransform);
    }

    Ship *ship = gStatus->getShip();
    Array<Item *> *equip = ship->getEquipment(2);
    if (equip == nullptr || equip->size() == 0)
        return;
    Item *item = (*equip)[0];
    if (item == nullptr)
        return;

    int idx = item->getIndex();

    unsigned short id0 = 0xffff;   // base geometry
    unsigned short id1 = 0xffff;   // second geometry
    int child0 = -1;
    int child1 = -1;
    int child2 = -1;

    switch (idx) {
    case 0xe0:
        id0 = 0x499a; id1 = 0x499b; child0 = 0x499c; child1 = 0x499d; child2 = -1; break;
    case 0x30: id0 = 0x1a74; id1 = 0x1a75; break;
    case 0x31: id0 = 0x1a76; id1 = 0x1a77; break;
    case 0xb4: id0 = 0x1a95; id1 = 0x1a96; break;
    case 0xb5: id0 = 0x1a97; id1 = 0x1a98; break;
    case 0xb6: id0 = 0x1a99; id1 = 0x1a9a; break;
    case 0xc6:
        id0 = 0x4963; id1 = 0x4967; child0 = 0x4964; child1 = 0x4966; child2 = -1; break;
    case 199:
        id0 = 0x4968; id1 = 0x496b; child0 = 0x4969; child1 = 0x496a; child2 = -1; break;
    case 200:
        id0 = 0x496c; id1 = 0x496f; child0 = 0x496d; child1 = 0x496e; child2 = 0x4970; break;
    case 0x2f: id0 = 0x1a72; id1 = 0x1a73; break;
    default:   id0 = 0xffff; id1 = 0xffff; break;
    }

    AEGeometry *geom0 = new AEGeometry(id0, canvas, false);
    AEGeometry *geom1 = new AEGeometry(id1, canvas, false);
    geom1->setRotationOrder(2);

    if (child0 != -1) {
        AEGeometry *c = new AEGeometry((uint16_t)(unsigned short)child0, canvas, false);
        geom0->addChild(c->childTransform);
        delete c;
    }
    if (child1 != -1) {
        AEGeometry *c = new AEGeometry((uint16_t)(unsigned short)child1, canvas, false);
        geom1->addChild(c->childTransform);
        delete c;
    }
    if (child2 != -1) {
        AEGeometry *c = new AEGeometry((uint16_t)(unsigned short)child2, canvas, false);
        geom1->addChild(c->childTransform);
        delete c;
    }

    delete this->turretGeom;
    this->turretGeom = new AEGeometry(canvas);

    FileRead fr;
    int shipIdx = gStatus->getShip()->getIndex();
    Array<Array<Vector *> *> *positions = fr.loadWeaponPositions(shipIdx);

    Vector *posVec = (*(*positions)[0])[0];
    geom0->setPosition(*posVec);
    geom1->setPosition(*posVec);
    geom0->translate(CutScene_turret_tx, CutScene_turret_ty, CutScene_turret_tz);

    int idx2 = item->getIndex();
    if (idx2 >= 0xc6 && item->getIndex() <= 200) {
        geom0->rotate(CutScene_turret_rx, CutScene_turret_ry, CutScene_turret_rz);
        geom1->rotate(CutScene_turret_rx2, CutScene_turret_ry2, CutScene_turret_rz2);
    }

    this->turretGeom->addChild(geom0->childTransform);
    this->turretGeom->addChild(geom1->childTransform);
    Array<KIPlayer *> *enemies2 = this->level->getEnemies();
    (*(AEGeometry **)((char *)(*enemies2)[0] + 8))->addChild(this->turretGeom->childTransform);

    if (positions == nullptr)
        return;

    for (unsigned int i = 0; i < positions->size(); i++) {
        Array<Vector *> *slot = (*positions)[i];
        if (slot != nullptr) {
            for (Vector *v : *slot)
                delete v;
            slot->clear();
            delete (*positions)[i];
            (*positions)[i] = nullptr;
        }
    }
    delete positions;
}

void CutScene::update_tail()
{
    if (this->level == nullptr)
        return;
    this->level->update((long long)(int)this->frameDelta, 0u, 0);
}

void CutScene::render2D_tail(Level *level)
{
    if (level != nullptr)
        level->render2D();
}

void CutScene::renderBG_tail(Level *level, uint32_t t)
{
    if (level != nullptr) {
        level->renderBG((int)t);
    }
}

void CutScene::processAux()
{
    if (this->level != nullptr)
        this->level->getEnemies();
}

void CutScene::turretFinalize(void * /*positions*/)
{
}

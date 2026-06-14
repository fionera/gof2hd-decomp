#include "gof2/game/core/CutScene.h"
#include "gof2/engine/core/AERandom.h"
#include "gof2/game/ship/TargetFollowCamera.h"
#include "gof2/game/core/PaintCanvasClass.h"
#include "gof2/game/mission/Status.h"
#include "gof2/game/world/SolarSystem.h"
#include "gof2/engine/render/AEGeometry.h"
#include "gof2/game/mission/Item.h"
#include "gof2/engine/render/LODManager.h"
#include "gof2/engine/math/Transform.h"
#include "gof2/engine/core/ApplicationManager.h"
#include "gof2/game/world/Level.h"
#include "gof2/game/ship/PlayerEgo.h"
#include "gof2/game/ship/PlayerFighter.h"
#include "gof2/game/ship/Ship.h"

// Status singleton holder (pc-rel global): the decompiler dropped the Status `self`
// argument on getSystem/getStation/getShip/getPlayingTime; recover it via *gStatus.
__attribute__((visibility("hidden"))) extern Status **gStatus;

extern "C" void *Level_dtor(void *level);

__attribute__((visibility("hidden"))) extern void **g_canvasFog; // FogEnable target

CutScene::~CutScene()
{
    if (this->rootGeom != 0) { this->rootGeom->~AEGeometry(); ::operator delete(this->rootGeom); }
    this->rootGeom = 0;

    if (this->turretGeom != 0) { this->turretGeom->~AEGeometry(); ::operator delete(this->turretGeom); }
    this->turretGeom = 0;

    if (this->level != 0) ::operator delete(Level_dtor(this->level));
    this->level = 0;

    ((PaintCanvas*)(*g_canvasFog))->FogEnable(0, 1);

    if (this->geom28 != 0) { this->geom28->~AEGeometry(); ::operator delete(this->geom28); }
    this->geom28 = 0;

    if (this->geom2c != 0) { this->geom2c->~AEGeometry(); ::operator delete(this->geom2c); }
    this->geom2c = 0;

    if (this->geom30 != 0) { this->geom30->~AEGeometry(); ::operator delete(this->geom30); }
    this->geom30 = 0;

    if (this->geom34 != 0) { this->geom34->~AEGeometry(); ::operator delete(this->geom34); }
    this->geom34 = 0;

    Array<AEGeometry *> *arr = this->geometries;
    if (arr != 0) {
        // ArrayReleaseClasses: destroy every owned AEGeometry, then drop the entries.
        for (AEGeometry *e : *arr) {
            if (e != 0) { e->~AEGeometry(); ::operator delete(e); }
        }
        arr->clear();
        delete this->geometries;
    }
    this->geometries = 0;
}

uint8_t CutScene::isInitialized()
{
    return this->initialized;
}

// update(): single unconditional tail-call thunk (b.w trampoline).

void CutScene::update()
{
    return ((CutScene *)(this))->update_tail();
}

__attribute__((visibility("hidden"))) extern void **g_appManager;

void CutScene::render3D()
{
    if (this->level != 0) {
        uint32_t t = (uint32_t)((ApplicationManager *)(*g_appManager))->GetElapsedTimeMillis();
        this->frameDelta = t;
        // Level::update(long long time, unsigned dt, int flag) — real 3-arg signature
        // recovered from src/Level.cpp; the in-flight Level.h still declares update().
        // Call through a typed member-function pointer so this compiles regardless of
        // the transitional header and preserves the original arguments.
        {
            typedef void (Level::*LevelUpdateFn)(long long, unsigned, int);
            LevelUpdateFn fn = (LevelUpdateFn)(&Level::update);
            (this->level->*fn)((long long)(int)t, 0u, 0);
        }
        this->level->render(this->frameDelta);
    }
    if (this->geom28 != 0) this->geom28->render();
    if (this->geom2c != 0) this->geom2c->render();
    if (this->geom30 != 0) this->geom30->render();
    if (this->geom34 != 0) this->geom34->render();

    Array<AEGeometry *> *arr = this->geometries;
    if (arr != 0) {
        for (uint32_t i = 0; i < arr->size(); i++) {
            (*arr)[i]->render();
            arr = this->geometries;
        }
    }
}

// render2D(): tail-call ext(*this) (the Level pointer at offset 0).

void CutScene::render2D()
{
    return this->render2D_tail(this->level);
}

extern "C" {
float VectorSignedToFloat(int v, int mode);
void *__aeabi_memcpy(void *dst, const void *src, unsigned int n);
void MatrixSetRotation(void *m, float x, float y, float z);
int Station_getIndex(void *station);
}

__attribute__((visibility("hidden"))) extern void **g_canvas;
__attribute__((visibility("hidden"))) extern void **g_random;
extern "C" {
extern float CutScene_proc_tx0, CutScene_proc_tz0;
extern float CutScene_proc_rx0, CutScene_proc_rz0, CutScene_proc_rz1;
extern float CutScene_fogColorMode17, CutScene_fogDensityMode17;
extern float CutScene_fogColorMode4, CutScene_fogDensityMode4;
}

void CutScene::process(int delta)
{
    CutScene *self = (CutScene *)(long)delta;
    if (self->initialized == 0)
        return;

    unsigned int now = (unsigned int)((ApplicationManager *)(*g_appManager))->GetCurrentTimeMillis();
    unsigned int prev = self->prevTimeLo;
    unsigned int dt = now - prev;
    // 64-bit accumulator at +0x50.
    unsigned int lo = self->accumLo;
    self->accumLo = lo + dt;
    self->accumHi = self->accumHi + ((int)dt >> 31) + (lo + dt < lo ? 1u : 0u);
    self->frameDelta = dt;
    self->field_0x40 = now & 0xffff;
    self->field_0x44 = 0;
    self->prevTimeLo = now & 0xffff;
    self->field_0x4c = 0;

    if (self->followCamera != 0)
        ((TargetFollowCamera *)(self->followCamera))->update((int)self->frameDelta);

    int mode = self->mode;

    if (mode == 2) {
        float ft = VectorSignedToFloat((int)self->frameDelta, 0);
        void *canvas = *g_canvas;
        self->cameraRotX = self->cameraRotX + self->rotationSpeed * ft;
        char tmp[0x3c];
        __aeabi_memcpy(tmp, ((PaintCanvas*)(canvas))->CameraGetLocal(0), 0x3c);
        char mtx[0x3c];
        MatrixSetRotation(mtx, 0.0f, 0.0f, 0.0f);
        ((PaintCanvas*)(canvas))->CameraSetLocal(0, *(const AbyssEngine::AEMath::Matrix*)(uintptr_t)self->cameraId74);

        long long pt = ((Status *)(*gStatus))->getPlayingTime();
        unsigned int *enemies = (unsigned int *)self->level->getEnemies();
        if (pt != 0 && enemies != 0 && enemies[0] > 1) {
            char *base = (char *)enemies[1] + enemies[0] * 4;
            void *e0 = *(void **)(base - 8);
            // RAWREAD: e0+8 (enemy-list entry is an unmodeled struct; +8 holds its AEGeometry*)
            if (e0 != 0 && *(int *)((char *)e0 + 8) != 0) {
                AEGeometry *g0 = (AEGeometry *)*(void **)((char *)e0 + 8);
                float f = VectorSignedToFloat(self->frameDelta, 0);
                g0->translate(f * CutScene_proc_tx0, 0.0f, 0.0f);
                float f1 = VectorSignedToFloat(self->frameDelta, 0);
                float f2 = VectorSignedToFloat(-self->frameDelta, 0);
                g0->rotate(f1 * CutScene_proc_rx0, 0.0f, f2 * CutScene_proc_rz0);
            }
            void *e1 = *(void **)(base - 4);
            // RAWREAD: e1+8 (enemy-list entry is an unmodeled struct; +8 holds its AEGeometry*)
            if (e1 != 0 && *(int *)((char *)e1 + 8) != 0) {
                AEGeometry *g1 = (AEGeometry *)*(void **)((char *)e1 + 8);
                float f = VectorSignedToFloat(self->frameDelta, 0);
                g1->translate(f * CutScene_proc_tx0, 0.0f, 0.0f);
                float f1 = VectorSignedToFloat(self->frameDelta, 0);
                float f2 = VectorSignedToFloat(-self->frameDelta, 0);
                g1->rotate(f1 * CutScene_proc_rx0, 0.0f, f2 * CutScene_proc_rz1);
            }
        }
    } else if (mode == 0x17) {
        unsigned int kind;
        void *st = ((Status *)(*gStatus))->getStation();
        if (Station_getIndex(st) == 0x65) {
            kind = 10;
        } else {
            st = ((Status *)(*gStatus))->getStation();
            if (Station_getIndex(st) == 100) {
                kind = 7;
            } else {
                ((Status *)(*gStatus))->getSystem();
                kind = (unsigned int)((SolarSystem *)(long)((Status *)(*gStatus))->getSystem())->getRace() | 2;
            }
        }
        self->fogTimer84 = self->frameDelta + self->fogTimer84;
        ((Status *)(*gStatus))->getSystem();
        if (((SolarSystem *)(long)((Status *)(*gStatus))->getSystem())->getRace() == 1) {
            ((PaintCanvas*)(*g_canvas))->FogSetParameter(0x2601, 0, CutScene_fogColorMode17,
                                        1.0f, CutScene_fogDensityMode17);
        } else if (kind == 2 && self->geometries != 0) {
            void *canvas = *g_canvas;
            Array<AEGeometry *> *arr = self->geometries;
            unsigned int n = arr->size();
            for (unsigned int i = 0; i < n; i++) {
                void *t = ((PaintCanvas*)(canvas))->TransformGetTransform(0);
                ((AbyssEngine::Transform *)(t))->Update((longlong)(unsigned)self->frameDelta, (bool)(unsigned char)self->frameDelta);
                arr = self->geometries;
                n = arr->size();
            }
            if (self->fogTimer84 > 3000) {
                self->fogTimer84 = 0;
                void *rng = *g_random;
                for (unsigned int i = 0; i < n; i++) {
                    if (((AbyssEngine::AERandom *)(rng))->nextInt() < 0x14) {
                        void *t = ((PaintCanvas*)(canvas))->TransformGetTransform(0);
                        if (((AbyssEngine::Transform *)(t))->IsRunning() == 0) {
                            AEGeometry *c0 = (*self->geometries)[i];
                            void *tr;
                            tr = ((PaintCanvas*)(uintptr_t)c0->transform)->TransformGetTransform(0);
                            ((AbyssEngine::Transform *)(tr))->SetAnimationState((AbyssEngine::AnimationMode)3, (void *)0);
                            tr = ((PaintCanvas*)(uintptr_t)c0->transform)->TransformGetTransform(0);
                            ((AbyssEngine::Transform *)(tr))->SetAnimationState((AbyssEngine::AnimationMode)1, (void *)0);
                            tr = ((PaintCanvas*)(uintptr_t)c0->childTransform)->TransformGetTransform(0);
                            ((AbyssEngine::Transform *)(tr))->SetAnimationState((AbyssEngine::AnimationMode)3, (void *)0);
                            tr = ((PaintCanvas*)(uintptr_t)c0->childTransform)->TransformGetTransform(0);
                            ((AbyssEngine::Transform *)(tr))->SetAnimationState((AbyssEngine::AnimationMode)1, (void *)0);
                        }
                    }
                    n = self->geometries->size();
                }
            }
        }

        void *canvas = *g_canvas;
        ((Level *)(self->level))->getEnemies();
        char tmp[0x3c];
        __aeabi_memcpy(tmp, ((PaintCanvas*)(canvas))->TransformGetLocal(0), 0x3c);
        char mtx[0x3c];
        MatrixSetRotation(mtx, 0.0f, 0.0f, 0.0f);
        void *enemies = (void *)(intptr_t)((Level *)(self->level))->getEnemies();
        // RAWREAD: enemies+4/+8/+0xc (getEnemies() returns an unmodeled {count, entry**}
        // list; entries and their embedded AEGeometry handle are untyped here)
        ((PaintCanvas*)(canvas))->TransformSetLocal(0,
            *(const AbyssEngine::AEMath::Matrix*)(*(void **)(*(char **)(*(char **)((char *)enemies + 4) + 8) + 0xc)));
        unsigned int *arr = (unsigned int *)((Level *)(self->level))->getEnemies();
        for (unsigned int i = 0; i < arr[0]; i++) {
            void *en = (void *)(intptr_t)((Level *)(self->level))->getEnemies();
            if (*(void **)(*(char **)((char *)en + 4) + i * 4) != 0) {
                ((Level *)(self->level))->getEnemies();
                void *t = ((PaintCanvas*)(canvas))->TransformGetTransform(0);
                ((AbyssEngine::Transform *)(t))->Update((longlong)(unsigned)self->frameDelta, (bool)(unsigned char)self->frameDelta);
            }
            arr = (unsigned int *)((Level *)(self->level))->getEnemies();
        }
    } else if (mode == 4) {
        void *canvas = *g_canvas;
        unsigned int *arr = (unsigned int *)((Level *)(self->level))->getEnemies();
        for (unsigned int i = 0; i < arr[0]; i++) {
            void *en = (void *)(intptr_t)((Level *)(self->level))->getEnemies();
            if (*(void **)(*(char **)((char *)en + 4) + i * 4) != 0) {
                ((Level *)(self->level))->getEnemies();
                void *t = ((PaintCanvas*)(canvas))->TransformGetTransform(0);
                ((AbyssEngine::Transform *)(t))->Update((longlong)(unsigned)self->frameDelta, (bool)(unsigned char)self->frameDelta);
            }
            arr = (unsigned int *)((Level *)(self->level))->getEnemies();
        }

        ((Status *)(*gStatus))->getSystem();
        int race = ((SolarSystem *)(long)((Status *)(*gStatus))->getSystem())->getRace();
        if (race == 0) {
            void *t = ((PaintCanvas*)(canvas))->TransformGetTransform(0);
            ((AbyssEngine::Transform *)(t))->Update((longlong)(unsigned)self->frameDelta, (bool)(unsigned char)self->frameDelta);
        } else {
            ((Status *)(*gStatus))->getSystem();
            int race1 = ((SolarSystem *)(long)((Status *)(*gStatus))->getSystem())->getRace();
            if (race1 == 1) {
                ((PaintCanvas*)(canvas))->FogSetParameter(0x2601, 0, CutScene_fogColorMode4,
                                            1.0f, CutScene_fogDensityMode4);
                ((PaintCanvas*)(canvas))->FogEnable(1, 1);
                ((Level *)(self->level))->getEnemies();
                ((Level *)(self->level))->getEnemies();
                void *t = ((PaintCanvas*)(canvas))->TransformGetTransform(0);
                ((AbyssEngine::Transform *)(t))->Update((longlong)(unsigned)self->frameDelta, (bool)(unsigned char)self->frameDelta);
                if (self->geom30 != 0) {
                    void *t2 = ((PaintCanvas*)(canvas))->TransformGetTransform(0);
                    ((AbyssEngine::Transform *)(t2))->Update((longlong)(unsigned)self->frameDelta, (bool)(unsigned char)self->frameDelta);
                    int acc = self->frameDelta + self->animTimer7c;
                    self->animTimer7c = acc;
                    if (acc > 20000) {
                        self->animTimer7c = 0;
                        if (((AbyssEngine::AERandom *)(*g_random))->nextInt() < 100) {
                            void *a = ((PaintCanvas*)(canvas))->TransformGetTransform(0);
                            ((AbyssEngine::Transform *)(a))->SetAnimationState((AbyssEngine::AnimationMode)3, (void *)0);
                            a = ((PaintCanvas*)(canvas))->TransformGetTransform(0);
                            ((AbyssEngine::Transform *)(a))->SetAnimationState((AbyssEngine::AnimationMode)1, (void *)0);
                        }
                    }
                }
                if (self->geom34 != 0) {
                    void *t2 = ((PaintCanvas*)(canvas))->TransformGetTransform(0);
                    ((AbyssEngine::Transform *)(t2))->Update((longlong)(unsigned)self->frameDelta, (bool)(unsigned char)self->frameDelta);
                    int acc = self->frameDelta + self->animTimer80;
                    self->animTimer80 = acc;
                    if (acc > 22000) {
                        self->animTimer80 = 0;
                        if (((AbyssEngine::AERandom *)(*g_random))->nextInt() < 100) {
                            void *a = ((PaintCanvas*)(canvas))->TransformGetTransform(0);
                            ((AbyssEngine::Transform *)(a))->SetAnimationState((AbyssEngine::AnimationMode)3, (void *)0);
                            ((PaintCanvas*)(canvas))->TransformGetTransform(0);
                            ((CutScene *)(self))->processAux();
                            return;
                        }
                    }
                }
                ((CutScene *)(self))->processAux();
                return;
            }
            ((Status *)(*gStatus))->getSystem();
            int race3 = ((SolarSystem *)(long)((Status *)(*gStatus))->getSystem())->getRace();
            if (race3 == 3) {
                ((Level *)(self->level))->getEnemies();
                ((Level *)(self->level))->getEnemies();
                void *t = ((PaintCanvas*)(canvas))->TransformGetTransform(0);
                ((AbyssEngine::Transform *)(t))->Update((longlong)(unsigned)self->frameDelta, (bool)(unsigned char)self->frameDelta);
                if (self->geom28 != 0) {
                    void *t2 = ((PaintCanvas*)(canvas))->TransformGetTransform(0);
                    ((AbyssEngine::Transform *)(t2))->Update((longlong)(unsigned)self->frameDelta, (bool)(unsigned char)self->frameDelta);
                }
                void *t3 = ((PaintCanvas*)(canvas))->TransformGetTransform(0);
                ((AbyssEngine::Transform *)(t3))->Update((longlong)(unsigned)self->frameDelta, (bool)(unsigned char)self->frameDelta);
                int acc7c = self->animTimer7c + self->frameDelta;
                int acc80 = self->frameDelta + self->animTimer80;
                self->animTimer7c = acc7c;
                self->animTimer80 = acc80;
                if (acc7c > 1000 && self->geom28 != 0) {
                    self->animTimer7c = 0;
                    if (((AbyssEngine::AERandom *)(*g_random))->nextInt() < 0x28) {
                        void *a = ((PaintCanvas*)(canvas))->TransformGetTransform(0);
                        ((AbyssEngine::Transform *)(a))->SetAnimationState((AbyssEngine::AnimationMode)3, (void *)0);
                        a = ((PaintCanvas*)(canvas))->TransformGetTransform(0);
                        ((AbyssEngine::Transform *)(a))->SetAnimationState((AbyssEngine::AnimationMode)1, (void *)0);
                    }
                    acc80 = self->animTimer80;
                }
                if (acc80 > 2000) {
                    self->animTimer80 = 0;
                    if (((AbyssEngine::AERandom *)(*g_random))->nextInt() < 0x1e) {
                        void *a = ((PaintCanvas*)(canvas))->TransformGetTransform(0);
                        ((AbyssEngine::Transform *)(a))->SetAnimationState((AbyssEngine::AnimationMode)3, (void *)0);
                        ((PaintCanvas*)(canvas))->TransformGetTransform(0);
                        ((CutScene *)(self))->processAux();
                        return;
                    }
                }
                ((CutScene *)(self))->processAux();
                return;
            }
            ((Status *)(*gStatus))->getSystem();
            if (((SolarSystem *)(long)((Status *)(*gStatus))->getSystem())->getRace() != 2)
                return;
            ((CutScene *)(self))->processAux();
            ((CutScene *)(self))->processAux();
            void *t = ((PaintCanvas*)(canvas))->TransformGetTransform(0);
            ((AbyssEngine::Transform *)(t))->Update((longlong)(unsigned)self->frameDelta, (bool)(unsigned char)self->frameDelta);
            ((CutScene *)(self))->processAux();
            ((CutScene *)(self))->processAux();
            void *t2 = ((PaintCanvas*)(canvas))->TransformGetTransform(0);
            ((AbyssEngine::Transform *)(t2))->Update((longlong)(unsigned)self->frameDelta, (bool)(unsigned char)self->frameDelta);
        }
    }
}

// renderBG(): tail-call ext(field_0, field_0x58).

void CutScene::renderBG()
{
    return this->renderBG_tail((Level *)this->level, this->frameDelta);
}

CutScene::CutScene(int param)
{
    this->field_0x18 = 0;
    this->field_0x1c = 0;
    this->mode = param;
    this->level = 0;
    this->turretGeom = 0;
    this->followCamera = 0;
    this->cameraId6c = -1;
    this->cameraId70 = -1;
    this->initialized = 0;
    this->geometries = 0;
    // ctor memset of the 0x8 and 0x28 zero-init blocks (vec8 / field_0x14 and the
    // four cinematic-geometry pointers).
    this->vec8.x = 0.0f; this->vec8.y = 0.0f; this->vec8.z = 0.0f;
    this->field_0x14 = 0.0f;
    this->geom28 = 0;
    this->geom2c = 0;
    this->geom30 = 0;
    this->geom34 = 0;
    // rotationSpeed is initialized from the literal-pool bit pattern 0x3851b717.
    { uint32_t bits = 0x3851b717u; __builtin_memcpy(&this->rotationSpeed, &bits, sizeof bits); }
}

// CutScene_ctor / CutScene_dtor: the C1/D1 entry-point thunks the decompiler
// emitted as free functions. The mod screens (SpaceLounge, ModStation,
// ModMainMenu) allocate a CutScene's storage themselves and then call these to
// construct/destruct it in place. CutScene_ctor runs the complete-object
// constructor over the already-allocated block; CutScene_dtor runs the
// destructor and hands the pointer back so the caller can free it.
extern "C" void CutScene_ctor(void *cutscene, int id)
{
    new (cutscene) CutScene(id);
}

extern "C" void *CutScene_dtor(void *p)
{
    ((CutScene *)p)->~CutScene();
    return p;
}

extern "C" {
void *__aeabi_memcpy(void *dst, const void *src, unsigned int n);
void *Globals_getShipGroup(void *globals, int type, int slot, bool flag);
float VectorSignedToFloat(int v, int mode);
}

__attribute__((visibility("hidden"))) extern void **g_canvas;
__attribute__((visibility("hidden"))) extern void **g_globals;
__attribute__((visibility("hidden"))) extern int CutScene_shipBankTable[];

void CutScene::replacePlayerShip(int a, int b)
{
    CutScene *self = (CutScene *)(long)a;

    if (((Level *)(self->level))->getEnemies() == 0)
        return;

    void *enemies = (void *)(intptr_t)((Level *)(self->level))->getEnemies();
    // RAWREAD: enemies+4/+8 (getEnemies() returns an unmodeled {count, entry**} list;
    // entry+8 holds the lead ship's AEGeometry*, all untyped here)
    void *lead = *(void **)(*(char **)((char *)enemies + 4) + 8);
    void *oldGeom = lead;
    if (oldGeom != 0) {
        if (self->turretGeom != 0) {
            void *canvas = *g_canvas;
            void *en2 = (void *)(intptr_t)((Level *)(self->level))->getEnemies();
            unsigned int parent = u32(*(void **)(*(char **)((char *)en2 + 4) + 8), 0xc);
            unsigned int child = u32(self->turretGeom, 0xc);
            ((PaintCanvas*)(canvas))->TransformRemoveChild(parent, child);
        }

        char matrix[0x3c];
        void *m = ((AEGeometry *)(oldGeom))->getMatrix();
        __aeabi_memcpy(matrix, m, 0x3c);

        void *grp = Globals_getShipGroup(*g_globals, b, 0, false);

        void *en3 = (void *)(intptr_t)((Level *)(self->level))->getEnemies();
        *(void **)(*(char **)((char *)en3 + 4) + 8) = grp;

        void *en4 = (void *)(intptr_t)((Level *)(self->level))->getEnemies();
        // The shim's single pointer is the replacement geometry; the matrix it installs
        // is the old ship's local transform saved into `matrix` above (the decompiler
        // dropped the implicit second argument).
        ((AEGeometry *)(*(void **)(*(char **)((char *)en4 + 4) + 8)))
            ->setMatrix(*(const AbyssEngine::AEMath::Matrix *)matrix);

        void *en5 = (void *)(intptr_t)((Level *)(self->level))->getEnemies();
        void *ship = *(void **)((char *)en5 + 4);
        float bank = VectorSignedToFloat(CutScene_shipBankTable[b], 0);
        void *obj = *(void **)ship;
        typedef void (*BankFn)(void *, int, float, int);
        BankFn fn = *(BankFn *)(*(char **)obj + 0x48);
        fn(obj, 0, bank, 0);

        void *en6 = (void *)(intptr_t)((Level *)(self->level))->getEnemies();
        ((PlayerFighter *)(*(void **)((char *)en6 + 4)))->setExhaustVisible(false);

        ((LODManager *)(*(void **)self))->removeObject((AEGeometry *)oldGeom);
        ((AEGeometry *)oldGeom)->~AEGeometry();
        ::operator delete(oldGeom);
    }

    checkForTurret();
}

extern "C" {
void Level_ctor(void *self, int mode);
void *__aeabi_memcpy(void *dst, const void *src, unsigned int n);
float VectorSignedToFloat(int v, int mode);
void MatrixSetTranslation(void *m, float x, float y, float z);
void MatrixSetRotation(void *m, float x, float y, float z);
void *TargetFollowCamera_dtor(void *self);
}

__attribute__((visibility("hidden"))) extern void **g_canvas;
__attribute__((visibility("hidden"))) extern void **g_random;
extern "C" {
extern float CutScene_initStartXRotMode2;
extern float CutScene_persp_fov, CutScene_persp_znear, CutScene_persp_zfar;
}

void CutScene::initialize()
{
    void *level = this->level;
    if (level == 0) {
        level = ::operator new(0x2a0);
        Level_ctor(level, this->mode);
        this->level = (Level *)level;
    }
    // Level::init() really returns int (staged setup, nonzero == done) per
    // src/Level.cpp; the in-flight Level.h still declares it as void. Call through
    // a typed member-function pointer to recover the return value and loop intent.
    {
        typedef int (Level::*LevelInitFn)();
        LevelInitFn fn = (LevelInitFn)(&Level::init);
        do {
            if ((((Level *)(level))->*fn)() != 0)
                break;
            level = this->level;
        } while (true);
    }

    void *player = (void *)(intptr_t)((Level *)(this->level))->getPlayer();
    this->player = player;
    if (player != 0)
        ((PlayerEgo *)(player))->setActive(true);

    ((Level *)(this->level))->initParticleSystems();

    char localMatrix[0x3c];
    int mode = this->mode;

    if (mode == 2) {
        void *canvas = *g_canvas;
        ((PaintCanvas*)(canvas))->CameraCreate(&this->cameraId74);
        ((PaintCanvas*)(canvas))->CameraSetPerspective(0, CutScene_persp_fov, CutScene_persp_znear,
                                         CutScene_persp_zfar);
        ((PaintCanvas*)(canvas))->CameraSetCurrent(this->cameraId74);
        char tmp[0x3c];
        __aeabi_memcpy(tmp, ((PaintCanvas*)(canvas))->CameraGetLocal(0), 0x3c);

        void *rng = *g_random;
        int rx = ((AbyssEngine::AERandom *)(rng))->nextInt();
        int ry = ((AbyssEngine::AERandom *)(rng))->nextInt();
        float tx = VectorSignedToFloat(rx - 20000, 0);
        float ty = VectorSignedToFloat(ry + 40000, 0);
        MatrixSetTranslation(localMatrix, tx, ty, 0.0f);
        this->cameraRotX = CutScene_initStartXRotMode2;
        MatrixSetRotation(localMatrix, 0.0f, 0.0f, 0.0f);
        ((PaintCanvas*)(canvas))->CameraSetLocal(0, *(const AbyssEngine::AEMath::Matrix*)(uintptr_t)this->cameraId74);

        long long pt = ((Status *)(*gStatus))->getPlayingTime();
        unsigned int *enemies = (unsigned int *)((Level *)(this->level))->getEnemies();
        if (pt != 0 && enemies != 0 && enemies[0] > 1) {
            char *base = (char *)enemies[1] + enemies[0] * 4;
            void *e0 = *(void **)(base - 8);
            if (e0 != 0 && *(void **)(base - 4) != 0) {
                float v[3];
                v[0] = VectorSignedToFloat(rx - 24000, 0);
                v[1] = 0.0f;
                v[2] = VectorSignedToFloat(ry + 0x9a4c, 0);
                (**(void (***)(void *, void *))e0)(e0, v);
                v[0] = VectorSignedToFloat(rx - 0x5b68, 0);
                v[2] = VectorSignedToFloat(ry + 0x96c8, 0);
                void *e1 = *(void **)((char *)enemies[1] + enemies[0] * 4 - 4);
                typedef void (*SetFn)(void *, void *);
                SetFn fn = *(SetFn *)(*(char **)e1 + 0x44);
                fn(e1, v);
            }
        }
    } else if (mode == 0x17) {
        this->turretGeom = 0;
        void *canvas = *g_canvas;
        ((PaintCanvas*)(canvas))->CameraCreate(&this->cameraId70);
        ((PaintCanvas*)(canvas))->CameraSetPerspective(0, CutScene_persp_fov, CutScene_persp_znear,
                                         CutScene_persp_zfar);
        ((PaintCanvas*)(canvas))->CameraSetCurrent(this->cameraId70);
        char tmp[0x3c];
        __aeabi_memcpy(tmp, ((PaintCanvas*)(canvas))->CameraGetLocal(0), 0x3c);
        MatrixSetRotation(localMatrix, 0.0f, 0.0f, 0.0f);
        MatrixSetTranslation(localMatrix, 0.0f, 0.0f, 0.0f);
        ((PaintCanvas*)(canvas))->CameraSetLocal(0, *(const AbyssEngine::AEMath::Matrix*)(uintptr_t)this->cameraId70);
        ((PaintCanvas*)(canvas))->TransformCreate(&this->transformId78);
        ((Level *)(this->level))->getEnemies();
        ((AEGeometry *)(0))->getPosition();
        ((PaintCanvas*)(canvas))->TransformGetLocal(0);
        MatrixSetTranslation(localMatrix, 0.0f, 0.0f, 0.0f);
        ((PaintCanvas*)(canvas))->TransformAddChild(this->transformId78, this->cameraId70);
        resetCamera();
        checkForTurret();
    } else if (mode == 4) {
        void *canvas = *g_canvas;
        ((PaintCanvas*)(canvas))->CameraCreate(&this->cameraId6c);
        ((PaintCanvas*)(canvas))->CameraSetPerspective(0, CutScene_persp_fov, CutScene_persp_znear,
                                         CutScene_persp_zfar);
        if (this->followCamera != 0) {
            ::operator delete(TargetFollowCamera_dtor(this->followCamera));
            this->followCamera = 0;
        }
        ((PaintCanvas*)(canvas))->CameraSetCurrent(this->cameraId6c);
        resetCamera();
        ((Status *)(*gStatus))->getSystem();
        int race = ((SolarSystem *)(long)((Status *)(*gStatus))->getSystem())->getRace();
        if (race == 3) {
            void *g = ::operator new(0xc0);
            new ((void*)g) AEGeometry((uint16_t)(0x36d6), (PaintCanvas*)canvas, false);
            this->geom2c = (AEGeometry *)g;
            void *t = ((PaintCanvas*)(canvas))->TransformGetTransform(0);
            ((AbyssEngine::Transform *)(t))->SetAnimationState((AbyssEngine::AnimationMode)0, (void *)0);
        } else {
            ((Status *)(*gStatus))->getSystem();
            if (((SolarSystem *)(long)((Status *)(*gStatus))->getSystem())->getRace() == 0) {
                void *g = ::operator new(0xc0);
                new ((void*)g) AEGeometry((uint16_t)(0x37c8), (PaintCanvas*)canvas, false);
                this->geom30 = (AEGeometry *)g;
                void *g2 = ::operator new(0xc0);
                new ((void*)g2) AEGeometry((uint16_t)(0x37c7), (PaintCanvas*)canvas, false);
                this->geom34 = (AEGeometry *)g2;
                ((AEGeometry *)(this->geom30))->addChild(u32(g2, 0xc));
                if (this->geom34 != 0)
                    [&]{ AEGeometry *g_ = (AEGeometry *)this->geom34; g_->~AEGeometry(); ::operator delete(g_); }();
                this->geom34 = 0;
            } else {
                ((Status *)(*gStatus))->getSystem();
                ((SolarSystem *)(long)((Status *)(*gStatus))->getSystem())->getRace();
            }
        }
    }

    void *root = ::operator new(0xc0);
    new ((void*)root) AEGeometry((PaintCanvas*)(*g_canvas));
    this->rootGeom = (AEGeometry *)root;
    ((AEGeometry*)(root))->setRotationOrder(2);

    this->accumLo = 0;
    this->accumHi = 0;
    this->animTimer7c = 0;
    this->animTimer80 = 0;
    this->fogTimer84 = 0;

    unsigned int now = (unsigned int)((ApplicationManager *)(*g_appManager))->GetCurrentTimeMillis();
    this->initialized = 1;
    this->field_0x40 = now & 0xffff;
    this->field_0x44 = 0;
    this->prevTimeLo = now & 0xffff;
    this->field_0x4c = 0;
}

extern "C" {
}

__attribute__((visibility("hidden"))) extern void **g_canvas;

// Perspective/fog constants recovered from the literal pool (kept as named
// globals; values resolved during a later matching pass).
extern "C" {
extern float CutScene_fogDensity_mode17;
extern float CutScene_fogDensity_mode4;
extern float CutScene_fogColor;
extern float CutScene_persp_fov_mode17;
extern float CutScene_persp_fov_mode4;
extern float CutScene_persp_znear;
extern float CutScene_persp_zfar;
}

void CutScene::resetCamera()
{
    int mode = this->mode;

    if (mode == 0x17) {
        ((Status *)(*gStatus))->getSystem();
        if (((SolarSystem *)(long)((Status *)(*gStatus))->getSystem())->getRace() == 1) {
            void *canvas = *g_canvas;
            ((PaintCanvas*)(canvas))->FogSetParameter(0x2601, 0, CutScene_fogColor,
                                        1.0f, CutScene_fogDensity_mode17);
            ((PaintCanvas*)(canvas))->FogEnable(1, 1);
        }
        void *canvas = *g_canvas;
        ((PaintCanvas*)(canvas))->CameraSetCurrent(this->cameraId70);
        ((PaintCanvas*)(canvas))->CameraSetPerspective(0, CutScene_persp_fov_mode17,
                                         CutScene_persp_znear, CutScene_persp_zfar);
        // Tail: notify the lead enemy ship.
        void *enemies = (void *)(intptr_t)((Level *)(this->level))->getEnemies();
        void **lead = *(void ***)((char *)enemies + 4);
        (void)lead;
        return;
    }

    if (mode != 4)
        return;

    ((Status *)(*gStatus))->getSystem();
    if (((SolarSystem *)(long)((Status *)(*gStatus))->getSystem())->getRace() == 1) {
        void *canvas = *g_canvas;
        ((PaintCanvas*)(canvas))->FogSetParameter(0x2601, 0, CutScene_fogColor,
                                    1.0f, CutScene_fogDensity_mode4);
        ((PaintCanvas*)(canvas))->FogEnable(1, 1);
    }
    void *canvas = *g_canvas;
    ((PaintCanvas*)(canvas))->CameraSetCurrent(this->cameraId6c);
    ((PaintCanvas*)(canvas))->CameraSetPerspective(0, CutScene_persp_fov_mode4,
                                     CutScene_persp_znear, CutScene_persp_zfar);
}

// Declaration of the loader's real method as a cast target. The full
// FileRead.h header cannot be included here because it carries placeholder
// struct definitions for Item/Ship that collide with the real class headers
// already included above.
class FileRead {
public:
    Array<Array<Vector *> *> *loadWeaponPositions(int32_t id);
};

extern "C" {
void FileRead_ctor(void *self);
void *FileRead_dtor(void *self);
}

__attribute__((visibility("hidden"))) extern void **g_canvas;
extern "C" {
extern float CutScene_turret_tx, CutScene_turret_ty, CutScene_turret_tz;
extern float CutScene_turret_rx, CutScene_turret_ry, CutScene_turret_rz;
extern float CutScene_turret_rx2, CutScene_turret_ry2, CutScene_turret_rz2;
}

void CutScene::checkForTurret()
{
    if (this->turretGeom != 0) {
        void *canvas = *g_canvas;
        void *enemies = (void *)(intptr_t)((Level *)(this->level))->getEnemies();
        unsigned int parent = u32(*(void **)(*(char **)((char *)enemies + 4) + 8), 0xc);
        unsigned int child = u32(this->turretGeom, 0xc);
        ((PaintCanvas*)(canvas))->TransformRemoveChild(parent, child);
    }

    void *ship = ((Status *)(*gStatus))->getShip();
    int *equip = (int *)((Ship*)(ship))->getEquipment(2);
    if (equip == 0 || equip[0] == 0)
        return;
    void *item = *(void **)(equip[1]);
    if (item == 0)
        return;

    int idx = ((Item *)(item))->getIndex();

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

    void *canvas = *g_canvas;

    void *geom0 = ::operator new(0xc0);
    new ((void*)geom0) AEGeometry((uint16_t)(id0), (PaintCanvas*)canvas, false);

    void *geom1 = ::operator new(0xc0);
    new ((void*)geom1) AEGeometry((uint16_t)(id1), (PaintCanvas*)canvas, false);
    ((AEGeometry*)(geom1))->setRotationOrder(2);

    if (child0 != -1) {
        void *c = ::operator new(0xc0);
        new ((void*)c) AEGeometry((uint16_t)((unsigned short)child0), (PaintCanvas*)canvas, false);
        ((AEGeometry *)(geom0))->addChild(u32(c, 0xc));
        ((AEGeometry *)c)->~AEGeometry();
        ::operator delete(c);
    }
    if (child1 != -1) {
        void *c = ::operator new(0xc0);
        new ((void*)c) AEGeometry((uint16_t)((unsigned short)child1), (PaintCanvas*)canvas, false);
        ((AEGeometry *)(geom1))->addChild(u32(c, 0xc));
        ((AEGeometry *)c)->~AEGeometry();
        ::operator delete(c);
    }
    if (child2 != -1) {
        void *c = ::operator new(0xc0);
        new ((void*)c) AEGeometry((uint16_t)((unsigned short)child2), (PaintCanvas*)canvas, false);
        ((AEGeometry *)(geom1))->addChild(u32(c, 0xc));
        ((AEGeometry *)c)->~AEGeometry();
        ::operator delete(c);
    }

    if (this->turretGeom != 0)
        [&]{ AEGeometry *g_ = (AEGeometry *)this->turretGeom; g_->~AEGeometry(); ::operator delete(g_); }();
    this->turretGeom = 0;

    void *root = ::operator new(0xc0);
    new ((void*)root) AEGeometry((PaintCanvas*)(canvas));
    this->turretGeom = (AEGeometry *)root;

    void *fr = ::operator new(1);
    FileRead_ctor(fr);
    ((Status *)(*gStatus))->getShip();
    int shipIdx = ((Ship *)(((Status *)(*gStatus))->getShip()))->getIndex();
    Array<Array<Vector *> *> *positions = ((FileRead *)(fr))->loadWeaponPositions(shipIdx);
    ::operator delete(FileRead_dtor(fr));

    Vector *posVec = (*(*positions)[0])[0];
    ((AEGeometry *)(geom0))->setPosition(*posVec);
    ((AEGeometry *)(geom1))->setPosition(*posVec);
    ((AEGeometry *)(geom0))->translate(CutScene_turret_tx, CutScene_turret_ty, CutScene_turret_tz);

    int idx2 = ((Item *)(item))->getIndex();
    if (idx2 >= 0xc6 && ((Item *)(item))->getIndex() <= 200) {
        ((AEGeometry *)(geom0))->rotate(CutScene_turret_rx, CutScene_turret_ry, CutScene_turret_rz);
        ((AEGeometry *)(geom1))->rotate(CutScene_turret_rx2, CutScene_turret_ry2, CutScene_turret_rz2);
    }

    ((AEGeometry *)(this->turretGeom))->addChild(u32(geom0, 0xc));
    ((AEGeometry *)(this->turretGeom))->addChild(u32(geom1, 0xc));
    void *enemies2 = (void *)(intptr_t)((Level *)(this->level))->getEnemies();
    ((AEGeometry *)(*(void **)(*(char **)((char *)enemies2 + 4) + 8)))->addChild(u32(this->turretGeom, 0xc));

    if (positions == 0)
        return;

    for (unsigned int i = 0; i < positions->size(); i++) {
        Array<Vector *> *slot = (*positions)[i];
        if (slot != 0) {
            // ArrayReleaseClasses_VectorPtr: delete each owned Vector, drop the entries.
            for (Vector *v : *slot)
                delete v;
            slot->clear();
            delete (*positions)[i];   // Array_VectorPtr_dtor: free the inner array object
            (*positions)[i] = 0;
        }
    }
    // ArrayReleaseClasses_ArrayVectorPtr: delete any remaining inner Array<Vector*>
    // objects (all already nulled above), then drop the outer entries.
    for (Array<Vector *> *e : *positions)
        delete e;
    positions->clear();
    positions->~Array<Array<Vector *> *>();   // Array_ArrayVectorPtr_dtor
    this->turretFinalize(positions);          // operator delete of the outer array object
}

// ---- update_tail / render2D_tail / renderBG_tail -----------------------------
// The decompiler split the trailing unconditional branch of update()/render2D()/
// renderBG() into its own thunk (a PLT/GOT veneer). Each simply delegates the
// cutscene's per-frame work to the embedded Level (field +0x0):
//   update()    advances the level world by the current frame delta (field +0x58),
//   render2D()  draws the level's 2D HUD overlay,
//   renderBG()  draws the level's animated background at the current time.

void CutScene::update_tail()
{
    Level *level = (Level *)this->level;
    if (level == 0)
        return;
    // Level::update(long long time, unsigned dt, int flag) — call through a typed
    // member-function pointer (the in-flight Level.h still declares update() void).
    unsigned int dt = this->frameDelta;
    typedef void (Level::*LevelUpdateFn)(long long, unsigned, int);
    LevelUpdateFn fn = (LevelUpdateFn)(&Level::update);
    (level->*fn)((long long)(int)dt, 0u, 0);
}

void CutScene::render2D_tail(Level *level)
{
    if (level != 0)
        level->render2D();
}

void CutScene::renderBG_tail(Level *level, uint32_t t)
{
    if (level != 0) {
        // The original passes the raw 32-bit time word in r1; renderBG() reads it
        // as its float time parameter.
        float ft;
        __builtin_memcpy(&ft, &t, sizeof ft);
        level->renderBG(ft);
    }
}

// ---- processAux -------------------------------------------------------------
// Peeled out of process()'s "race == 2" branch: a GOT-indirect call that pokes
// the cutscene's Level to refresh its enemy list (the cinematic ships) before
// each Transform::Update. Same as the inline ((Level*)field_0x0)->getEnemies()
// the sibling race branches use.
void CutScene::processAux()
{
    Level *level = (Level *)this->level;
    if (level != 0)
        level->getEnemies();
}

// ---- turretFinalize ---------------------------------------------------------
// The trailing tail-call of checkForTurret(): after the weapon-position table's
// ~Array() has run (leaving the array pointer in r0), this frees the now-empty
// Array<Array<Vector*>*> object. It is the operator delete half of the standard
// delete idiom that the decompiler split off from the destructor.
void CutScene::turretFinalize(void *positions)
{
    ::operator delete(positions);
}

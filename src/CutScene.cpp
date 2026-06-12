#include "gof2/CutScene.h"
#include "gof2/Status.h"
#include "gof2/AEGeometry.h"
#include "gof2/Item.h"
#include "gof2/LODManager.h"
#include "gof2/Transform.h"
#include "gof2/ApplicationManager.h"
#include "gof2/Level.h"
#include "gof2/PlayerEgo.h"
#include "gof2/PlayerFighter.h"
#include "gof2/Ship.h"


extern "C" void CutScene_update_tail(CutScene *self);
extern "C" void CutScene_render2D_tail(void *level);
extern "C" void CutScene_renderBG_tail(void *level, uint32_t bg);

// Status singleton holder (pc-rel global): the decompiler dropped the Status `self`
// argument on getSystem/getStation/getShip/getPlayingTime; recover it via *gStatus.
__attribute__((visibility("hidden"))) extern Status **gStatus;

// ---- _CutScene_98420.cpp ----
extern "C" void *AEGeometry_dtor(void *geom);              // 0x71fc8
extern "C" void *Level_dtor(void *level);                  // 0x71fd4
extern "C" void PaintCanvas_FogEnable(void *canvas, int a, int b); // 0x71fe0
extern "C" void ArrayReleaseClasses_AEGeometryPtr(void *arr);      // 0x71fec
extern "C" void *Array_AEGeometryPtr_dtor(void *arr);             // 0x717c4

__attribute__((visibility("hidden"))) extern void **g_canvasFog; // FogEnable target

CutScene::~CutScene()
{
    void *g = pp(this, 0x20);
    if (g != 0) ::operator delete(AEGeometry_dtor(g));
    pp(this, 0x20) = 0;

    g = pp(this, 0x64);
    if (g != 0) ::operator delete(AEGeometry_dtor(g));
    pp(this, 0x64) = 0;

    g = pp(this, 0x0);
    if (g != 0) ::operator delete(Level_dtor(g));
    pp(this, 0x0) = 0;

    PaintCanvas_FogEnable(*g_canvasFog, 0, 1);

    g = pp(this, 0x28);
    if (g != 0) ::operator delete(AEGeometry_dtor(g));
    pp(this, 0x28) = 0;

    g = pp(this, 0x2c);
    if (g != 0) ::operator delete(AEGeometry_dtor(g));
    pp(this, 0x2c) = 0;

    g = pp(this, 0x30);
    if (g != 0) ::operator delete(AEGeometry_dtor(g));
    pp(this, 0x30) = 0;

    g = pp(this, 0x34);
    if (g != 0) ::operator delete(AEGeometry_dtor(g));
    pp(this, 0x34) = 0;

    void *arr = pp(this, 0x38);
    if (arr != 0) {
        ArrayReleaseClasses_AEGeometryPtr(arr);
        arr = pp(this, 0x38);
        if (arr != 0) ::operator delete(Array_AEGeometryPtr_dtor(arr));
    }
    pp(this, 0x38) = 0;
}

// ---- isInitialized_98e80.cpp ----
uint8_t CutScene::isInitialized()
{
    return u8(this, 0x5c);
}

// ---- update_99664.cpp ----
// update(): single unconditional tail-call thunk (b.w trampoline).

void CutScene::update()
{
    return CutScene_update_tail(this);
}

// ---- render3D_9985c.cpp ----
__attribute__((visibility("hidden"))) extern void **g_appManager;

void CutScene::render3D()
{
    void *level = pp(this, 0x0);
    if (level != 0) {
        uint32_t t = (uint32_t)((ApplicationManager *)(*g_appManager))->GetElapsedTimeMillis();
        u32(this, 0x58) = t;
        // Level::update(long long time, unsigned dt, int flag) — real 3-arg signature
        // recovered from src/Level.cpp; the in-flight Level.h still declares update().
        // Call through a typed member-function pointer so this compiles regardless of
        // the transitional header and preserves the original arguments.
        {
            typedef void (Level::*LevelUpdateFn)(long long, unsigned, int);
            LevelUpdateFn fn = (LevelUpdateFn)(&Level::update);
            (((Level *)(pp(this, 0x0)))->*fn)((long long)(int)t, 0u, 0);
        }
        ((Level *)(pp(this, 0x0)))->render(u32(this, 0x58));
    }
    if (pp(this, 0x28) != 0) ((AEGeometry *)(pp(this, 0x28)))->render();
    if (pp(this, 0x2c) != 0) ((AEGeometry *)(pp(this, 0x2c)))->render();
    if (pp(this, 0x30) != 0) ((AEGeometry *)(pp(this, 0x30)))->render();
    if (pp(this, 0x34) != 0) ((AEGeometry *)(pp(this, 0x34)))->render();

    void *arr = pp(this, 0x38);
    if (arr != 0) {
        for (uint32_t i = 0; i < *(uint32_t *)arr; i++) {
            void **data = (void **)pp(arr, 0x4);
            ((AEGeometry *)(data[i]))->render();
            arr = pp(this, 0x38);
        }
    }
}

// ---- render2D_9984c.cpp ----
// render2D(): tail-call ext(*this) (the Level pointer at offset 0).

void CutScene::render2D()
{
    return CutScene_render2D_tail(pp(this, 0x0));
}

// ---- process_98e8c.cpp ----
extern "C" {
void TargetFollowCamera_update(void *cam);
float VectorSignedToFloat(int v, int mode);
void *PaintCanvas_CameraGetLocal(void *canvas);
void PaintCanvas_CameraSetLocal(void *canvas, void *matrix);
void *__aeabi_memcpy(void *dst, const void *src, unsigned int n);
void MatrixSetRotation(void *m, float x, float y, float z);
int Station_getIndex(void *station);
int SolarSystem_getRace();
void PaintCanvas_FogSetParameter(void *canvas, int mode, int a, float b, float c, float d);
void PaintCanvas_FogEnable(void *canvas, int a, int b);
void *PaintCanvas_TransformGetTransform(void *canvas);
void *PaintCanvas_TransformGetLocal(void *canvas);
void PaintCanvas_TransformSetLocal(void *canvas, void *matrix);
int AERandom_nextInt(void *rng);
void CutScene_processAux(void *self);
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
    void *self = (void *)(long)delta;
    if (u8(self, 0x5c) == 0)
        return;

    unsigned int now = (unsigned int)((ApplicationManager *)(*g_appManager))->GetCurrentTimeMillis();
    unsigned int prev = u32(self, 0x48);
    unsigned int dt = now - prev;
    // 64-bit accumulator at +0x50.
    unsigned int lo = u32(self, 0x50);
    u32(self, 0x50) = lo + dt;
    u32(self, 0x54) = u32(self, 0x54) + ((int)dt >> 31) + (lo + dt < lo ? 1u : 0u);
    u32(self, 0x58) = dt;
    u32(self, 0x40) = now & 0xffff;
    u32(self, 0x44) = 0;
    u32(self, 0x48) = now & 0xffff;
    u32(self, 0x4c) = 0;

    if (pp(self, 0x68) != 0)
        TargetFollowCamera_update(pp(self, 0x68));

    int mode = i32(self, 0x88);

    if (mode == 2) {
        float ft = VectorSignedToFloat(i32(self, 0x58), 0);
        void *canvas = *g_canvas;
        f32(self, 0x4) = f32(self, 0x4) + f32(self, 0x24) * ft;
        char tmp[0x3c];
        __aeabi_memcpy(tmp, PaintCanvas_CameraGetLocal(canvas), 0x3c);
        char mtx[0x3c];
        MatrixSetRotation(mtx, 0.0f, 0.0f, 0.0f);
        PaintCanvas_CameraSetLocal(canvas, pp(self, 0x74));

        long long pt = ((Status *)(*gStatus))->getPlayingTime();
        unsigned int *enemies = (unsigned int *)((Level *)(pp(self, 0x0)))->getEnemies();
        if (pt != 0 && enemies != 0 && enemies[0] > 1) {
            char *base = (char *)enemies[1] + enemies[0] * 4;
            void *e0 = *(void **)(base - 8);
            if (e0 != 0 && *(int *)((char *)e0 + 8) != 0) {
                AEGeometry *g0 = (AEGeometry *)*(void **)((char *)e0 + 8);
                float f = VectorSignedToFloat(i32(self, 0x58), 0);
                g0->translate(f * CutScene_proc_tx0, 0.0f, 0.0f);
                float f1 = VectorSignedToFloat(i32(self, 0x58), 0);
                float f2 = VectorSignedToFloat(-i32(self, 0x58), 0);
                g0->rotate(f1 * CutScene_proc_rx0, 0.0f, f2 * CutScene_proc_rz0);
            }
            void *e1 = *(void **)(base - 4);
            if (e1 != 0 && *(int *)((char *)e1 + 8) != 0) {
                AEGeometry *g1 = (AEGeometry *)*(void **)((char *)e1 + 8);
                float f = VectorSignedToFloat(i32(self, 0x58), 0);
                g1->translate(f * CutScene_proc_tx0, 0.0f, 0.0f);
                float f1 = VectorSignedToFloat(i32(self, 0x58), 0);
                float f2 = VectorSignedToFloat(-i32(self, 0x58), 0);
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
                kind = (unsigned int)SolarSystem_getRace() | 2;
            }
        }
        i32(self, 0x84) = i32(self, 0x58) + i32(self, 0x84);
        ((Status *)(*gStatus))->getSystem();
        if (SolarSystem_getRace() == 1) {
            PaintCanvas_FogSetParameter(*g_canvas, 0x2601, 0, CutScene_fogColorMode17,
                                        1.0f, CutScene_fogDensityMode17);
        } else if (kind == 2 && pp(self, 0x38) != 0) {
            void *canvas = *g_canvas;
            unsigned int *arr = (unsigned int *)pp(self, 0x38);
            unsigned int n = arr[0];
            for (unsigned int i = 0; i < n; i++) {
                void *t = PaintCanvas_TransformGetTransform(canvas);
                ((AbyssEngine::Transform *)(t))->Update((longlong)(unsigned)i32(self, 0x58), (bool)(unsigned char)i32(self, 0x58));
                arr = (unsigned int *)pp(self, 0x38);
                n = arr[0];
            }
            if (i32(self, 0x84) > 3000) {
                i32(self, 0x84) = 0;
                void *rng = *g_random;
                for (unsigned int i = 0; i < n; i++) {
                    if (AERandom_nextInt(rng) < 0x14) {
                        void *t = PaintCanvas_TransformGetTransform(canvas);
                        if (((AbyssEngine::Transform *)(t))->IsRunning() == 0) {
                            char *data = *(char **)((char *)pp(self, 0x38) + 4);
                            void *c0 = *(void **)(data + i * 4);
                            void *tr;
                            tr = PaintCanvas_TransformGetTransform(*(void **)((char *)c0 + 0xc));
                            ((AbyssEngine::Transform *)(tr))->SetAnimationState((AbyssEngine::AnimationMode)3, (void *)0);
                            tr = PaintCanvas_TransformGetTransform(*(void **)((char *)c0 + 0xc));
                            ((AbyssEngine::Transform *)(tr))->SetAnimationState((AbyssEngine::AnimationMode)1, (void *)0);
                            tr = PaintCanvas_TransformGetTransform(*(void **)((char *)c0 + 0x14));
                            ((AbyssEngine::Transform *)(tr))->SetAnimationState((AbyssEngine::AnimationMode)3, (void *)0);
                            tr = PaintCanvas_TransformGetTransform(*(void **)((char *)c0 + 0x14));
                            ((AbyssEngine::Transform *)(tr))->SetAnimationState((AbyssEngine::AnimationMode)1, (void *)0);
                        }
                    }
                    n = *(unsigned int *)pp(self, 0x38);
                }
            }
        }

        void *canvas = *g_canvas;
        ((Level *)(pp(self, 0x0)))->getEnemies();
        char tmp[0x3c];
        __aeabi_memcpy(tmp, PaintCanvas_TransformGetLocal(canvas), 0x3c);
        char mtx[0x3c];
        MatrixSetRotation(mtx, 0.0f, 0.0f, 0.0f);
        void *enemies = (void *)(intptr_t)((Level *)(pp(self, 0x0)))->getEnemies();
        PaintCanvas_TransformSetLocal(canvas,
            *(void **)(*(char **)(*(char **)((char *)enemies + 4) + 8) + 0xc));
        unsigned int *arr = (unsigned int *)((Level *)(pp(self, 0x0)))->getEnemies();
        for (unsigned int i = 0; i < arr[0]; i++) {
            void *en = (void *)(intptr_t)((Level *)(pp(self, 0x0)))->getEnemies();
            if (*(void **)(*(char **)((char *)en + 4) + i * 4) != 0) {
                ((Level *)(pp(self, 0x0)))->getEnemies();
                void *t = PaintCanvas_TransformGetTransform(canvas);
                ((AbyssEngine::Transform *)(t))->Update((longlong)(unsigned)i32(self, 0x58), (bool)(unsigned char)i32(self, 0x58));
            }
            arr = (unsigned int *)((Level *)(pp(self, 0x0)))->getEnemies();
        }
    } else if (mode == 4) {
        void *canvas = *g_canvas;
        unsigned int *arr = (unsigned int *)((Level *)(pp(self, 0x0)))->getEnemies();
        for (unsigned int i = 0; i < arr[0]; i++) {
            void *en = (void *)(intptr_t)((Level *)(pp(self, 0x0)))->getEnemies();
            if (*(void **)(*(char **)((char *)en + 4) + i * 4) != 0) {
                ((Level *)(pp(self, 0x0)))->getEnemies();
                void *t = PaintCanvas_TransformGetTransform(canvas);
                ((AbyssEngine::Transform *)(t))->Update((longlong)(unsigned)i32(self, 0x58), (bool)(unsigned char)i32(self, 0x58));
            }
            arr = (unsigned int *)((Level *)(pp(self, 0x0)))->getEnemies();
        }

        ((Status *)(*gStatus))->getSystem();
        int race = SolarSystem_getRace();
        if (race == 0) {
            void *t = PaintCanvas_TransformGetTransform(canvas);
            ((AbyssEngine::Transform *)(t))->Update((longlong)(unsigned)i32(self, 0x58), (bool)(unsigned char)i32(self, 0x58));
        } else {
            ((Status *)(*gStatus))->getSystem();
            int race1 = SolarSystem_getRace();
            if (race1 == 1) {
                PaintCanvas_FogSetParameter(canvas, 0x2601, 0, CutScene_fogColorMode4,
                                            1.0f, CutScene_fogDensityMode4);
                PaintCanvas_FogEnable(canvas, 1, 1);
                ((Level *)(pp(self, 0x0)))->getEnemies();
                ((Level *)(pp(self, 0x0)))->getEnemies();
                void *t = PaintCanvas_TransformGetTransform(canvas);
                ((AbyssEngine::Transform *)(t))->Update((longlong)(unsigned)i32(self, 0x58), (bool)(unsigned char)i32(self, 0x58));
                if (pp(self, 0x30) != 0) {
                    void *t2 = PaintCanvas_TransformGetTransform(canvas);
                    ((AbyssEngine::Transform *)(t2))->Update((longlong)(unsigned)i32(self, 0x58), (bool)(unsigned char)i32(self, 0x58));
                    int acc = i32(self, 0x58) + i32(self, 0x7c);
                    i32(self, 0x7c) = acc;
                    if (acc > 20000) {
                        i32(self, 0x7c) = 0;
                        if (AERandom_nextInt(*g_random) < 100) {
                            void *a = PaintCanvas_TransformGetTransform(canvas);
                            ((AbyssEngine::Transform *)(a))->SetAnimationState((AbyssEngine::AnimationMode)3, (void *)0);
                            a = PaintCanvas_TransformGetTransform(canvas);
                            ((AbyssEngine::Transform *)(a))->SetAnimationState((AbyssEngine::AnimationMode)1, (void *)0);
                        }
                    }
                }
                if (pp(self, 0x34) != 0) {
                    void *t2 = PaintCanvas_TransformGetTransform(canvas);
                    ((AbyssEngine::Transform *)(t2))->Update((longlong)(unsigned)i32(self, 0x58), (bool)(unsigned char)i32(self, 0x58));
                    int acc = i32(self, 0x58) + i32(self, 0x80);
                    i32(self, 0x80) = acc;
                    if (acc > 22000) {
                        i32(self, 0x80) = 0;
                        if (AERandom_nextInt(*g_random) < 100) {
                            void *a = PaintCanvas_TransformGetTransform(canvas);
                            ((AbyssEngine::Transform *)(a))->SetAnimationState((AbyssEngine::AnimationMode)3, (void *)0);
                            PaintCanvas_TransformGetTransform(canvas);
                            CutScene_processAux(self);
                            return;
                        }
                    }
                }
                CutScene_processAux(self);
                return;
            }
            ((Status *)(*gStatus))->getSystem();
            int race3 = SolarSystem_getRace();
            if (race3 == 3) {
                ((Level *)(pp(self, 0x0)))->getEnemies();
                ((Level *)(pp(self, 0x0)))->getEnemies();
                void *t = PaintCanvas_TransformGetTransform(canvas);
                ((AbyssEngine::Transform *)(t))->Update((longlong)(unsigned)i32(self, 0x58), (bool)(unsigned char)i32(self, 0x58));
                if (pp(self, 0x28) != 0) {
                    void *t2 = PaintCanvas_TransformGetTransform(canvas);
                    ((AbyssEngine::Transform *)(t2))->Update((longlong)(unsigned)i32(self, 0x58), (bool)(unsigned char)i32(self, 0x58));
                }
                void *t3 = PaintCanvas_TransformGetTransform(canvas);
                ((AbyssEngine::Transform *)(t3))->Update((longlong)(unsigned)i32(self, 0x58), (bool)(unsigned char)i32(self, 0x58));
                int acc7c = i32(self, 0x7c) + i32(self, 0x58);
                int acc80 = i32(self, 0x58) + i32(self, 0x80);
                i32(self, 0x7c) = acc7c;
                i32(self, 0x80) = acc80;
                if (acc7c > 1000 && pp(self, 0x28) != 0) {
                    i32(self, 0x7c) = 0;
                    if (AERandom_nextInt(*g_random) < 0x28) {
                        void *a = PaintCanvas_TransformGetTransform(canvas);
                        ((AbyssEngine::Transform *)(a))->SetAnimationState((AbyssEngine::AnimationMode)3, (void *)0);
                        a = PaintCanvas_TransformGetTransform(canvas);
                        ((AbyssEngine::Transform *)(a))->SetAnimationState((AbyssEngine::AnimationMode)1, (void *)0);
                    }
                    acc80 = i32(self, 0x80);
                }
                if (acc80 > 2000) {
                    i32(self, 0x80) = 0;
                    if (AERandom_nextInt(*g_random) < 0x1e) {
                        void *a = PaintCanvas_TransformGetTransform(canvas);
                        ((AbyssEngine::Transform *)(a))->SetAnimationState((AbyssEngine::AnimationMode)3, (void *)0);
                        PaintCanvas_TransformGetTransform(canvas);
                        CutScene_processAux(self);
                        return;
                    }
                }
                CutScene_processAux(self);
                return;
            }
            ((Status *)(*gStatus))->getSystem();
            if (SolarSystem_getRace() != 2)
                return;
            CutScene_processAux(self);
            CutScene_processAux(self);
            void *t = PaintCanvas_TransformGetTransform(canvas);
            ((AbyssEngine::Transform *)(t))->Update((longlong)(unsigned)i32(self, 0x58), (bool)(unsigned char)i32(self, 0x58));
            CutScene_processAux(self);
            CutScene_processAux(self);
            void *t2 = PaintCanvas_TransformGetTransform(canvas);
            ((AbyssEngine::Transform *)(t2))->Update((longlong)(unsigned)i32(self, 0x58), (bool)(unsigned char)i32(self, 0x58));
        }
    }
}

// ---- renderBG_99852.cpp ----
// renderBG(): tail-call ext(field_0, field_0x58).

void CutScene::renderBG()
{
    return CutScene_renderBG_tail(pp(this, 0x0), u32(this, 0x58));
}

// ---- CutScene_983e4.cpp ----
CutScene::CutScene(int param)
{
    i32(this, 0x18) = 0;
    i32(this, 0x1c) = 0;
    i32(this, 0x88) = param;
    i32(this, 0x0) = 0;
    i32(this, 0x64) = 0;
    i32(this, 0x68) = 0;
    i32(this, 0x6c) = -1;
    i32(this, 0x70) = -1;
    u8(this, 0x5c) = 0;
    i32(this, 0x38) = 0;
    __builtin_memset((char *)this + 0x8, 0, 16);
    __builtin_memset((char *)this + 0x28, 0, 16);
    u32(this, 0x24) = 0x3851b717u;
}

// ---- replacePlayerShip_99668.cpp ----
extern "C" {
void PaintCanvas_TransformRemoveChild(void *canvas, unsigned int parent, unsigned int child);
void *__aeabi_memcpy(void *dst, const void *src, unsigned int n);
void *Globals_getShipGroup(void *globals, int type, int slot, bool flag);
void *AEGeometry_setMatrix(void *matrix);
float VectorSignedToFloat(int v, int mode);
void *AEGeometry_dtor(void *geom);
}

__attribute__((visibility("hidden"))) extern void **g_canvas;
__attribute__((visibility("hidden"))) extern void **g_globals;
__attribute__((visibility("hidden"))) extern int CutScene_shipBankTable[];

void CutScene::replacePlayerShip(int a, int b)
{
    void *self = (void *)(long)a;

    if (((Level *)(pp(self, 0x0)))->getEnemies() == 0)
        return;

    void *enemies = (void *)(intptr_t)((Level *)(pp(self, 0x0)))->getEnemies();
    void *lead = *(void **)(*(char **)((char *)enemies + 4) + 8);
    void *oldGeom = lead;
    if (oldGeom != 0) {
        if (pp(self, 0x64) != 0) {
            void *canvas = *g_canvas;
            void *en2 = (void *)(intptr_t)((Level *)(pp(self, 0x0)))->getEnemies();
            unsigned int parent = u32(*(void **)(*(char **)((char *)en2 + 4) + 8), 0xc);
            unsigned int child = u32(pp(self, 0x64), 0xc);
            PaintCanvas_TransformRemoveChild(canvas, parent, child);
        }

        char matrix[0x3c];
        void *m = ((AEGeometry *)(oldGeom))->getMatrix();
        __aeabi_memcpy(matrix, m, 0x3c);

        void *grp = Globals_getShipGroup(*g_globals, b, 0, false);

        void *en3 = (void *)(intptr_t)((Level *)(pp(self, 0x0)))->getEnemies();
        *(void **)(*(char **)((char *)en3 + 4) + 8) = grp;

        void *en4 = (void *)(intptr_t)((Level *)(pp(self, 0x0)))->getEnemies();
        AEGeometry_setMatrix(*(void **)(*(char **)((char *)en4 + 4) + 8));

        void *en5 = (void *)(intptr_t)((Level *)(pp(self, 0x0)))->getEnemies();
        void *ship = *(void **)((char *)en5 + 4);
        float bank = VectorSignedToFloat(CutScene_shipBankTable[b], 0);
        void *obj = *(void **)ship;
        typedef void (*BankFn)(void *, int, float, int);
        BankFn fn = *(BankFn *)(*(char **)obj + 0x48);
        fn(obj, 0, bank, 0);

        void *en6 = (void *)(intptr_t)((Level *)(pp(self, 0x0)))->getEnemies();
        ((PlayerFighter *)(*(void **)((char *)en6 + 4)))->setExhaustVisible(false);

        ((LODManager *)(*(void **)self))->removeObject((AEGeometry *)oldGeom);
        ::operator delete(AEGeometry_dtor(oldGeom));
    }

    checkForTurret();
}

// ---- initialize_98628.cpp ----
extern "C" {
void Level_ctor(void *self, int mode);
void PaintCanvas_CameraCreate(void *canvas, unsigned int *out);
void PaintCanvas_CameraSetPerspective(void *canvas, float fov, float znear, float zfar);
void PaintCanvas_CameraSetCurrent(void *canvas, unsigned int cam);
void *PaintCanvas_CameraGetLocal(void *canvas);
void PaintCanvas_CameraSetLocal(void *canvas, void *matrix);
void PaintCanvas_TransformCreate(void *canvas, unsigned int *out);
void *PaintCanvas_TransformGetLocal(void *canvas);
void *PaintCanvas_TransformGetTransform(void *canvas);
void PaintCanvas_TransformAddChild(void *canvas, unsigned int parent, unsigned int child);
void *__aeabi_memcpy(void *dst, const void *src, unsigned int n);
int AERandom_nextInt(void *rng);
float VectorSignedToFloat(int v, int mode);
void MatrixSetTranslation(void *m, float x, float y, float z);
void MatrixSetRotation(void *m, float x, float y, float z);
int SolarSystem_getRace();
void AEGeometry_ctor_id(void *self, unsigned short id, void *canvas, bool flag);
void AEGeometry_ctor_default(void *self, void *canvas);
void AEGeometry_setRotationOrder(void *self, int order);
void *AEGeometry_dtor(void *self);
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
    void *level = pp(this, 0x0);
    if (level == 0) {
        level = ::operator new(0x2a0);
        Level_ctor(level, i32(this, 0x88));
        pp(this, 0x0) = level;
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
            level = pp(this, 0x0);
        } while (true);
    }

    void *player = (void *)(intptr_t)((Level *)(pp(this, 0x0)))->getPlayer();
    pp(this, 0x60) = player;
    if (player != 0)
        ((PlayerEgo *)(player))->setActive(true);

    ((Level *)(pp(this, 0x0)))->initParticleSystems();

    char localMatrix[0x3c];
    int mode = i32(this, 0x88);

    if (mode == 2) {
        void *canvas = *g_canvas;
        PaintCanvas_CameraCreate(canvas, (unsigned int *)((char *)this + 0x74));
        PaintCanvas_CameraSetPerspective(canvas, CutScene_persp_fov, CutScene_persp_znear,
                                         CutScene_persp_zfar);
        PaintCanvas_CameraSetCurrent(canvas, u32(this, 0x74));
        char tmp[0x3c];
        __aeabi_memcpy(tmp, PaintCanvas_CameraGetLocal(canvas), 0x3c);

        void *rng = *g_random;
        int rx = AERandom_nextInt(rng);
        int ry = AERandom_nextInt(rng);
        float tx = VectorSignedToFloat(rx - 20000, 0);
        float ty = VectorSignedToFloat(ry + 40000, 0);
        MatrixSetTranslation(localMatrix, tx, ty, 0.0f);
        f32(this, 0x4) = CutScene_initStartXRotMode2;
        MatrixSetRotation(localMatrix, 0.0f, 0.0f, 0.0f);
        PaintCanvas_CameraSetLocal(canvas, pp(this, 0x74));

        long long pt = ((Status *)(*gStatus))->getPlayingTime();
        unsigned int *enemies = (unsigned int *)((Level *)(pp(this, 0x0)))->getEnemies();
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
        pp(this, 0x64) = 0;
        void *canvas = *g_canvas;
        PaintCanvas_CameraCreate(canvas, (unsigned int *)((char *)this + 0x70));
        PaintCanvas_CameraSetPerspective(canvas, CutScene_persp_fov, CutScene_persp_znear,
                                         CutScene_persp_zfar);
        PaintCanvas_CameraSetCurrent(canvas, u32(this, 0x70));
        char tmp[0x3c];
        __aeabi_memcpy(tmp, PaintCanvas_CameraGetLocal(canvas), 0x3c);
        MatrixSetRotation(localMatrix, 0.0f, 0.0f, 0.0f);
        MatrixSetTranslation(localMatrix, 0.0f, 0.0f, 0.0f);
        PaintCanvas_CameraSetLocal(canvas, pp(this, 0x70));
        PaintCanvas_TransformCreate(canvas, (unsigned int *)((char *)this + 0x78));
        ((Level *)(pp(this, 0x0)))->getEnemies();
        ((AEGeometry *)(0))->getPosition();
        PaintCanvas_TransformGetLocal(canvas);
        MatrixSetTranslation(localMatrix, 0.0f, 0.0f, 0.0f);
        PaintCanvas_TransformAddChild(canvas, u32(this, 0x78), u32(this, 0x70));
        resetCamera();
        checkForTurret();
    } else if (mode == 4) {
        void *canvas = *g_canvas;
        PaintCanvas_CameraCreate(canvas, (unsigned int *)((char *)this + 0x6c));
        PaintCanvas_CameraSetPerspective(canvas, CutScene_persp_fov, CutScene_persp_znear,
                                         CutScene_persp_zfar);
        if (pp(this, 0x68) != 0) {
            ::operator delete(TargetFollowCamera_dtor(pp(this, 0x68)));
            pp(this, 0x68) = 0;
        }
        PaintCanvas_CameraSetCurrent(canvas, u32(this, 0x6c));
        resetCamera();
        ((Status *)(*gStatus))->getSystem();
        int race = SolarSystem_getRace();
        if (race == 3) {
            void *g = ::operator new(0xc0);
            AEGeometry_ctor_id(g, 0x36d6, canvas, false);
            pp(this, 0x2c) = g;
            void *t = PaintCanvas_TransformGetTransform(canvas);
            ((AbyssEngine::Transform *)(t))->SetAnimationState((AbyssEngine::AnimationMode)0, (void *)0);
        } else {
            ((Status *)(*gStatus))->getSystem();
            if (SolarSystem_getRace() == 0) {
                void *g = ::operator new(0xc0);
                AEGeometry_ctor_id(g, 0x37c8, canvas, false);
                pp(this, 0x30) = g;
                void *g2 = ::operator new(0xc0);
                AEGeometry_ctor_id(g2, 0x37c7, canvas, false);
                pp(this, 0x34) = g2;
                ((AEGeometry *)(pp(this, 0x30)))->addChild(u32(g2, 0xc));
                if (pp(this, 0x34) != 0)
                    ::operator delete(AEGeometry_dtor(pp(this, 0x34)));
                pp(this, 0x34) = 0;
            } else {
                ((Status *)(*gStatus))->getSystem();
                SolarSystem_getRace();
            }
        }
    }

    void *root = ::operator new(0xc0);
    AEGeometry_ctor_default(root, *g_canvas);
    pp(this, 0x20) = root;
    AEGeometry_setRotationOrder(root, 2);

    i32(this, 0x50) = 0;
    i32(this, 0x54) = 0;
    i32(this, 0x7c) = 0;
    i32(this, 0x80) = 0;
    i32(this, 0x84) = 0;

    unsigned int now = (unsigned int)((ApplicationManager *)(*g_appManager))->GetCurrentTimeMillis();
    u8(this, 0x5c) = 1;
    u32(this, 0x40) = now & 0xffff;
    u32(this, 0x44) = 0;
    u32(this, 0x48) = now & 0xffff;
    u32(this, 0x4c) = 0;
}

// ---- resetCamera_98514.cpp ----
extern "C" {
int SolarSystem_getRace();
void PaintCanvas_FogSetParameter(void *canvas, int mode, int a, float b, float c, float d);
void PaintCanvas_FogEnable(void *canvas, int a, int b);
void PaintCanvas_CameraSetCurrent(void *canvas, unsigned int cam);
void PaintCanvas_CameraSetPerspective(void *canvas, float fov, float znear, float zfar);
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
    int mode = i32(this, 0x88);

    if (mode == 0x17) {
        ((Status *)(*gStatus))->getSystem();
        if (SolarSystem_getRace() == 1) {
            void *canvas = *g_canvas;
            PaintCanvas_FogSetParameter(canvas, 0x2601, 0, CutScene_fogColor,
                                        1.0f, CutScene_fogDensity_mode17);
            PaintCanvas_FogEnable(canvas, 1, 1);
        }
        void *canvas = *g_canvas;
        PaintCanvas_CameraSetCurrent(canvas, u32(this, 0x70));
        PaintCanvas_CameraSetPerspective(canvas, CutScene_persp_fov_mode17,
                                         CutScene_persp_znear, CutScene_persp_zfar);
        // Tail: notify the lead enemy ship.
        void *enemies = (void *)(intptr_t)((Level *)(pp(this, 0x0)))->getEnemies();
        void **lead = *(void ***)((char *)enemies + 4);
        (void)lead;
        return;
    }

    if (mode != 4)
        return;

    ((Status *)(*gStatus))->getSystem();
    if (SolarSystem_getRace() == 1) {
        void *canvas = *g_canvas;
        PaintCanvas_FogSetParameter(canvas, 0x2601, 0, CutScene_fogColor,
                                    1.0f, CutScene_fogDensity_mode4);
        PaintCanvas_FogEnable(canvas, 1, 1);
    }
    void *canvas = *g_canvas;
    PaintCanvas_CameraSetCurrent(canvas, u32(this, 0x6c));
    PaintCanvas_CameraSetPerspective(canvas, CutScene_persp_fov_mode4,
                                     CutScene_persp_znear, CutScene_persp_zfar);
}

// ---- checkForTurret_98a68.cpp ----
extern "C" {
void PaintCanvas_TransformRemoveChild(void *canvas, unsigned int parent, unsigned int child);
void AEGeometry_ctor_id(void *self, unsigned short id, void *canvas, bool flag);
void AEGeometry_ctor_default(void *self, void *canvas);
void AEGeometry_setRotationOrder(void *self, int order);
void *AEGeometry_dtor(void *self);
void FileRead_ctor(void *self);
void *FileRead_dtor(void *self);
void *FileRead_loadWeaponPositions(void *self, int shipIndex);
int Ship_getIndex(void *ship);
void ArrayReleaseClasses_VectorPtr(void *arr);
void *Array_VectorPtr_dtor(void *arr);
void ArrayReleaseClasses_ArrayVectorPtr(void *arr);
void *Array_ArrayVectorPtr_dtor(void *arr);
void CutScene_turretFinalize();
}

__attribute__((visibility("hidden"))) extern void **g_canvas;
extern "C" {
extern float CutScene_turret_tx, CutScene_turret_ty, CutScene_turret_tz;
extern float CutScene_turret_rx, CutScene_turret_ry, CutScene_turret_rz;
extern float CutScene_turret_rx2, CutScene_turret_ry2, CutScene_turret_rz2;
}

void CutScene::checkForTurret()
{
    if (pp(this, 0x64) != 0) {
        void *canvas = *g_canvas;
        void *enemies = (void *)(intptr_t)((Level *)(pp(this, 0x0)))->getEnemies();
        unsigned int parent = u32(*(void **)(*(char **)((char *)enemies + 4) + 8), 0xc);
        unsigned int child = u32(pp(this, 0x64), 0xc);
        PaintCanvas_TransformRemoveChild(canvas, parent, child);
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
    AEGeometry_ctor_id(geom0, id0, canvas, false);

    void *geom1 = ::operator new(0xc0);
    AEGeometry_ctor_id(geom1, id1, canvas, false);
    AEGeometry_setRotationOrder(geom1, 2);

    if (child0 != -1) {
        void *c = ::operator new(0xc0);
        AEGeometry_ctor_id(c, (unsigned short)child0, canvas, false);
        ((AEGeometry *)(geom0))->addChild(u32(c, 0xc));
        ::operator delete(AEGeometry_dtor(c));
    }
    if (child1 != -1) {
        void *c = ::operator new(0xc0);
        AEGeometry_ctor_id(c, (unsigned short)child1, canvas, false);
        ((AEGeometry *)(geom1))->addChild(u32(c, 0xc));
        ::operator delete(AEGeometry_dtor(c));
    }
    if (child2 != -1) {
        void *c = ::operator new(0xc0);
        AEGeometry_ctor_id(c, (unsigned short)child2, canvas, false);
        ((AEGeometry *)(geom1))->addChild(u32(c, 0xc));
        ::operator delete(AEGeometry_dtor(c));
    }

    if (pp(this, 0x64) != 0)
        ::operator delete(AEGeometry_dtor(pp(this, 0x64)));
    pp(this, 0x64) = 0;

    void *root = ::operator new(0xc0);
    AEGeometry_ctor_default(root, canvas);
    pp(this, 0x64) = root;

    void *fr = ::operator new(1);
    FileRead_ctor(fr);
    ((Status *)(*gStatus))->getShip();
    int shipIdx = Ship_getIndex(((Status *)(*gStatus))->getShip());
    void *positions = FileRead_loadWeaponPositions(fr, shipIdx);
    ::operator delete(FileRead_dtor(fr));

    void *posVec = *(void **)(*(char **)(*(char **)((char *)positions + 4) + 8) + 4);
    ((AEGeometry *)(geom0))->setPosition(*(const Vector *)posVec);
    ((AEGeometry *)(geom1))->setPosition(*(const Vector *)posVec);
    ((AEGeometry *)(geom0))->translate(CutScene_turret_tx, CutScene_turret_ty, CutScene_turret_tz);

    int idx2 = ((Item *)(item))->getIndex();
    if (idx2 >= 0xc6 && ((Item *)(item))->getIndex() <= 200) {
        ((AEGeometry *)(geom0))->rotate(CutScene_turret_rx, CutScene_turret_ry, CutScene_turret_rz);
        ((AEGeometry *)(geom1))->rotate(CutScene_turret_rx2, CutScene_turret_ry2, CutScene_turret_rz2);
    }

    ((AEGeometry *)(pp(this, 0x64)))->addChild(u32(geom0, 0xc));
    ((AEGeometry *)(pp(this, 0x64)))->addChild(u32(geom1, 0xc));
    void *enemies2 = (void *)(intptr_t)((Level *)(pp(this, 0x0)))->getEnemies();
    ((AEGeometry *)(*(void **)(*(char **)((char *)enemies2 + 4) + 8)))->addChild(u32(pp(this, 0x64), 0xc));

    if (positions == 0)
        return;

    char *data = *(char **)((char *)positions + 4);
    for (unsigned int i = 0; i < *(unsigned int *)positions; i++) {
        void *slot = *(void **)(data + i * 4);
        if (slot != 0) {
            ArrayReleaseClasses_VectorPtr(slot);
            void *s2 = *(void **)(data + i * 4);
            if (s2 != 0)
                ::operator delete(Array_VectorPtr_dtor(s2));
            *(void **)(data + i * 4) = 0;
        }
    }
    ArrayReleaseClasses_ArrayVectorPtr(positions);
    Array_ArrayVectorPtr_dtor(positions);
    CutScene_turretFinalize();
}

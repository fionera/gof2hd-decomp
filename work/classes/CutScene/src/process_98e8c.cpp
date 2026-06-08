#include "class.h"

extern "C" {
unsigned int ApplicationManager_GetCurrentTimeMillis();
void TargetFollowCamera_update(void *cam);
float VectorSignedToFloat(int v, int mode);
void *PaintCanvas_CameraGetLocal(void *canvas);
void PaintCanvas_CameraSetLocal(void *canvas, void *matrix);
void __aeabi_memcpy(void *dst, const void *src, unsigned int n);
void MatrixSetRotation(void *m, float x, float y, float z);
long long Status_getPlayingTime();
void *Level_getEnemies(void *level);
void AEGeometry_translate(float x, float y, float z);
void AEGeometry_rotate(float x, float y, float z);
void *Status_getStation();
int Station_getIndex(void *station);
void *Status_getSystem();
int SolarSystem_getRace();
void PaintCanvas_FogSetParameter(void *canvas, int mode, int a, float b, float c, float d);
void PaintCanvas_FogEnable(void *canvas, int a, int b);
void *PaintCanvas_TransformGetTransform(void *canvas);
void *PaintCanvas_TransformGetLocal(void *canvas);
void PaintCanvas_TransformSetLocal(void *canvas, void *matrix);
void Transform_Update(void *t, bool dt);
int Transform_IsRunning(void *t);
void Transform_SetAnimationState(void *t, int a, int b);
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

    unsigned int now = ApplicationManager_GetCurrentTimeMillis();
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

        long long pt = Status_getPlayingTime();
        unsigned int *enemies = (unsigned int *)Level_getEnemies(pp(self, 0x0));
        if (pt != 0 && enemies != 0 && enemies[0] > 1) {
            char *base = (char *)enemies[1] + enemies[0] * 4;
            void *e0 = *(void **)(base - 8);
            if (e0 != 0 && *(int *)((char *)e0 + 8) != 0) {
                float f = VectorSignedToFloat(i32(self, 0x58), 0);
                AEGeometry_translate(f * CutScene_proc_tx0, 0.0f, f * CutScene_proc_tz0);
                float f1 = VectorSignedToFloat(i32(self, 0x58), 0);
                float f2 = VectorSignedToFloat(-i32(self, 0x58), 0);
                AEGeometry_rotate(f1 * CutScene_proc_rx0, 0.0f, f2 * CutScene_proc_rz0);
            }
            void *e1 = *(void **)(base - 4);
            if (e1 != 0 && *(int *)((char *)e1 + 8) != 0) {
                float f = VectorSignedToFloat(i32(self, 0x58), 0);
                AEGeometry_translate(f * CutScene_proc_tx0, 0.0f, CutScene_proc_tx0);
                float f1 = VectorSignedToFloat(i32(self, 0x58), 0);
                float f2 = VectorSignedToFloat(-i32(self, 0x58), 0);
                AEGeometry_rotate(f1 * CutScene_proc_rx0, 0.0f, f2 * CutScene_proc_rz1);
            }
        }
    } else if (mode == 0x17) {
        unsigned int kind;
        void *st = Status_getStation();
        if (Station_getIndex(st) == 0x65) {
            kind = 10;
        } else {
            st = Status_getStation();
            if (Station_getIndex(st) == 100) {
                kind = 7;
            } else {
                Status_getSystem();
                kind = (unsigned int)SolarSystem_getRace() | 2;
            }
        }
        i32(self, 0x84) = i32(self, 0x58) + i32(self, 0x84);
        Status_getSystem();
        if (SolarSystem_getRace() == 1) {
            PaintCanvas_FogSetParameter(*g_canvas, 0x2601, 0, CutScene_fogColorMode17,
                                        1.0f, CutScene_fogDensityMode17);
        } else if (kind == 2 && pp(self, 0x38) != 0) {
            void *canvas = *g_canvas;
            unsigned int *arr = (unsigned int *)pp(self, 0x38);
            unsigned int n = arr[0];
            for (unsigned int i = 0; i < n; i++) {
                void *t = PaintCanvas_TransformGetTransform(canvas);
                Transform_Update(t, (bool)(unsigned char)i32(self, 0x58));
                arr = (unsigned int *)pp(self, 0x38);
                n = arr[0];
            }
            if (i32(self, 0x84) > 3000) {
                i32(self, 0x84) = 0;
                void *rng = *g_random;
                for (unsigned int i = 0; i < n; i++) {
                    if (AERandom_nextInt(rng) < 0x14) {
                        void *t = PaintCanvas_TransformGetTransform(canvas);
                        if (Transform_IsRunning(t) == 0) {
                            char *data = *(char **)((char *)pp(self, 0x38) + 4);
                            void *c0 = *(void **)(data + i * 4);
                            void *tr;
                            tr = PaintCanvas_TransformGetTransform(*(void **)((char *)c0 + 0xc));
                            Transform_SetAnimationState(tr, 3, 0);
                            tr = PaintCanvas_TransformGetTransform(*(void **)((char *)c0 + 0xc));
                            Transform_SetAnimationState(tr, 1, 0);
                            tr = PaintCanvas_TransformGetTransform(*(void **)((char *)c0 + 0x14));
                            Transform_SetAnimationState(tr, 3, 0);
                            tr = PaintCanvas_TransformGetTransform(*(void **)((char *)c0 + 0x14));
                            Transform_SetAnimationState(tr, 1, 0);
                        }
                    }
                    n = *(unsigned int *)pp(self, 0x38);
                }
            }
        }

        void *canvas = *g_canvas;
        Level_getEnemies(pp(self, 0x0));
        char tmp[0x3c];
        __aeabi_memcpy(tmp, PaintCanvas_TransformGetLocal(canvas), 0x3c);
        char mtx[0x3c];
        MatrixSetRotation(mtx, 0.0f, 0.0f, 0.0f);
        void *enemies = Level_getEnemies(pp(self, 0x0));
        PaintCanvas_TransformSetLocal(canvas,
            *(void **)(*(char **)(*(char **)((char *)enemies + 4) + 8) + 0xc));
        unsigned int *arr = (unsigned int *)Level_getEnemies(pp(self, 0x0));
        for (unsigned int i = 0; i < arr[0]; i++) {
            void *en = Level_getEnemies(pp(self, 0x0));
            if (*(void **)(*(char **)((char *)en + 4) + i * 4) != 0) {
                Level_getEnemies(pp(self, 0x0));
                void *t = PaintCanvas_TransformGetTransform(canvas);
                Transform_Update(t, (bool)(unsigned char)i32(self, 0x58));
            }
            arr = (unsigned int *)Level_getEnemies(pp(self, 0x0));
        }
    } else if (mode == 4) {
        void *canvas = *g_canvas;
        unsigned int *arr = (unsigned int *)Level_getEnemies(pp(self, 0x0));
        for (unsigned int i = 0; i < arr[0]; i++) {
            void *en = Level_getEnemies(pp(self, 0x0));
            if (*(void **)(*(char **)((char *)en + 4) + i * 4) != 0) {
                Level_getEnemies(pp(self, 0x0));
                void *t = PaintCanvas_TransformGetTransform(canvas);
                Transform_Update(t, (bool)(unsigned char)i32(self, 0x58));
            }
            arr = (unsigned int *)Level_getEnemies(pp(self, 0x0));
        }

        Status_getSystem();
        int race = SolarSystem_getRace();
        if (race == 0) {
            void *t = PaintCanvas_TransformGetTransform(canvas);
            Transform_Update(t, (bool)(unsigned char)i32(self, 0x58));
        } else {
            Status_getSystem();
            int race1 = SolarSystem_getRace();
            if (race1 == 1) {
                PaintCanvas_FogSetParameter(canvas, 0x2601, 0, CutScene_fogColorMode4,
                                            1.0f, CutScene_fogDensityMode4);
                PaintCanvas_FogEnable(canvas, 1, 1);
                Level_getEnemies(pp(self, 0x0));
                Level_getEnemies(pp(self, 0x0));
                void *t = PaintCanvas_TransformGetTransform(canvas);
                Transform_Update(t, (bool)(unsigned char)i32(self, 0x58));
                if (pp(self, 0x30) != 0) {
                    void *t2 = PaintCanvas_TransformGetTransform(canvas);
                    Transform_Update(t2, (bool)(unsigned char)i32(self, 0x58));
                    int acc = i32(self, 0x58) + i32(self, 0x7c);
                    i32(self, 0x7c) = acc;
                    if (acc > 20000) {
                        i32(self, 0x7c) = 0;
                        if (AERandom_nextInt(*g_random) < 100) {
                            void *a = PaintCanvas_TransformGetTransform(canvas);
                            Transform_SetAnimationState(a, 3, 0);
                            a = PaintCanvas_TransformGetTransform(canvas);
                            Transform_SetAnimationState(a, 1, 0);
                        }
                    }
                }
                if (pp(self, 0x34) != 0) {
                    void *t2 = PaintCanvas_TransformGetTransform(canvas);
                    Transform_Update(t2, (bool)(unsigned char)i32(self, 0x58));
                    int acc = i32(self, 0x58) + i32(self, 0x80);
                    i32(self, 0x80) = acc;
                    if (acc > 22000) {
                        i32(self, 0x80) = 0;
                        if (AERandom_nextInt(*g_random) < 100) {
                            void *a = PaintCanvas_TransformGetTransform(canvas);
                            Transform_SetAnimationState(a, 3, 0);
                            PaintCanvas_TransformGetTransform(canvas);
                            CutScene_processAux(self);
                            return;
                        }
                    }
                }
                CutScene_processAux(self);
                return;
            }
            Status_getSystem();
            int race3 = SolarSystem_getRace();
            if (race3 == 3) {
                Level_getEnemies(pp(self, 0x0));
                Level_getEnemies(pp(self, 0x0));
                void *t = PaintCanvas_TransformGetTransform(canvas);
                Transform_Update(t, (bool)(unsigned char)i32(self, 0x58));
                if (pp(self, 0x28) != 0) {
                    void *t2 = PaintCanvas_TransformGetTransform(canvas);
                    Transform_Update(t2, (bool)(unsigned char)i32(self, 0x58));
                }
                void *t3 = PaintCanvas_TransformGetTransform(canvas);
                Transform_Update(t3, (bool)(unsigned char)i32(self, 0x58));
                int acc7c = i32(self, 0x7c) + i32(self, 0x58);
                int acc80 = i32(self, 0x58) + i32(self, 0x80);
                i32(self, 0x7c) = acc7c;
                i32(self, 0x80) = acc80;
                if (acc7c > 1000 && pp(self, 0x28) != 0) {
                    i32(self, 0x7c) = 0;
                    if (AERandom_nextInt(*g_random) < 0x28) {
                        void *a = PaintCanvas_TransformGetTransform(canvas);
                        Transform_SetAnimationState(a, 3, 0);
                        a = PaintCanvas_TransformGetTransform(canvas);
                        Transform_SetAnimationState(a, 1, 0);
                    }
                    acc80 = i32(self, 0x80);
                }
                if (acc80 > 2000) {
                    i32(self, 0x80) = 0;
                    if (AERandom_nextInt(*g_random) < 0x1e) {
                        void *a = PaintCanvas_TransformGetTransform(canvas);
                        Transform_SetAnimationState(a, 3, 0);
                        PaintCanvas_TransformGetTransform(canvas);
                        CutScene_processAux(self);
                        return;
                    }
                }
                CutScene_processAux(self);
                return;
            }
            Status_getSystem();
            if (SolarSystem_getRace() != 2)
                return;
            CutScene_processAux(self);
            CutScene_processAux(self);
            void *t = PaintCanvas_TransformGetTransform(canvas);
            Transform_Update(t, (bool)(unsigned char)i32(self, 0x58));
            CutScene_processAux(self);
            CutScene_processAux(self);
            void *t2 = PaintCanvas_TransformGetTransform(canvas);
            Transform_Update(t2, (bool)(unsigned char)i32(self, 0x58));
        }
    }
}

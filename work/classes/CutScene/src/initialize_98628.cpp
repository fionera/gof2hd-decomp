#include "class.h"

extern "C" {
void *operator_new(unsigned int size);
void operator_delete(void *p);
void Level_ctor(void *self, int mode);
int Level_init(void *self);
void *Level_getPlayer(void *self);
void PlayerEgo_setActive(void *ego, bool active);
void Level_initParticleSystems(void *self);
void *Level_getEnemies(void *self);
void PaintCanvas_CameraCreate(void *canvas, unsigned int *out);
void PaintCanvas_CameraSetPerspective(void *canvas, float fov, float znear, float zfar);
void PaintCanvas_CameraSetCurrent(void *canvas, unsigned int cam);
void *PaintCanvas_CameraGetLocal(void *canvas);
void PaintCanvas_CameraSetLocal(void *canvas, void *matrix);
void PaintCanvas_TransformCreate(void *canvas, unsigned int *out);
void *PaintCanvas_TransformGetLocal(void *canvas);
void *PaintCanvas_TransformGetTransform(void *canvas);
void PaintCanvas_TransformAddChild(void *canvas, unsigned int parent, unsigned int child);
void Transform_SetAnimationState(void *t, int a, int b);
void __aeabi_memcpy(void *dst, const void *src, unsigned int n);
int AERandom_nextInt(void *rng);
float VectorSignedToFloat(int v, int mode);
void MatrixSetTranslation(void *m, float x, float y, float z);
void MatrixSetRotation(void *m, float x, float y, float z);
long long Status_getPlayingTime();
void *Status_getSystem();
int SolarSystem_getRace();
void AEGeometry_ctor_id(void *self, unsigned short id, void *canvas, bool flag);
void AEGeometry_ctor_default(void *self, void *canvas);
void AEGeometry_setRotationOrder(void *self, int order);
void AEGeometry_addChild(void *self, unsigned int child);
void *AEGeometry_dtor(void *self);
void *AEGeometry_getPosition(void *self);
void *TargetFollowCamera_dtor(void *self);
unsigned int ApplicationManager_GetCurrentTimeMillis();
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
        level = operator_new(0x2a0);
        Level_ctor(level, i32(this, 0x88));
        pp(this, 0x0) = level;
    }
    do {
        if (Level_init(level) != 0)
            break;
        level = pp(this, 0x0);
    } while (true);

    void *player = Level_getPlayer(pp(this, 0x0));
    pp(this, 0x60) = player;
    if (player != 0)
        PlayerEgo_setActive(player, true);

    Level_initParticleSystems(pp(this, 0x0));

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

        long long pt = Status_getPlayingTime();
        unsigned int *enemies = (unsigned int *)Level_getEnemies(pp(this, 0x0));
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
        Level_getEnemies(pp(this, 0x0));
        AEGeometry_getPosition(0);
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
            operator_delete(TargetFollowCamera_dtor(pp(this, 0x68)));
            pp(this, 0x68) = 0;
        }
        PaintCanvas_CameraSetCurrent(canvas, u32(this, 0x6c));
        resetCamera();
        Status_getSystem();
        int race = SolarSystem_getRace();
        if (race == 3) {
            void *g = operator_new(0xc0);
            AEGeometry_ctor_id(g, 0x36d6, canvas, false);
            pp(this, 0x2c) = g;
            void *t = PaintCanvas_TransformGetTransform(canvas);
            Transform_SetAnimationState(t, 0, 0);
        } else {
            Status_getSystem();
            if (SolarSystem_getRace() == 0) {
                void *g = operator_new(0xc0);
                AEGeometry_ctor_id(g, 0x37c8, canvas, false);
                pp(this, 0x30) = g;
                void *g2 = operator_new(0xc0);
                AEGeometry_ctor_id(g2, 0x37c7, canvas, false);
                pp(this, 0x34) = g2;
                AEGeometry_addChild(pp(this, 0x30), u32(g2, 0xc));
                if (pp(this, 0x34) != 0)
                    operator_delete(AEGeometry_dtor(pp(this, 0x34)));
                pp(this, 0x34) = 0;
            } else {
                Status_getSystem();
                SolarSystem_getRace();
            }
        }
    }

    void *root = operator_new(0xc0);
    AEGeometry_ctor_default(root, *g_canvas);
    pp(this, 0x20) = root;
    AEGeometry_setRotationOrder(root, 2);

    i32(this, 0x50) = 0;
    i32(this, 0x54) = 0;
    i32(this, 0x7c) = 0;
    i32(this, 0x80) = 0;
    i32(this, 0x84) = 0;

    unsigned int now = ApplicationManager_GetCurrentTimeMillis();
    u8(this, 0x5c) = 1;
    u32(this, 0x40) = now & 0xffff;
    u32(this, 0x44) = 0;
    u32(this, 0x48) = now & 0xffff;
    u32(this, 0x4c) = 0;
}

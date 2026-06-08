#include "class.h"

struct Matrix;
struct SolarSystem;
struct StarSystem;
struct Engine;

// PC-relative cookie + the PaintCanvas singleton + a few float-constant table pointers.
__attribute__((visibility("hidden"))) extern int    *g_rbg_stack;    // [DAT_000d46fc]
__attribute__((visibility("hidden"))) extern unsigned *g_rbg_canvas;  // [DAT_000d4700]
__attribute__((visibility("hidden"))) extern void   **g_rbg_rng;      // [DAT_000d4b04]
__attribute__((visibility("hidden"))) extern int    **g_rbg_engine;   // [DAT_000d4b0c]

extern "C" {
void PaintCanvas_SetColor_rbg(unsigned c);
void PaintCanvas_BeginBG_rbg();
void PaintCanvas_EndBG_rbg(unsigned c);
void PaintCanvas_CameraGetCurrent_rbg();
void PaintCanvas_CameraGetLocal_rbg(unsigned c);
void PaintCanvas_SetWorldViewMatrix_rbg(unsigned c);
void PaintCanvas_SetTexture_rbg(unsigned c, unsigned tex);
void PaintCanvas_SetBlendMode_rbg(unsigned c, int mode);
void PaintCanvas_DrawMesh_rbg(unsigned c, unsigned mesh);
void PaintCanvas_DrawTransform_rbg(unsigned c, Matrix *m);
int  PaintCanvas_TransformGetTransform_rbg(unsigned c);
void Matrix_getInverse_rbg(Matrix *dst);
void Matrix_assign_rbg(Matrix *dst, Matrix *src);
void Matrix_mulEq_rbg(Matrix *dst, Matrix *rhs);
void Transform_Update_rbg(int xform, int flag);
int  Status_inAlienOrbit_rbg();
int  Status_getSystem_rbg();
int  SolarSystem_getIndex_rbg();
int  Status_inSupernovaSystem_rbg();
int  Status_getCurrentCampaignMission_rbg();
void StarSystem_render_rbg(StarSystem *s);
void StarSystem_getLightDirection_rbg(void *dst);
int  AERandom_nextInt_rbg(int rng);
void Engine_SetModelMatrix_rbg(Matrix *m);
void Engine_LightSetLight_rbg(Engine *e, int v);
void Engine_GlEnable_rbg(unsigned e, int flag);
void Engine_LightEnable_rbg(int flag);
// Builds the rotated skybox basis (uses corrupted SIMD cross-products in the original) and writes
// it into the level's skybox matrix at self+0x1d0. `mode` selects the alien-orbit variant.
void Level_rbg_buildSkyMatrix(char *self, int mode, float spin);
}

// Level::renderBG(float t) — draws the skybox, nebula, planet rings and supernova glow.
extern "C" void Level_renderBG(Level *thisptr, float t)
{
    char *self = (char *)thisptr;
    unsigned canvas = *g_rbg_canvas;

    PaintCanvas_SetColor_rbg(canvas);
    PaintCanvas_BeginBG_rbg();
    PaintCanvas_CameraGetCurrent_rbg();
    PaintCanvas_CameraGetLocal_rbg(canvas);

    Matrix *sky = (Matrix *)(self + 0x1d0);
    Matrix_getInverse_rbg(sky);
    *(int *)(self + 0x1ec) = 0;
    *(int *)(self + 0x1dc) = 0;
    *(int *)(self + 0x1fc) = 0;

    bool alienRing = false;
    if (Status_inAlienOrbit_rbg() == 0) {
        Status_getSystem_rbg();
        if (SolarSystem_getIndex_rbg() == 0x1b)
            alienRing = true;
    }
    Level_rbg_buildSkyMatrix(self, alienRing ? 1 : 0, t);
    Matrix_mulEq_rbg(sky, (Matrix *)(self + 0x20c));

    PaintCanvas_SetWorldViewMatrix_rbg(canvas);
    PaintCanvas_SetTexture_rbg(canvas, *(unsigned *)(self + 0x19c));
    PaintCanvas_SetBlendMode_rbg(canvas, 0);
    PaintCanvas_DrawMesh_rbg(canvas, *(unsigned *)(self + 8));
    PaintCanvas_SetTexture_rbg(canvas, *(unsigned *)(self + 0x198));
    PaintCanvas_SetBlendMode_rbg(canvas, 2);
    PaintCanvas_DrawMesh_rbg(canvas, *(unsigned *)(self + 4));

    // optional far cloud layer.
    if (*(int *)(self + 0x1b4) != -1) {
        PaintCanvas_SetTexture_rbg(canvas, *(unsigned *)(self + 0x1b8));
        PaintCanvas_SetBlendMode_rbg(canvas, 1);
        PaintCanvas_CameraGetCurrent_rbg();
        PaintCanvas_CameraGetLocal_rbg(canvas);
        Matrix_getInverse_rbg(sky);
        Matrix_assign_rbg(sky, sky);
        *(int *)(self + 0x1ec) = 0;
        *(int *)(self + 0x1dc) = 0;
        *(int *)(self + 0x1fc) = 0;
        Matrix_getInverse_rbg(sky);
        PaintCanvas_DrawTransform_rbg(canvas, *(Matrix **)(self + 0x1b4));
    }

    StarSystem_render_rbg(*(StarSystem **)(self + 0xec));

    // supernova glow billboards.
    if (Status_inSupernovaSystem_rbg() != 0 && *(int *)(self + 0xc) != -1) {
        int camp = Status_getCurrentCampaignMission_rbg();
        PaintCanvas_SetTexture_rbg(canvas, *(unsigned *)(self + 0x1a0));
        PaintCanvas_SetBlendMode_rbg(canvas, 2);
        float scale = (0x6a < camp) ? 1.5f : 1.0f;
        int flag = (int)(scale * t);
        int xf = PaintCanvas_TransformGetTransform_rbg(canvas);
        Transform_Update_rbg(xf, flag);
        Matrix_getInverse_rbg(sky);
        PaintCanvas_DrawTransform_rbg(canvas, *(Matrix **)(self + 0x10));
        xf = PaintCanvas_TransformGetTransform_rbg(canvas);
        Transform_Update_rbg(xf, flag);
        PaintCanvas_DrawTransform_rbg(canvas, *(Matrix **)(self + 0x18));
    }

    // rotating planet ring.
    if (*(int *)(self + 0x1bc) != -1) {
        PaintCanvas_SetTexture_rbg(canvas, *(unsigned *)(self + 0x1c0));
        PaintCanvas_SetBlendMode_rbg(canvas, 2);
        PaintCanvas_CameraGetCurrent_rbg();
        PaintCanvas_CameraGetLocal_rbg(canvas);
        Matrix_getInverse_rbg(sky);
        Matrix_assign_rbg(sky, sky);
        int xf = PaintCanvas_TransformGetTransform_rbg(canvas);
        int before = *(int *)(xf + 0x110);
        int xf2 = PaintCanvas_TransformGetTransform_rbg(canvas);
        Transform_Update_rbg(xf2, (int)t);
        if (*(int *)(xf + 0x110) < before) {
            // re-randomize the ring tilt — corrupted SIMD in the original; rebuild via helper.
            int rng = *(int *)g_rbg_rng;
            AERandom_nextInt_rbg(rng);
            AERandom_nextInt_rbg(rng);
            AERandom_nextInt_rbg(rng);
            Level_rbg_buildSkyMatrix(self, 2, t);
        }
        Matrix_mulEq_rbg(sky, (Matrix *)(self + 0x248));
        *(int *)(self + 0x1dc) = 0;
        *(int *)(self + 0x1ec) = 0;
        *(int *)(self + 0x1fc) = 0;
        Matrix_getInverse_rbg(sky);
        PaintCanvas_DrawTransform_rbg(canvas, *(Matrix **)(self + 0x1bc));
    }

    // supernova flare mesh (when the explosion timeline is past its trigger).
    if (*(char *)(self + 0x289) != 0 &&
        1.0f <= *(float *)(*(int *)g_rbg_engine + 0x28)) {
        PaintCanvas_CameraGetCurrent_rbg();
        PaintCanvas_CameraGetLocal_rbg(canvas);
        Matrix_getInverse_rbg(sky);
        Matrix_assign_rbg(sky, sky);
        *(int *)(self + 0x1ec) = 0;
        *(int *)(self + 0x1dc) = 0;
        *(int *)(self + 0x1fc) = 0;
        PaintCanvas_SetWorldViewMatrix_rbg(canvas);
        PaintCanvas_SetColor_rbg(canvas);
        Engine *eng = *(Engine **)(canvas + 0x34);
        Engine_SetModelMatrix_rbg((Matrix *)eng);
        PaintCanvas_SetTexture_rbg(canvas, *(unsigned *)(self + 0x1c4));
        PaintCanvas_SetBlendMode_rbg(canvas, 8);
        Engine_LightSetLight_rbg(*(Engine **)(canvas + 0x34), 0x4000);
        Engine_GlEnable_rbg(*(unsigned *)(canvas + 0x34), 0);
        PaintCanvas_DrawMesh_rbg(canvas, *(unsigned *)(self + 0x1cc));
        Engine_GlEnable_rbg(*(unsigned *)(canvas + 0x34), 0);
        Engine_LightEnable_rbg(*(int *)(canvas + 0x34));
    }

    PaintCanvas_EndBG_rbg(canvas);
}

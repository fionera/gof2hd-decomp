#include "class.h"

extern "C" {
void *Status_getSystem();
int SolarSystem_getRace();
void PaintCanvas_FogSetParameter(void *canvas, int mode, int a, float b, float c, float d);
void PaintCanvas_FogEnable(void *canvas, int a, int b);
void PaintCanvas_CameraSetCurrent(void *canvas, unsigned int cam);
void PaintCanvas_CameraSetPerspective(void *canvas, float fov, float znear, float zfar);
void *Level_getEnemies(void *level);
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
        Status_getSystem();
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
        void *enemies = Level_getEnemies(pp(this, 0x0));
        void **lead = *(void ***)((char *)enemies + 4);
        (void)lead;
        return;
    }

    if (mode != 4)
        return;

    Status_getSystem();
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

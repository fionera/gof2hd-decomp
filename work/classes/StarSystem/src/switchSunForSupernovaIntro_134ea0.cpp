#include "class.h"

typedef void *(*GetTransformFn)(void *canvas, int transform_id);
typedef void (*SetTransformModeFn)(void *transform, int mode, int value);

__attribute__((visibility("hidden"))) extern void **g_StarSystem_intro_canvas;
__attribute__((visibility("hidden"))) extern GetTransformFn g_StarSystem_intro_getTransform;
__attribute__((visibility("hidden"))) extern SetTransformModeFn g_StarSystem_intro_setTransformMode;
__attribute__((visibility("hidden"))) extern void **g_StarSystem_intro_status;
__attribute__((visibility("hidden"))) extern uint32_t g_StarSystem_intro_colors[];

extern "C" void PaintCanvas_TextureCreate(void *canvas, uint32_t texture, void *slot, int flags);
extern "C" void AEGeometry_setMesh(AEGeometry *geom, uint32_t mesh);
extern "C" void AEGeometry_getScaling(char *out, AEGeometry *geom);
extern "C" void Vector_scale(char *out, const char *in, uint32_t scale_bits);
extern "C" void AEGeometry_setScaling(AEGeometry *geom, const char *scale);
extern "C" void AEGeometry_setScaling1(AEGeometry *geom, uint32_t scale_bits);
extern "C" void *Status_getSystem(void *status);
extern "C" uint32_t SolarSystem_getIndex(void *system);

extern "C" void StarSystem_switchSunForSupernovaIntro(StarSystem *self)
{
    void **canvasHolder = g_StarSystem_intro_canvas;
    char scaled[12];
    char current[12];
    void *canvas = *canvasHolder;
    PaintCanvas_TextureCreate(canvas, 0x2df3, P(P(self, 0x14), 4), 0);
    PaintCanvas_TextureCreate(*canvasHolder, 0x2df4, (char *)self + 0x10, 0);

    AEGeometry_setMesh((AEGeometry *)P(self, 0x40), 0x2df2);
    AEGeometry *streak = (AEGeometry *)P(self, 0x40);
    AEGeometry_getScaling(current, streak);
    Vector_scale(scaled, current, 0x40a00000);
    AEGeometry_setScaling(streak, scaled);

    AEGeometry *sun = *(AEGeometry **)P(P(self, 0x1c), 4);
    AEGeometry_setMesh(sun, 0x2df1);
    AEGeometry_setScaling1(*(AEGeometry **)P(P(self, 0x1c), 4), 0x3f2fc800);

    GetTransformFn getTransform = g_StarSystem_intro_getTransform;
    int transformId = I(*(void **)P(P(self, 0x1c), 4), 0x0c);
    void *transform = getTransform(*canvasHolder, transformId);
    SetTransformModeFn setTransformMode = g_StarSystem_intro_setTransformMode;
    setTransformMode(transform, 0, 0);
    transformId = I(*(void **)P(P(self, 0x1c), 4), 0x0c);
    setTransformMode(getTransform(*canvasHolder, transformId), 3, 0);
    transformId = I(*(void **)P(P(self, 0x1c), 4), 0x0c);
    setTransformMode(getTransform(*canvasHolder, transformId), 1, 0);

    void *system = Status_getSystem(*g_StarSystem_intro_status);
    U(self, 0x3c) = g_StarSystem_intro_colors[SolarSystem_getIndex(system)];
}

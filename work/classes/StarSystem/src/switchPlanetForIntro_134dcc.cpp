#include "class.h"

__attribute__((visibility("hidden"))) extern int *g_StarSystem_planet_guard;
__attribute__((visibility("hidden"))) extern void **g_StarSystem_planet_canvas;

extern "C" void PaintCanvas_TextureCreate(void *canvas, uint32_t texture, void *slot, int flags);
extern "C" void AEGeometry_getScaling(char *out, AEGeometry *geom);
extern "C" void Vector_scale(char *out, const char *in, float scale);
extern "C" void AEGeometry_setScaling(AEGeometry *geom, const char *scale);

extern "C" void StarSystem_switchPlanetForIntro(StarSystem *self)
{
    char scaled[12];
    char current[12];
    int flags = 0;
    PaintCanvas_TextureCreate(*g_StarSystem_planet_canvas, 0x273a,
                              (char *)P(P(self, 0x14), 4) + U(self, 0x50) * 4, flags);
    AEGeometry_getScaling(current, ((AEGeometry **)P(P(self, 0x1c), 4))[U(self, 0x50)]);
    Vector_scale(scaled, current, 2.0f);
    AEGeometry_setScaling(((AEGeometry **)P(P(self, 0x1c), 4))[U(self, 0x50)], scaled);
}

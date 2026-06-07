#include "class.h"

__attribute__((visibility("hidden"))) extern void **g_StarSystem_render2d_canvas;

extern "C" void *PaintCanvas_CameraGetCurrent(void *canvas);
extern "C" void *PaintCanvas_CameraGetLocal(void *canvas, void *current);
extern "C" void LensFlare_update(void *flare, int mode);
extern "C" int PaintCanvas_GetScreenPosition(void *canvas, char *world, char *screen);
extern "C" void LensFlare_render2D(void *flare, float x, float y, float z, int color);

extern "C" void StarSystem_render2D(StarSystem *self)
{
    char posBytes[12];
    void *flare = P(self, 0x2c);
    if (flare != 0) {
        void **canvasHolder = g_StarSystem_render2d_canvas;
        void *canvas = *canvasHolder;
        void *current = PaintCanvas_CameraGetCurrent(canvas);
        void *local = PaintCanvas_CameraGetLocal(canvas, current);
        float *pos = (float *)posBytes;
        pos[0] = FL(local, 0x0c) + FL(self, 0x30) * 65536.0f;
        pos[1] = FL(local, 0x1c) + FL(self, 0x34) * 65536.0f;
        pos[2] = FL(local, 0x2c) + FL(self, 0x38) * 65536.0f;
        LensFlare_update(P(self, 0x2c), 0);
        if (PaintCanvas_GetScreenPosition(*canvasHolder, posBytes, posBytes) != 0) {
            LensFlare_render2D(P(self, 0x2c), pos[0], pos[1], pos[2], I(self, 0x3c));
        }
    }
}

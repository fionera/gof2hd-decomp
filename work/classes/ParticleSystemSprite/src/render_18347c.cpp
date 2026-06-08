#include "class.h"

// ParticleSystemSprite::render(PaintCanvas*, unsigned int handle)
// Camera-relative draw: snapshots the current camera's local matrix twice (the engine swaps
// the active camera between the two queries) and forwards both 15-float matrices to the
// sprite-system draw entry. A handle of 0xffffffff means "no system" and is skipped.

struct ParticleSystemSprite {
    void render(void *canvas, unsigned int handle);
};

extern "C" void  PaintCanvas_CameraGetCurrent();
extern "C" float *PaintCanvas_CameraGetLocal(unsigned int canvas);
extern "C" void  PaintCanvas_DrawSpriteSystemCam(
    void *canvas, unsigned int handle,
    float a00, float a01, float a02, float a03, float a04, float a05, float a06, float a07,
    float a08, float a09, float a10, float a11, float a12, float a13, float a14,
    float b00, float b01, float b02, float b03, float b04, float b05, float b06, float b07,
    float b08, float b09, float b10, float b11, float b12, float b13, float b14);

void ParticleSystemSprite::render(void *canvas, unsigned int handle)
{
    if (handle == 0xffffffffu)
        return;

    PaintCanvas_CameraGetCurrent();
    float *a = PaintCanvas_CameraGetLocal((unsigned int)(unsigned long)canvas);
    float a0=a[0],a1=a[1],a2=a[2],a3=a[3],a4=a[4],a5=a[5],a6=a[6],a7=a[7];
    float a8=a[8],a9=a[9],a10=a[10],a11=a[11],a12=a[12],a13=a[13],a14=a[14];

    PaintCanvas_CameraGetCurrent();
    float *b = PaintCanvas_CameraGetLocal((unsigned int)(unsigned long)canvas);

    PaintCanvas_DrawSpriteSystemCam(canvas, handle,
        a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,
        b[0],b[1],b[2],b[3],b[4],b[5],b[6],b[7],b[8],b[9],b[10],b[11],b[12],b[13],b[14]);
}

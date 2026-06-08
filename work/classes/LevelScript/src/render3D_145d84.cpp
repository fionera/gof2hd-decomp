#include "class.h"

typedef void (*RenderProc)(void *);

extern "C" void AEGeometry_render(void *geometry);
extern "C" int Explosion_isPlaying(void *explosion);
extern "C" void Explosion_render(void *explosion);

static volatile RenderProc gRenderProc;

void LevelScript::render3D()
{
    if (P(this, 0xdc) != 0) {
        AEGeometry_render(P(this, 0xdc));
    }
    if (P(this, 0xd8) != 0) {
        AEGeometry_render(P(this, 0xd8));
    }
    if (P(this, 0xb8) != 0) {
        AEGeometry_render(P(this, 0xb8));
    }
    if (P(this, 0xbc) != 0) {
        AEGeometry_render(P(this, 0xbc));
    }
    if (P(this, 0xc0) != 0) {
        AEGeometry_render(P(this, 0xc0));
    }
    if (P(this, 0xc4) != 0) {
        AEGeometry_render(P(this, 0xc4));
    }
    if (P(this, 0xc8) != 0 && Explosion_isPlaying(P(this, 0xc8)) != 0) {
        Explosion_render(P(this, 0xc8));
    }
    if (UC(this, 0xa8) != 0 && P(this, 0xac) != 0) {
        RenderProc render = gRenderProc;
        render(P(this, 0xac));
        render(P(this, 0xb0));
        return render(P(this, 0xb4));
    }
}

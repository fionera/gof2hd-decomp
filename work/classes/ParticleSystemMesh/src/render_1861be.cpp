#include "class.h"

extern "C" void _psm_canvasSetTexture(PaintCanvas *canvas, uint32_t texture, uint32_t fallback);
extern "C" void _psm_canvasSetBlendMode(PaintCanvas *canvas, BlendMode mode);
extern "C" uint32_t _psm_cameraGetCurrent(PaintCanvas *canvas);
extern "C" uint32_t _psm_cameraGetLocal(PaintCanvas *canvas, uint32_t current);
extern "C" void _psm_render2(PaintCanvas *canvas, uint32_t texture, uint32_t camera);

struct ParticleSystemMesh {
    static void render(PaintCanvas *canvas, uint32_t mesh, uint32_t texture, BlendMode blend);
};

void ParticleSystemMesh::render(PaintCanvas *canvas, uint32_t mesh, uint32_t texture, BlendMode blend)
{
    if (mesh == 0xffffffff)
        return;
    _psm_canvasSetTexture(canvas, texture, 0xffffffff);
    _psm_canvasSetBlendMode(canvas, blend);
    uint32_t current = _psm_cameraGetCurrent(canvas);
    uint32_t local = _psm_cameraGetLocal(canvas, current);
    return _psm_render2(canvas, mesh, local);
}

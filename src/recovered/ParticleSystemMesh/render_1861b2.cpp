#include "class.h"

extern "C" void _psm_render2(PaintCanvas *canvas, uint32_t texture, uint32_t camera);

struct ParticleSystemMesh {
    static void render(PaintCanvas *canvas, uint32_t texture);
};

void ParticleSystemMesh::render(PaintCanvas *canvas, uint32_t texture)
{
    if (texture != 0xffffffff)
        return _psm_render2(canvas, texture, 0);
}

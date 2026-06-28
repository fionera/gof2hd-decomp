#ifndef GOF2_AMESHMERGER_H
#define GOF2_AMESHMERGER_H
#include "engine/core/Array.h"
#include "../core/AEString.h"

namespace AbyssEngine {
    class Mesh;
    class PaintCanvas;
}

void AMeshMerger_drawMeshes(void *canvas, uint32_t transformId, uint32_t flags);

// lint: void_ptr (externally-linked symbol, Pv mangling baked in)

class AMeshMerger {
public:
    unsigned char _pad_0[8];
    Array<AbyssEngine::Mesh *> meshes;
    unsigned char _pad_10[4];
    AbyssEngine::PaintCanvas *canvas;
    unsigned char _pad_18[4];
    uint32_t transformId;

    ~AMeshMerger();

    void render();
};
#endif

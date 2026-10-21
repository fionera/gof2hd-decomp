#ifndef GOF2_AMESHMERGER_H
#define GOF2_AMESHMERGER_H
#include "engine/core/Array.h"
#include "../core/AEString.h"

namespace AbyssEngine {
    class Mesh;
    class PaintCanvas;
}

class AMeshMerger {
public:
    int rows;
    uint16_t flags;
    uint8_t initialized;
    Array<AbyssEngine::Mesh *> meshes;
    AbyssEngine::PaintCanvas *canvas;
    uint32_t mergedMeshId;
    uint32_t transformId;

    ~AMeshMerger();

    void render();
};
#endif

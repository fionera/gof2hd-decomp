#ifndef GOF2_AMESHMERGER_H
#define GOF2_AMESHMERGER_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

// Galaxy on Fire 2 -- AMeshMerger.
// Owns an embedded Array<Mesh*> of the meshes to be batched and drawn, plus the
// render-target canvas and the transform to apply when drawing them.

// Engine Mesh is referenced only through pointers held by the embedded Array.
namespace AbyssEngine { class Mesh; }

// Engine draw routine that renders the accumulated meshes onto a canvas.
void AMeshMerger_drawMeshes(void* canvas, uint32_t transformId, uint32_t flags);

class AMeshMerger {
public:
    unsigned char _pad_0[8];
    Array<AbyssEngine::Mesh*> meshes;
    unsigned char _pad_10[4];
    void* canvas;
    unsigned char _pad_18[4];
    uint32_t transformId;

    ~AMeshMerger();
    void render();
};
#endif

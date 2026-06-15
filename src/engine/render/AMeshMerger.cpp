#include "gof2/engine/render/AMeshMerger.h"

// Draw the accumulated meshes onto the canvas using the configured transform.
void AMeshMerger::render()
{
    AMeshMerger_drawMeshes(this->canvas, this->transformId, 0);
}

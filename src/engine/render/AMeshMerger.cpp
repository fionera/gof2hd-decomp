#include "engine/render/AMeshMerger.h"

// Destroy the merger, releasing the embedded Array<Mesh*>.
AMeshMerger::~AMeshMerger()
{
}

// Draw the accumulated meshes onto the canvas using the configured transform.
void AMeshMerger::render()
{
    AMeshMerger_drawMeshes(this->canvas, this->transformId, 0);
}

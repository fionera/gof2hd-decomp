#include "gof2/engine/render/AMeshMerger.h"

// AMeshMerger::render()
//   tail-call engine draw: drawMeshes(this->field_0x14, this->field_0x1c, 0).
void AMeshMerger::render()
{
    AMeshMerger_drawMeshes(this->field_0x14, this->field_0x1c, 0);
}

// AMeshMerger::~AMeshMerger()
//   The embedded `meshes` Array<Mesh*> at +0x08 destructs automatically.
AMeshMerger::~AMeshMerger() = default;

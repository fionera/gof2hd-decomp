#include "gof2/engine/render/AMeshMerger.h"

// AMeshMerger::render()
//   tail-call engine draw: drawMeshes(this->field_0x14, this->field_0x1c, 0).
void AMeshMerger::render()
{
    AMeshMerger_drawMeshes(this->field_0x14, this->field_0x1c, 0);
}

// AMeshMerger::~AMeshMerger()
//   Array<AbyssEngine::Mesh*>::~Array(this + 0x08);
AMeshMerger::~AMeshMerger()
{
    AMeshMerger_ArrayMesh_dtor((char *)this + 0x08);
}

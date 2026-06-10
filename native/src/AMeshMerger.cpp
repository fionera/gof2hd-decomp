#include "gof2/AMeshMerger.h"

// ---- render_1811e4.cpp ----
// AMeshMerger::render()
//   tail-call engine draw: drawMeshes(this->field_0x14, this->field_0x1c, 0).
void AMeshMerger::render()
{
    AMeshMerger_drawMeshes(this->field_0x14, this->field_0x1c, 0);
}

// ---- _AMeshMerger_1811f0.cpp ----
// AMeshMerger::~AMeshMerger()
//   Array<AbyssEngine::Mesh*>::~Array(this + 0x08);
AMeshMerger::~AMeshMerger()
{
    AMeshMerger_ArrayMesh_dtor((char *)this + 0x08);
}

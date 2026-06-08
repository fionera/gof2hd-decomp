#include "class.h"

// AMeshMerger::render()
//   tail-call engine draw: drawMeshes(this->field_14, this->field_1c, 0).
struct AMeshMerger { void render(); };
void AMeshMerger::render()
{
    AMeshMerger_drawMeshes(u32(this, 0x14), u32(this, 0x1c), 0);
}

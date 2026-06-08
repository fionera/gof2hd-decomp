#include "class.h"

// render(): tail-call the engine render routine with (field_0xc, field_0x14, 0).
extern "C" void MeshMerger_render_tail(void *a, void *b, int z);

void MeshMerger::render()
{
    return MeshMerger_render_tail(pp(this, 0xc), pp(this, 0x14), 0);
}

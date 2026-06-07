#include "class.h"

// AEGeometry::render() -> if (visible) tail-call PaintCanvas render(canvas, tf, 0)
extern "C" void _ae_geom_render(uint32_t canvas, uint32_t tf, int z);

struct AEGeometry { void render(); };
void AEGeometry::render()
{
    if (u8(this, 0x48) == 0)
        return;
    return _ae_geom_render(u32(this, 0x2c), u32(this, 0xc), 0);
}

#include "class.h"

// AEGeometry::render() -> if (visible) tail-call PaintCanvas render(canvas, tf, 0)
extern "C" void _ae_geom_render(uint32_t canvas, uint32_t tf, int z);

struct AEGeometry { void render(); };
void AEGeometry::render()
{
    if (this->f_48 == 0)
        return;
    return _ae_geom_render(this->f_2c, this->f_c, 0);
}

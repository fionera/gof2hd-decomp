#include "class.h"

void Trail::render()
{
    return Trail_renderTransform(*gTrailCanvasRender, u32(this, 0x14), 0);
}

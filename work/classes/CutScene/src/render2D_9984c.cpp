#include "class.h"

// render2D(): tail-call ext(*this) (the Level pointer at offset 0).
extern "C" void CutScene_render2D_tail(void *level);

void CutScene::render2D()
{
    return CutScene_render2D_tail(pp(this, 0x0));
}

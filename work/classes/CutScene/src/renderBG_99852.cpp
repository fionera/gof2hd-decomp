#include "class.h"

// renderBG(): tail-call ext(field_0, field_0x58).
extern "C" void CutScene_renderBG_tail(void *level, uint32_t bg);

void CutScene::renderBG()
{
    return CutScene_renderBG_tail(pp(this, 0x0), u32(this, 0x58));
}

#include "class.h"

extern "C" void paintcanvas_ext_set_wvm(void *);

void SetWorldViewMatrix(AbyssEngine::PaintCanvas *self, const AbyssEngine::AEMath::Matrix &)
{
    return paintcanvas_ext_set_wvm(*(void **)((char *)self + 0x34));
}

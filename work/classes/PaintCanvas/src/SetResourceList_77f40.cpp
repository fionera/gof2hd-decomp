#include "class.h"

extern "C" void paintcanvas_ext_set_reslist(void *, unsigned int, void *);

void SetResourceList(AbyssEngine::PaintCanvas *self, void **list, unsigned int count)
{
    return paintcanvas_ext_set_reslist(list, count, (char *)self + 0x134);
}

#include "class.h"

extern "C" void paintcanvas_ext_add_resource(void *, void *);

void AddResource(AbyssEngine::PaintCanvas *self, void *resource)
{
    return paintcanvas_ext_add_resource(resource, (char *)self + 0x134);
}

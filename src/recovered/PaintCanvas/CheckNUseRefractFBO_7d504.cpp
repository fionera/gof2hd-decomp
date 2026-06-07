#include "class.h"

extern char paintcanvas_g_refract1;
extern char paintcanvas_g_refract2;
extern "C" int paintcanvas_ext_is_posteffect(void *);
extern "C" void paintcanvas_ext_use_refract(void *);

void CheckNUseRefractFBO(AbyssEngine::PaintCanvas *self, bool)
{
    if (paintcanvas_g_refract1 != 0 && paintcanvas_g_refract2 != 0 &&
        paintcanvas_ext_is_posteffect(*(void **)((char *)self + 0x34)) == 0) {
        return paintcanvas_ext_use_refract(*(void **)((char *)self + 0x34));
    }
}

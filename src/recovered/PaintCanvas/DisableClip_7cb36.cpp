#include "class.h"

extern "C" void paintcanvas_ext_disable(int);

void DisableClip()
{
    return paintcanvas_ext_disable(0xc11);
}

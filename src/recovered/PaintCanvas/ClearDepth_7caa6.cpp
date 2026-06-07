#include "class.h"

extern "C" void paintcanvas_ext_clear(int);

void ClearDepth()
{
    return paintcanvas_ext_clear(0x100);
}

#include "class.h"

extern "C" void paintcanvas_ext_texcreate6(unsigned short, unsigned int &, void *, void *,
                                           unsigned int &, bool);

void TextureCreate(unsigned short w, unsigned int &ref, bool b)
{
    return paintcanvas_ext_texcreate6(w, ref, 0, 0, *(unsigned int *)&b, b);
}

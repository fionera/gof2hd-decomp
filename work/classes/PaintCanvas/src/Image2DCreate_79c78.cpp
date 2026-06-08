#include "class.h"

extern "C" void *paintcanvas_ext_i2d_findres(void *self, unsigned short id);
extern "C" void paintcanvas_ext_i2d_texcreate(void *self, unsigned short id, bool b);
extern "C" int paintcanvas_ext_i2d_imgregion(void *eng, char *path, unsigned short region, void *img);

namespace AbyssEngine {
void ArrayAddImage2D(Image2D *item, void *array);
}

void Image2DCreate(AbyssEngine::PaintCanvas *self, unsigned short param_1, unsigned int *param_2)
{
    char *res = (char *)paintcanvas_ext_i2d_findres(self, param_1);
    if (res == 0) {
        return;
    }
    unsigned short *info = *(unsigned short **)(res + 0xc);
    char *texres = (char *)paintcanvas_ext_i2d_findres(self, *info);
    if (texres == 0) {
        return;
    }
    if (*(int *)(texres + 8) == -1) {
        paintcanvas_ext_i2d_texcreate(self, *info, true);
    }
    unsigned int idx = *(unsigned int *)(res + 8);
    if (idx == 0xffffffff) {
        char *img = (char *)operator new(0x18);
        memset(img, 0, 0x18);
        char *texpath = (char *)**(char ***)(texres + 0xc);
        int ok = paintcanvas_ext_i2d_imgregion(*(void **)((char *)self + 0x34), texpath,
                                               info[1], img);
        if (ok != 1) {
            return;
        }
        if (*(int *)(texres + 8) != -1) {
            *(int *)(img + 4) = *(int *)(texres + 8);
        }
        AbyssEngine::ArrayAddImage2D((AbyssEngine::Image2D *)img, (char *)self + 0x14c);
        idx = *(unsigned int *)((char *)self + 0x14c) - 1;
        *(unsigned int *)(res + 8) = idx;
    }
    *param_2 = idx;
}

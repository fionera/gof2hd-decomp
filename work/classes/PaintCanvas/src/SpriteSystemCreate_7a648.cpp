#include "class.h"

extern "C" int paintcanvas_ext_ss2_sscreate(void *eng, unsigned short id, bool b, void **out);
extern "C" void paintcanvas_ext_ss2_matcreate(void *self, unsigned short id, unsigned int *out);

namespace AbyssEngine {
void ArrayAddSpriteSystem(SpriteSystem *item, void *array);
}

void SpriteSystemCreate(AbyssEngine::PaintCanvas *self, unsigned short param_1, bool param_2,
                        unsigned short param_3, unsigned int *param_4)
{
    void *ss = 0;
    unsigned int result;
    int ok = paintcanvas_ext_ss2_sscreate(*(void **)((char *)self + 0x34), param_1, param_2, &ss);
    if (ok == 1) {
        unsigned int mat = 0xffffffff;
        paintcanvas_ext_ss2_matcreate(self, param_3, &mat);
        if (mat <= *(unsigned int *)((char *)self + 0x174)) {
            void *node = *(void **)((char *)ss + 0x10);
            *(unsigned int *)((char *)node + 0x30) =
                *(unsigned int *)(*(char **)((char *)self + 0x178) + mat * 4);
        }
        unsigned int i;
        for (i = 0; i < *(unsigned int *)((char *)self + 0x180); i++) {
            void **slot = (void **)(*(char **)((char *)self + 0x184) + i * 4);
            if (*(int *)slot == 0) {
                *slot = ss;
                ss = 0;
                *param_4 = i;
                return;
            }
        }
        if (ss == 0) {
            return;
        }
        AbyssEngine::ArrayAddSpriteSystem((AbyssEngine::SpriteSystem *)ss,
                                          (char *)self + 0x180);
        result = *(unsigned int *)((char *)self + 0x180) - 1;
    } else {
        result = 0xffffffff;
    }
    *param_4 = result;
}

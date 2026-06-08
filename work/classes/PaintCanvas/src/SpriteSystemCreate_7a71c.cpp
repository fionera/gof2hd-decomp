#include "class.h"

extern "C" int paintcanvas_ext_sscreate(void *eng, unsigned short id, bool b, void **out);

namespace AbyssEngine {
void ArrayAddSpriteSystem(SpriteSystem *item, void *array);
}

void SpriteSystemCreate(AbyssEngine::PaintCanvas *self, unsigned short param_1,
                        bool param_2, unsigned int *param_3)
{
    void *ss = 0;
    unsigned int result;
    int ok = paintcanvas_ext_sscreate(*(void **)((char *)self + 0x34), param_1, param_2, &ss);
    if (ok == 1) {
        unsigned int i;
        for (i = 0; i < *(unsigned int *)((char *)self + 0x180); i++) {
            void **slot = (void **)(*(char **)((char *)self + 0x184) + i * 4);
            if (*(int *)slot == 0) {
                *slot = ss;
                ss = 0;
                *param_3 = i;
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
    *param_3 = result;
}

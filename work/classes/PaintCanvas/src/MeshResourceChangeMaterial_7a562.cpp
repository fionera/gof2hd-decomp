#include "class.h"

void MeshResourceChangeMaterial(AbyssEngine::PaintCanvas *self, unsigned short matId, unsigned short value)
{
    unsigned int count = *(unsigned int *)((char *)self + 0x134);
    for (unsigned int i = 0; i < count; ++i) {
        char *res = (*(char ***)((char *)self + 0x138))[i];
        if (res) {
            if (*(unsigned short *)res == matId) {
                char *mat = *(char **)(res + 0xc);
                *(unsigned short *)(mat + 0x4) = value;
            }
        }
    }
}

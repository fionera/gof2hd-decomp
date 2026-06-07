#include "class.h"

extern "C" void RestoreImage2D(void *, void *);

void MeshSet2DMask(AbyssEngine::PaintCanvas *self, unsigned int index, int)
{
    unsigned int i = index;
    if (*(unsigned int *)((char *)self + 0x14c) <= index) {
        return;
    }
    char **arr = *(char ***)((char *)self + 0x150);
    char *img = arr[i];
    if (*(unsigned char *)(img + 0x14) != 0) {
        RestoreImage2D(arr, img);
        arr = *(char ***)((char *)self + 0x150);
    }
    *(char **)((char *)self + 0x20) = arr[i];
}

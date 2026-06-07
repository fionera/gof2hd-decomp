#include "class.h"

extern "C" void ArraySetLength_AESoundInterface(int size, void *array);

extern "C" void AESoundRessource_SetSound_7fb74(AbyssEngine::AESoundRessource *self, AbyssEngine::AESoundInfo *info, int count)
{
    *(volatile int *)((char *)self + 0x10) = count;
    AbyssEngine::AESoundRessource *saved = self;
    *(AbyssEngine::AESoundInfo * volatile *)((char *)saved + 0) = info;
    ArraySetLength_AESoundInterface(count, (char *)saved + 4);
    int length = *(int *)((char *)saved + 4);
    int zero = 0;
    for (int i = 0; length != i; ++i) {
        *(int *)(*(char **)((char *)saved + 8) + i * 4) = zero;
    }
}

#include "class.h"

extern "C" void AESoundRessource_getSoundInfo_7fa30(AbyssEngine::AESoundRessource *self, int id, AbyssEngine::AESoundInfo *info, int *index)
{
    *index = -1;
    uint32_t count = *(uint32_t *)((char *)self + 4);
    for (uint32_t i = 0; i < count; ++i) {
        char *base = *(char **)((char *)self + 0);
        AbyssEngine::AESoundInfo *entry = (AbyssEngine::AESoundInfo *)(base + i * 12);
        if (entry->id == id) {
            *index = i;
            *info = *entry;
            return;
        }
    }
}

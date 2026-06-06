#include "class.h"

extern Array<AEPakFileEntry *> *g_AEFile_pakFiles;

__attribute__((minsize)) void AEFile::sortPakFileEntryList()
{
    Array<AEPakFileEntry *> **slot = &g_AEFile_pakFiles;
    int32_t count = (int32_t)(*slot)->size;
    if (count == 0) {
        return;
    }

    while (--count, count >= 0) {
        for (int32_t index = 0; count != index; index++) {
            AEPakFileEntry **entries = (*slot)->data;
            AEPakFileEntry *left = entries[index];
            AEPakFileEntry *right = entries[index + 1];
            if (right->crc < left->crc) {
                entries[index] = right;
                (*slot)->data[index + 1] = left;
            }
        }
    }
}

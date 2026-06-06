#include "class.h"

extern Array<AEPakFileEntry *> *volatile g_AEFile_pakFiles;

void AEFile::sortPakFileEntryList()
{
    int32_t count = g_AEFile_pakFiles->size;

    if (count == 0) {
        return;
    }

    goto decrement;

scan:
    {
        int32_t index = 0;
        goto test;

    loop:
        {
            Array<AEPakFileEntry *> *pakFiles = g_AEFile_pakFiles;
            AEPakFileEntry **entries = pakFiles->data;
            AEPakFileEntry **slot = entries + index;
            AEPakFileEntry *left = entries[index];
            AEPakFileEntry *right = slot[1];

            if (right->crc < left->crc) {
                entries[index] = right;
                g_AEFile_pakFiles->data[index + 1] = left;
            }
        }
        ++index;

    test:
        if (count != index) {
            goto loop;
        }
    }

decrement:
    --count;
    if (count >= 0) {
        goto scan;
    }
}

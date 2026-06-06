#include "class.h"

extern Array<AELowLevelFile *> *g_AEFile_openFiles;
extern Array<AEPakFileEntry *> *volatile g_AEFile_pakFiles;

void AEFile::Release()
{
    if (g_AEFile_openFiles != 0) {
        ArrayReleaseClasses(*g_AEFile_openFiles);
        if (g_AEFile_openFiles != 0) {
            delete g_AEFile_openFiles;
        }
        g_AEFile_openFiles = 0;
    }

    if (g_AEFile_pakFiles != 0) {
        Array<AEPakFileEntry *> *entries = g_AEFile_pakFiles;
        uint32_t offset = 0;
        uint32_t index = 0;
        goto check_entries;
    entry_loop:
        {
            AEPakFileEntry *entry = *(AEPakFileEntry **)((uint8_t *)entries->data + offset);
            if (entry == 0) {
                entries->data[index] = 0;
            } else {
                entry->name.~String();
                operator delete(entry);
                *(AEPakFileEntry **)((uint8_t *)g_AEFile_pakFiles->data + offset) = 0;
            }
            offset += 4;
            entries = g_AEFile_pakFiles;
            index += 1;
        }
    check_entries:
        if (index < entries->size) {
            goto entry_loop;
        }
        if (entries != 0) {
            delete entries;
        }
        g_AEFile_pakFiles = 0;
    }
}

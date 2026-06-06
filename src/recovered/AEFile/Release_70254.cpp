#include "class.h"

extern Array<AELowLevelFile *> *g_AEFile_openFiles;
extern Array<AEPakFileEntry *> *volatile g_AEFile_pakFiles;

__attribute__((minsize)) void AEFile::Release()
{
    if (g_AEFile_openFiles != 0) {
        ArrayReleaseClasses(*g_AEFile_openFiles);
        if (g_AEFile_openFiles != 0) {
            delete g_AEFile_openFiles;
        }
        g_AEFile_openFiles = 0;
    }

    Array<AEPakFileEntry *> *entries = g_AEFile_pakFiles;
    if (entries != 0) {
        uint32_t offset = 0;
        uint32_t index = 0;
        while (index < entries->size) {
            AEPakFileEntry *entry = *(AEPakFileEntry **)((uint8_t *)entries->data + offset);
            AEPakFileEntry **slot;
            if (entry == 0) {
                slot = &entries->data[index];
            } else {
                entry->name.~String();
                operator delete(entry);
                slot = (AEPakFileEntry **)((uint8_t *)g_AEFile_pakFiles->data + offset);
            }
            *slot = 0;
            index += 1;
            offset += 4;
            entries = g_AEFile_pakFiles;
        }
        if (entries != 0) {
            delete entries;
        }
        g_AEFile_pakFiles = 0;
    }
}

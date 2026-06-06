#include "class.h"

extern Array<AEPakFileEntry *> *g_pakFileEntries;
extern FileInterface *g_fileInterface;
extern char g_AELowLevelPakFile_vtable[];

__attribute__((stack_protect))
__attribute__((minsize)) AELowLevelFile *AEFile::findPakFile(const String &path)
{
    AELowLevelFile *result;
    Array<AEPakFileEntry *> **entriesGlobal = &g_pakFileEntries;

    if ((*entriesGlobal)->size != 0) {
        uint32_t wantedCrc = crc32_ccitt(path);
        Array<AEPakFileEntry *> *entries = *entriesGlobal;
        int32_t low = 0;
        int32_t high = entries->size;

search:
        do {
            int32_t distance = high - low;
            int32_t half = distance >> 1;
            int32_t index = low + half;
            uint32_t entryCrc;

            if (high <= low) {
                goto not_found;
            }

            AEPakFileEntry *entry = entries->data[index];
            entryCrc = entry->crc;

            if (entryCrc == wantedCrc) {
                uint16_t *name = entry->name;
                AEPakFileEntry *foundEntry = (*entriesGlobal)->data[index];
                FileInterface *fileInterface = g_fileInterface;
                void *handle;

                if (foundEntry->size == 0xffffffff) {
                    String entryName(name, false);
                    AEPakFileEntry *openEntry = (*entriesGlobal)->data[index];
                    handle = fileInterface->vtable->OpenRead(fileInterface, entryName, openEntry->name.size, 0, 0, 0, openEntry->offset);
                } else {
                    String entryName(name, false);
                    AEPakFileEntry *openEntry = (*entriesGlobal)->data[index];
                    handle = fileInterface->vtable->OpenRead(fileInterface, entryName, openEntry->name.size, 1, openEntry->packedSize, openEntry->size, openEntry->offset);
                }

                AELowLevelPakFile *pakFile = new AELowLevelPakFile;
                AEPakFileEntry *newEntry = (*entriesGlobal)->data[index];
                pakFile->vtable = (AELowLevelFileVTable *)(g_AELowLevelPakFile_vtable + 8);
                pakFile->handle = handle;
                pakFile->packedSize = newEntry->packedSize;
                pakFile->size = newEntry->size;
                pakFile->position = 0;
                result = pakFile;
                goto done;
            }

            if (wantedCrc <= entryCrc) {
                bool done = high == index;
                high = index;
                if (done) {
                    goto done_zero;
                }
                goto search;
            }

            low = index;
            if (half == 0) {
                goto not_found;
            }
        } while (true);
    }

not_found:
    result = 0;
    return result;

done_zero:
    result = 0;
done:
    return result;
}

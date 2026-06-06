#include "class.h"

extern Array<AEPakFileEntry *> *g_AEFile_pakFiles;
extern Array<AELowLevelFile *> *g_AEFile_openFiles;
extern FileInterface *g_AEFile_fileInterface;
extern uint32_t g_AEFile_initialized;

inline void *operator new(uint32_t, void *ptr) noexcept
{
    return ptr;
}

void AEFile::SetInterface(FileInterface *fileInterface)
{
    if (__builtin_expect(fileInterface == 0, 0)) {
        return;
    }
    if (__builtin_expect(fileInterface->enabled == 0, 0)) {
        return;
    }

    uint32_t *initialized = &g_AEFile_initialized;
    if (*initialized != 0) {
        fileInterface->vtable->ResetSaveDirectory(fileInterface);
    }

    Array<AEPakFileEntry *> **pakFilesSlot = &g_AEFile_pakFiles;
    if (*pakFilesSlot == 0) {
        Array<AEPakFileEntry *> *pakFiles =
            static_cast<Array<AEPakFileEntry *> *>(operator new(sizeof(Array<AEPakFileEntry *>)));
        new (pakFiles) Array<AEPakFileEntry *>();
        *pakFilesSlot = pakFiles;
    }

    Array<AELowLevelFile *> **openFilesSlot = &g_AEFile_openFiles;
    if (*openFilesSlot == 0) {
        Array<AELowLevelFile *> *openFiles =
            static_cast<Array<AELowLevelFile *> *>(operator new(sizeof(Array<AELowLevelFile *>)));
        new (openFiles) Array<AELowLevelFile *>();
        *openFilesSlot = openFiles;
    }
    g_AEFile_fileInterface = fileInterface;
}

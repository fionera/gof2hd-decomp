#include "class.h"

extern Array<AEPakFileEntry *> *g_AEFile_pakFiles;
extern Array<AELowLevelFile *> *g_AEFile_openFiles;
extern FileInterface *g_AEFile_fileInterface;
extern uint32_t g_AEFile_initialized;

__attribute__((minsize)) void AEFile::SetInterface(FileInterface *fileInterface)
{
    if (fileInterface == 0) {
        return;
    }
    if (fileInterface->enabled == 0) {
        return;
    }

    if (g_AEFile_initialized != 0) {
        fileInterface->vtable->ResetSaveDirectory(fileInterface);
    }

    if (g_AEFile_pakFiles == 0) {
        g_AEFile_pakFiles = new Array<AEPakFileEntry *>();
    }
    if (g_AEFile_openFiles == 0) {
        g_AEFile_openFiles = new Array<AELowLevelFile *>();
    }
    g_AEFile_fileInterface = fileInterface;
}

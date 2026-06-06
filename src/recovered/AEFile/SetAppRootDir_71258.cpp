#include "class.h"

extern FileInterface *g_AEFile_fileInterface;

__attribute__((minsize)) void AEFile::SetAppRootDir(const char *path) {
    FileInterface *fileInterface = g_AEFile_fileInterface;
    if ((uint32_t)fileInterface != 0) {
        reinterpret_cast<void (*)(FileInterface *, const char *)>(
            fileInterface->vtable->ResetSaveDirectory)(fileInterface, path);
    }
}

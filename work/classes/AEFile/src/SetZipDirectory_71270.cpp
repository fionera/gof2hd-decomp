#include "class.h"

extern FileInterface *g_AEFile_fileInterface;

__attribute__((minsize))
void AEFile::SetZipDirectory(const char *path)
{
    FileInterface *fileInterface = g_AEFile_fileInterface;
    if (fileInterface != 0) {
        reinterpret_cast<void (*)(FileInterface *, const char *)>(fileInterface->vtable->field_38)(
            fileInterface, path);
    }
}

#include "class.h"

extern FileInterface *gAEFileInterface;

__attribute__((minsize))
void AEFile::ResetSaveDirectory()
{
    FileInterface *fileInterface = gAEFileInterface;
    if (fileInterface != 0) {
        typedef void (*ResetSaveDirectoryFn)(FileInterface *);
        ResetSaveDirectoryFn resetSaveDirectory =
            *reinterpret_cast<ResetSaveDirectoryFn *>(
                reinterpret_cast<uint8_t *>(fileInterface->vtable) + 0x54);
        resetSaveDirectory(fileInterface);
    }
}

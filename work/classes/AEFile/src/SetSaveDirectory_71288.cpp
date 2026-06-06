#include "class.h"

extern FileInterface *gAEFileInterface;

__attribute__((minsize)) void AEFile::SetSaveDirectory(String path) {
    void * volatile cookie = __stack_chk_guard;

    FileInterface *fileInterface = gAEFileInterface;
    if (fileInterface != 0) {
        String savePath(path, false);
        fileInterface->vtable->SetSaveDirectory(fileInterface, savePath);
    }

    uint32_t guardDelta = (uint32_t)cookie - (uint32_t)__stack_chk_guard;
    if (guardDelta == 0) {
        return;
    }
    __stack_chk_fail(guardDelta);
}

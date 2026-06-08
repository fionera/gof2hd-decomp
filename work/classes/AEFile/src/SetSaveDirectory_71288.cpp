#include "class.h"

extern FileInterface *gAEFileInterface;

__attribute__((minsize)) void AEFile::SetSaveDirectory(String path) {
    void * volatile cookie = __stack_chk_guard;

    FileInterface *fileInterface = gAEFileInterface;
    if (fileInterface != 0) {
        String savePath(path, false);
        fileInterface->vtable->SetSaveDirectory(fileInterface, savePath);
    }

    uint32_t saved = (uint32_t)(__UINTPTR_TYPE__)cookie;
    uint32_t current = (uint32_t)(__UINTPTR_TYPE__)*(void *volatile *)&__stack_chk_guard;
    uint32_t guardDelta = current - saved;
    if (guardDelta == 0) {
        return;
    }
    __stack_chk_fail(guardDelta);
}

#include "class.h"

extern FileInterface *gAEFileFileInterface;

__attribute__((minsize)) uint32_t AEFile::FileDelete(const String &path)
{
    void * volatile cookie = __stack_chk_guard;

    FileInterface *fileInterface = gAEFileFileInterface;
    uint32_t result = 0;
    if (fileInterface != 0) {
        String localPath(path, false);
        result = fileInterface->vtable->FileDelete(fileInterface, localPath);
    }

    uint32_t saved = (uint32_t)(__UINTPTR_TYPE__)cookie;
    uint32_t current = (uint32_t)(__UINTPTR_TYPE__)*(void *volatile *)&__stack_chk_guard;
    uint32_t guardDelta = current - saved;
    if (guardDelta == 0) {
        return result;
    }
    __stack_chk_fail(guardDelta);
}

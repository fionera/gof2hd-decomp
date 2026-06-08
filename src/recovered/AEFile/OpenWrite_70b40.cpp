#include "class.h"

__attribute__((minsize)) uint32_t AEFile::OpenWrite(const char *path, uint32_t *handle)
{
    void * volatile cookie = __stack_chk_guard;
    uint32_t result;
    {
        String string(path, false);
        result = OpenWrite(string, handle);
    }

    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie;
    if (guardDelta == 0) {
        return result;
    }
    __stack_chk_fail(guardDelta);
}

#include "class.h"

__attribute__((minsize)) uint32_t AEFile::OpenRead(const char *path, uint32_t *handle)
{
    void * volatile cookie = __stack_chk_guard;
    uint32_t result;
    {
        String string(path, false);
        result = OpenRead(string, handle);
    }

    uint32_t guardDelta = (uint32_t)__stack_chk_guard - (uint32_t)cookie;
    if (guardDelta == 0) {
        return result;
    }
    __stack_chk_fail(guardDelta);
}

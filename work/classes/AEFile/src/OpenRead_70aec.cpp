#include "class.h"

uint32_t AEFile::OpenRead(const char *path, uint32_t *handle)
{
    void * volatile guard = __stack_chk_guard;
    String string(path, false);
    uint32_t result = OpenRead(string, handle);
    string.~String();
    if (__stack_chk_guard == guard) {
        return result;
    }
    __stack_chk_fail();
}

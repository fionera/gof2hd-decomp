#include "class.h"

uint32_t AEFile::OpenAppend(const char *path, uint32_t *handle)
{
    void * volatile guard = __stack_chk_guard;
    String string(path, false);
    uint32_t result = OpenAppend(string, handle);
    string.~String();
    if (__stack_chk_guard == guard) {
        return result;
    }
    __stack_chk_fail();
}

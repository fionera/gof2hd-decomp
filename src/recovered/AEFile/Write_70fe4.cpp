#include "class.h"

uint32_t AEFile::Write(const String &value, uint32_t handle, bool wide)
{
    void * volatile cookie = __stack_chk_guard;
    uint32_t result;

    if (wide) {
        const uint16_t *text = value.GetAEWChar();
        uint32_t size = value.size;

        result = Write(4, &size, handle);
        if (result != 0) {
            result = Write(value.size << 1, text, handle);
        }
    } else {
        char *text = value.GetAEChar();
        uint32_t size = value.size;

        result = Write(4, &size, handle);
        if (result != 0) {
            result = Write(value.size, text, handle);
        }
        operator delete(text);
    }

    uint32_t guardDelta = (uint32_t)__stack_chk_guard - (uint32_t)cookie;
    if (guardDelta == 0) {
        return result;
    }
    __stack_chk_fail(guardDelta);
}

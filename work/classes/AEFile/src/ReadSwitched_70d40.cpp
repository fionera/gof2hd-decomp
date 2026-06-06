#include "class.h"

__attribute__((minsize)) uint32_t AEFile::ReadSwitched(String &value, uint32_t handle, bool)
{
    void * volatile cookie = __stack_chk_guard;
    uint16_t length;
    uint32_t result = 0;

    if (ReadSwitched(length, handle) != 0) {
        uint32_t bytes = length;
        char *buffer = static_cast<char *>(operator new(bytes + 1));
        if (Read(bytes, buffer, handle) != 0) {
            buffer[length] = '\0';
            value.Set(buffer);
            result = 1;
        }
        operator delete(buffer);
    }

    uint32_t guardDelta = (uint32_t)__stack_chk_guard - (uint32_t)cookie;
    if (guardDelta == 0) {
        return result;
    }
    __stack_chk_fail(guardDelta);
}

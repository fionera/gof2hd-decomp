#include "class.h"

__attribute__((minsize)) uint32_t AEFile::Read(String &value, uint32_t handle, bool wide)
{
    uint32_t locals[2];
    uint32_t result = 0;

    locals[1] = (uint32_t)__stack_chk_guard;
    result = Read(4, &locals[0], handle);

    if (wide) {
        if (result != 0) {
            uint32_t length = locals[0];
            uint16_t *buffer = new uint16_t[length + 1];
            result = Read(length << 1, buffer, handle);
            if (result != 0) {
                buffer[locals[0]] = 0;
                value.Set(buffer);
                result = 1;
            }
            delete[] buffer;
        } else {
            result = 0;
        }
    } else {
        if (result != 0) {
            uint32_t length = locals[0];
            char *buffer = new char[length + 1];
            result = Read(length, buffer, handle);
            if (result != 0) {
                buffer[locals[0]] = 0;
                value.Set(buffer);
                result = 1;
            }
            delete[] buffer;
        } else {
            result = 0;
        }
    }

    uint32_t stack_difference = (uint32_t)__stack_chk_guard - locals[1];
    if (stack_difference == 0) {
        return result;
    }
    __stack_chk_fail(stack_difference);
}

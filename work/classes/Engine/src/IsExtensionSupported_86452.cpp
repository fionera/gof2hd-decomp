#include "class.h"

extern "C" const char *glGetString(unsigned int name);

extern "C" bool Engine_IsExtensionSupported(Engine *, const char *extension)
{
    const char *extensions = glGetString(0x1f03);

    uint32_t allLength = 0;
    while (extensions[allLength] != 0) {
        allLength += 1;
    }

    uint32_t extLength = 0;
    while (extension[extLength] != 0) {
        extLength += 1;
    }

    uint32_t index = 0;
    while (index < allLength) {
        if (extensions[index] == extension[0]) {
            uint32_t offset = 0;
            while (offset < extLength && index + offset < allLength &&
                   extensions[index + offset] == extension[offset]) {
                offset += 1;
                if (offset == extLength) {
                    return true;
                }
            }
        }
        index += 1;
    }
    return false;
}

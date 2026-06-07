#include "class.h"

extern "C" String *String_assign(String *self, String *other);

extern "C" void ApplicationManager_KeyCodeSetMapping(ApplicationManager *self, void *array)
{
    unsigned int count = *(unsigned int *)array;
    if (count == 0x40) {
        unsigned int index = 0;
        int offset = 0;
        while (index < count) {
            uint32_t *mapping = *(uint32_t **)(*(char **)((char *)array + 4) + index * 4);
            char *dst = *(char **)((char *)self + 0x10) + offset;
            *(uint32_t *)dst = *mapping;
            String_assign((String *)(dst + 4), (String *)(mapping + 1));
            offset += 0x10;
            count = *(unsigned int *)array;
            ++index;
        }
    }
}

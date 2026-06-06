#include "class.h"

extern FileInterface *gAEFileFileInterface;

inline void *operator new(uint32_t, void *ptr) noexcept
{
    return ptr;
}

uint32_t AEFile::FileDelete(const String &path)
{
    struct StackFrame {
        uint32_t stringStorage[3];
        void *guard;
    } frame;

    void **guard = &__stack_chk_guard;
    FileInterface *fileInterface = gAEFileFileInterface;
    frame.guard = *guard;
    uint32_t result = 0;

    if (fileInterface != 0) {
        String *localPath = new (frame.stringStorage) String(path, false);
        result = fileInterface->vtable->FileDelete(fileInterface, *localPath);
        localPath->~String();
    }

    if ((uint32_t)*guard - (uint32_t)frame.guard != 0) {
        __stack_chk_fail((uint32_t)*guard - (uint32_t)frame.guard);
    }
    return result;
}

#include "class.h"

extern FileInterface *gAEFileFileInterface;

inline void *operator new(uint32_t, void *ptr) noexcept
{
    return ptr;
}

uint32_t AEFile::FileExist(const String &path)
{
    void * volatile cookie;
    unsigned char nativePathStorage[sizeof(String)] __attribute__((aligned(4)));
    unsigned char pakPathStorage[sizeof(String)] __attribute__((aligned(4)));
    unsigned char combinedStorage[sizeof(String)] __attribute__((aligned(4)));
    unsigned char prefixStorage[sizeof(String)] __attribute__((aligned(4)));
    unsigned char pathCopyStorage[sizeof(String)] __attribute__((aligned(4)));
    FileInterface *fileInterface = gAEFileFileInterface;
    cookie = __stack_chk_guard;
    uint32_t result;

    if (fileInterface != 0) {
        String *nativePath = reinterpret_cast<String *>(nativePathStorage);
        new (nativePath) String(path, false);
        result = fileInterface->vtable->FileExist(fileInterface, *nativePath);
        nativePath->~String();

        if (result == 0) {
            String *pakPath = reinterpret_cast<String *>(pakPathStorage);
            new (pakPath) String(path, false);
            if (*(*pakPath)[0u] != '/') {
                String *prefix = reinterpret_cast<String *>(prefixStorage);
                String *pathCopy = reinterpret_cast<String *>(pathCopyStorage);
                String *combined = reinterpret_cast<String *>(combinedStorage);
                new (prefix) String("/", false);
                new (pathCopy) String(path, false);
                new (combined) String(*prefix + *pathCopy);
                *pakPath = *combined;
                combined->~String();
                pathCopy->~String();
                prefix->~String();
            }

            AELowLevelFile *pakFile = findPakFile(*pakPath);
            pakPath->~String();
            result = pakFile != 0;
        } else {
            result = 1;
        }
    } else {
        result = 0;
    }

    if (cookie == __stack_chk_guard) {
        return result;
    }
    __stack_chk_fail();
}

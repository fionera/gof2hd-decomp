#include "class.h"

extern "C" int fseek(void *f, long off, int whence);
extern "C" long ftell(void *f);

// FileInterfaceAndroid::GetFileSize() — seek end, tell, seek start; FILE* at +0x8.
extern "C" int FileInterfaceAndroid_GetFileSize(FileInterfaceAndroid *self)
{
    fseek(F<void *>(self, 0x8), 0, 2);
    int size = ftell(F<void *>(self, 0x8));
    fseek(F<void *>(self, 0x8), 0, 0);
    return size;
}

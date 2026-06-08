#include "class.h"

// FileInterfaceAndroid::SetZipDirectory(void*) — cbz r1; str r1,[r0,#0x34]; bx lr
extern "C" void FileInterfaceAndroid_SetZipDirectory(FileInterfaceAndroid *self, void *p)
{
    if (p != 0)
        F<void *>(self, 0x34) = p;
}

#include "class.h"

// FileInterfaceAndroid::SetAppRootDir(void*) — cbz r1; str r1,[r0,#0x30]; bx lr
extern "C" void FileInterfaceAndroid_SetAppRootDir(FileInterfaceAndroid *self, void *p)
{
    if (p != 0)
        self->f_30 = p;
}

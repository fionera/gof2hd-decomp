#include "class.h"

AENormalFile::AENormalFile(FileInterface *file)
{
    void *vtable = g_AENormalFile_vtable;
    P(this, 0) = (char *)vtable + 8;
    fi_held(this) = (FI *)file;
}

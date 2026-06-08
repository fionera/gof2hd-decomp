#include "class.h"

AENormalFile::AENormalFile(FileInterface *file)
{
    void *vtable = g_AENormalFile_vtable;
    this->vptr = (char *)vtable + 8;
    this->held = (FI *)file;
}

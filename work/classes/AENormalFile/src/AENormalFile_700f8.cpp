#include "class.h"

AENormalFile::AENormalFile(FileInterface *file)
{
    void *vtable = g_AENormalFile_vtable;
    this->f_0 = (char *)vtable + 8;
    fi_held(this) = (FI *)file;
}

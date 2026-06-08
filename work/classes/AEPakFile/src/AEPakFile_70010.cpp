#include "class.h"

AEPakFile::AEPakFile(FileInterface *file, int param_2, int param_3)
{
    void *vtable = g_AEPakFile_vtable;
    this->f_0 = (char *)vtable + 8;
    this->f_4 = file;
    I(this, 0x8) = param_2;
    I(this, 0xc) = param_3;
    I(this, 0x10) = 0;
}

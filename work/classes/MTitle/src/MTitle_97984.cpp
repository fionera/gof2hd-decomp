#include "class.h"

__attribute__((visibility("hidden"))) extern void *volatile g_MTitle_vtable;

MTitle::MTitle()
{
    I(this, 0xc) = 100;
    void *vtable = g_MTitle_vtable;
    this->f_0 = (char *)vtable + 8;
}

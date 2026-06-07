#include "class.h"

__attribute__((visibility("hidden"))) extern void **g_Galaxy_gs_globals;

extern "C" void *Galaxy_gs_op_new(unsigned int size);
extern "C" void Galaxy_FileRead_ctor(void *self);
extern "C" int Galaxy_FileRead_loadStation(void *self, int index);
extern "C" void *Galaxy_FileRead_dtor(void *self);
extern "C" void Galaxy_gs_op_delete(void *p);

int Galaxy::getStation(int index)
{
    int result;
    if (index < 0) {
        result = I(*g_Galaxy_gs_globals, 0x78);
    } else {
        void *fr = Galaxy_gs_op_new(1);
        Galaxy_FileRead_ctor(fr);
        result = Galaxy_FileRead_loadStation(fr, index);
        Galaxy_gs_op_delete(Galaxy_FileRead_dtor(fr));
    }
    return result;
}

#include "class.h"

extern "C" void *Galaxy_ctor_op_new_array(unsigned int size);
extern "C" void *Galaxy_ctor_op_new(unsigned int size);
extern "C" void Galaxy_ctor_FileRead_ctor(void *self);
extern "C" void *Galaxy_ctor_FileRead_loadSystemsBinary(void *self);
extern "C" void *Galaxy_ctor_FileRead_dtor(void *self);
extern "C" void Galaxy_ctor_op_delete(void *p);

Galaxy::Galaxy()
{
    uint8_t *stations = (uint8_t *)Galaxy_ctor_op_new_array(0x87);
    this->stations = stations;
    for (int i = 0; i != 0x87; i = i + 1) {
        stations[i] = 0;
    }
    void *fr = Galaxy_ctor_op_new(1);
    Galaxy_ctor_FileRead_ctor(fr);
    this->systems = Galaxy_ctor_FileRead_loadSystemsBinary(fr);
    Galaxy_ctor_op_delete(Galaxy_ctor_FileRead_dtor(fr));
}

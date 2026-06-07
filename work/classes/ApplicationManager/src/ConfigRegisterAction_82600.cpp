#include "class.h"

extern "C" void ArrayAdd_long_long(long long value, void *array);
extern "C" void ext_001ab5f8(long long key, void *array);

extern "C" void ApplicationManager_ConfigRegisterAction(ApplicationManager *self, long long value, long long key)
{
    void *array = (char *)self + 0x88;
    ArrayAdd_long_long(value, array);
    ext_001ab5f8(key, array);
}

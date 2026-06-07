#include "class.h"

struct SystemPathFinder;

extern "C" void *Array_int_dtor(void *arr);
extern "C" void *SystemPathFinder_getSystemPath(SystemPathFinder *self, void *systems, int start, int current);

void StarMap::setStart(int start, int target)
{
    field<int32_t>(this, 0x1dc) = target;
    field<int32_t>(this, 0x1e0) = start;
    void *path = ptr_field(this, 0xa0);
    if (path != 0) {
        operator delete(Array_int_dtor(path));
    }
    ptr_field(this, 0xa0) = 0;
    ptr_field(this, 0xa0) =
        SystemPathFinder_getSystemPath((SystemPathFinder *)ptr_field(this, 0x50), ptr_field(this, 0x54),
                                       start, field<int32_t>(this, 0x104));
}

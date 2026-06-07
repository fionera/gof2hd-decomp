#include "class.h"

// Route::getDockingTime(int) -> docking time at the given index, or 0.
extern "C" int Route_getDockingTimeAt(Route *self, int index)
{
    if (F<void *>(self, 0x10) != 0) {
        Array<int> *times = F<Array<int> *>(self, 0x14);
        if (index < (int)times->length)
            return times->data[index];
    }
    return 0;
}

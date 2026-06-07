#include "class.h"

extern "C" int BluePrint_getTotalAmount(BluePrint *self, int item);
extern "C" int BluePrint_getRemainingAmount(BluePrint *self, int item);

// BluePrint::getCurrentAmount(int) -> already-produced amount = total - remaining.
extern "C" int BluePrint_getCurrentAmount(BluePrint *self, int item)
{
    int total = BluePrint_getTotalAmount(self, item);
    int remaining = BluePrint_getRemainingAmount(self, item);
    return total - remaining;
}

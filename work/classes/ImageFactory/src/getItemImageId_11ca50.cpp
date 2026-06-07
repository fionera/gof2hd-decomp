#include "class.h"

// ImageFactory::getItemImageId(int) -> base id (0x898 below 0xb0, else 0xef0) + param.
struct ImageFactory { int getItemImageId(int param_1); };
int ImageFactory::getItemImageId(int param_1)
{
    int base = 0xef0;
    if (param_1 < 0xb0) base = 0x898;
    return base + param_1;
}

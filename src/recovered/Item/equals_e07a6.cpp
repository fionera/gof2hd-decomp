#define wchar_t gof2_wchar_t
#include "class.h"
#undef wchar_t

__attribute__((minsize)) bool Item::equals(Item *other)
{
    if (other != 0) {
        return index == other->index;
    }
    return false;
}

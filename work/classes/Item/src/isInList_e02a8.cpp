#define wchar_t gof2_wchar_t
#include "class.h"
#undef wchar_t

bool Item::isInList(Item *item, Array *items)
{
    return isInList(item->index, items);
}

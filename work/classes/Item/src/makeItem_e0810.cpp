#define wchar_t gof2_wchar_t
#include "class.h"
#undef wchar_t

Item *Item::makeItem() {
    int savedPrice = price;
    Item *item = clone();
    item->price = savedPrice;
    item->amount = 1;
    return item;
}

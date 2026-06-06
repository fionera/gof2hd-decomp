#define wchar_t gof2_wchar_t
#include "class.h"

Item *Item::makeItem(int amount)
{
    int savedPrice = price;
    Item *item = clone();
    item->price = savedPrice;
    item->amount = amount;
    return item;
}

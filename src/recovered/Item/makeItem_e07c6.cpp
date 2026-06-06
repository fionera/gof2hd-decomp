#include "class.h"

Item *Item::makeItem(int amount, int price)
{
    Item *item = clone();
    item->price = price;
    item->amount = amount;
    return item;
}

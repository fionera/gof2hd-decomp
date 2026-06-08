#define wchar_t gof2_wchar_t
#include "class.h"
#undef wchar_t

void *operator new(__SIZE_TYPE__ size);

Item *Item::clone()
{
    Item *copy = static_cast<Item *>(operator new(sizeof(Item)));
    copy->ingredients = ingredients;
    copy->quantities = quantities;
    copy->attributes = attributes;
    copy->init();
    copy->price = price;
    copy->unsaleable = unsaleable;
    copy->amount = amount;
    copy->stationAmount = stationAmount;
    return copy;
}

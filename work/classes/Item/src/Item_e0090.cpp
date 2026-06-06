#include "class.h"

__attribute__((minsize)) Item::Item(Array *ingredients_, Array *quantities_, Array *attributes_)
    : ingredients(ingredients_), quantities(quantities_), attributes(attributes_)
{
    init();
}

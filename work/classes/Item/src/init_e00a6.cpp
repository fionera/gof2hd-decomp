#include "class.h"

__attribute__((minsize)) void Item::init()
{
    if (attributes == 0) {
        return;
    }

    int *data = static_cast<int *>(attributes->data);

    index = data[1];
    type = data[3];
    sort = data[5];
    tecLevel = data[7];
    occurence = data[13];
    minPrice = data[15];
    maxPrice = data[17];
    minPriceSystem = data[9];
    int maxSystem = data[11];
    amount = 0;
    stationAmount = 0;
    blueprintAmount = 0;
    missingIngredients = 0;
    maxPriceSystem = maxSystem;
    unsaleable = false;
    price = minPrice + (maxPrice - minPrice) / 2;

}

#define wchar_t gof2_wchar_t
#include "class.h"
#undef wchar_t

struct Station {
    int getTecLevel();
};

void Item::adjustPrice(Station *station)
{
    int basePrice = minPrice;
    price = basePrice + static_cast<int>((static_cast<float>(10 - station->getTecLevel()) / 10.0f) *
                                         static_cast<float>(maxPrice - minPrice));
}

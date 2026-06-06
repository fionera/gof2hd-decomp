#define wchar_t gof2_wchar_t
#include "class.h"
#undef wchar_t

float Item::getPriceRate() {
    return static_cast<float>(price - minPrice) /
           static_cast<float>(maxPrice - minPrice);
}

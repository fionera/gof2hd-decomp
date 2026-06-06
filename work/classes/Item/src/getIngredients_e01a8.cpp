#define wchar_t gof2_wchar_t
#include "class.h"
#undef wchar_t

Array *Item::getIngredients() {
    return ingredients;
}

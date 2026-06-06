#include "class.h"

static const bool canBeInstalledMultipleTimesBySort[] = {
    true,  true,  true,  true,  true,  true,  true,  true,
    false, false, false, true,  true,  false, false, false,
    false, false, false, false, true,  false, true,  true,
    true,  true,  false, false, false, false, false, false,
    true,  false, true,  false, true,  false, false, true,
    true,  false, true,
};

__attribute__((minsize)) bool Item::canBeInstalledMultipleTimes()
{
    return canBeInstalledMultipleTimesBySort[sort];
}

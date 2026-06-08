#include "class.h"

extern "C" Array<int> *BluePrint_getQuantityList(BluePrint *self);

// BluePrint::getCompletionRate() -> averaged per-ingredient completion fraction.
// For each ingredient i: produced fraction = (target - remaining) / target,
// then averaged across all ingredients (divide each term by the ingredient count).
extern "C" float BluePrint_getCompletionRate(BluePrint *self)
{
    Array<int> *quantity = BluePrint_getQuantityList(self);
    Array<int> *counters = F<Array<int> *>(self, 0x0);
    float rate = 0.0f;
    for (uint32_t i = 0; i < quantity->length; i++) {
        int target = quantity->data[i];
        int remaining = counters->data[i];
        float frac = (float)(target - remaining) / (float)target;
        rate += frac / (float)counters->length;
    }
    return rate;
}

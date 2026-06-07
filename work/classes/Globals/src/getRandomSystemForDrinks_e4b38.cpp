#include "class.h"

extern "C" int AERandom_nextInt(int rng, int bound);
extern "C" void Globals_getRandomSystemForDrinks_tail(int a, int b);
extern void *const gDrinks_a __attribute__((visibility("hidden")));
extern void *const gDrinks_rng __attribute__((visibility("hidden")));

extern "C" void Globals_getRandomSystemForDrinks()
{
    int a = *(int *)gDrinks_a;
    int r = AERandom_nextInt(*(int *)gDrinks_rng, 0x16);
    return Globals_getRandomSystemForDrinks_tail(a, r);
}

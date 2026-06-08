#include "class.h"

struct Galaxy;
__attribute__((visibility("hidden"))) extern Galaxy **g_ccpGalaxy;   // DAT_000bccb8
__attribute__((visibility("hidden"))) extern int *g_ccpRandom;       // DAT_000bccbc
__attribute__((visibility("hidden"))) extern int *g_ccpPrice;        // DAT_000bccc0 / extra
__attribute__((visibility("hidden"))) extern int **g_ccpPriceMod;    // DAT_000bccd0

extern "C" {
int Galaxy_getSystems(Galaxy *g);                                    // 0x71884
int Ship_getEquipment2(Ship *ship);                                  // 0x7333c
int Ship_getCargo2(void);                                            // 0x73750
int Station_getItems2(void);                                         // 0x71830
int Station_getIndex2(Station *s);                                   // 0x71824
void AERandom_setSeed(long long seed);                               // 0x739c0
int Status_inAlienOrbit2(void);                                      // 0x723d0
int SolarSystem_getIndex2(int sys);                                  // 0x71a7c
int Item_getMinPriceSystem(void *item);                              // 0x73864-area accessor 1
int Item_getMaxPriceSystem(void *item);                              // 0x739cc
int Galaxy_distancePercent(Galaxy *g, int a, int b, int c, int d);   // 0x739d8
int Item_getMinPrice(void *item);                                    // 0x739e4
int Item_getMaxPrice(void *item);                                    // 0x71cbc
int Item_getSinglePrice(void *item);                                 // 0x71944
int AERandom_nextInt2(int rng, int bound);                           // 0x71848
void Item_setPrice2(void *item, int price);                          // 0x73864
void AERandom_reset2(int rng);                                       // 0x718cc
int Item_coordA(int sysCoord);                                       // 0x73a... (blx r9)  coord accessor
int Item_coordB(int sysCoord);                                       // (blx r10) coord accessor
int Item_getIndex2(void *item);                                      // (blx r11)
}

// Status::calcCargoPrices()
//   Reprices every item in the player's cargo, equipment, and current station inventory based
//   on the galactic distance between the item's cheapest/most-expensive systems and the current
//   system, clamped to the item's price band with a small per-system random jitter.
void Status::calcCargoPrices()
{
    Galaxy *gal = *g_ccpGalaxy;
    int systems = Galaxy_getSystems(gal);
    int *rng = g_ccpRandom;
    const float kPriceScale = 1.0f;    // DAT_000bccac
    const float kJitter = 1.0f;        // DAT_000bccb0

    for (unsigned src = 0; src != 3; src = src + 1) {
        unsigned which = src & 0x7fffffff;
        unsigned *list;
        if (which == 1)
            list = (unsigned *)Ship_getEquipment2(this->ship);
        else if (which == 0)
            list = (unsigned *)Ship_getCargo2();
        else
            list = (unsigned *)Station_getItems2();
        if (list == 0)
            continue;

        // seed by the station index so prices are deterministic per visit.
        AERandom_setSeed((long long)Station_getIndex2(this->station));
        bool ringWorld = false;
        if (Status_inAlienOrbit2() == 0)
            ringWorld = (SolarSystem_getIndex2(this->system) == 0x19);

        unsigned n = *list;
        for (unsigned i = 0; i < n; i = i + 1) {
            void *item = *(void **)(list[1] + i * 4);
            if (item == 0)
                continue;

            // distance percent for the item's min-price system pair.
            int aMin = Item_coordA(*(int *)(*(int *)(systems + 4) + Item_getIndex2(item) * 4));
            int aMax = Item_coordB(*(int *)(*(int *)(systems + 4) + Item_getIndex2(item) * 4));
            int bMin = Item_coordA(*(int *)(*(int *)(systems + 4) + Item_getMaxPriceSystem(item) * 4));
            int bMax = Item_coordB(*(int *)(*(int *)(systems + 4) + Item_getMaxPriceSystem(item) * 4));
            int dItem = Galaxy_distancePercent(gal, aMin, aMax, bMin, bMax);

            // distance percent for the current station's system.
            int cMin = Item_coordA(*(int *)(*(int *)(systems + 4) + Item_getIndex2(item) * 4));
            int cMax = Item_coordB(*(int *)(*(int *)(systems + 4) + Item_getIndex2(item) * 4));
            int sSys = Station_getIndex2(this->station);
            int sMin = Item_coordA(*(int *)(*(int *)(systems + 4) + sSys * 4));
            int sMax = Item_coordB(*(int *)(*(int *)(systems + 4) + sSys * 4));
            int dHere = Galaxy_distancePercent(gal, cMin, cMax, sMin, sMax);

            float t = ((kPriceScale / (float)dItem) * (float)dHere) / kPriceScale;
            int minPrice = Item_getMinPrice(item);
            int band = Item_getMaxPrice(item) - Item_getMinPrice(item);
            float clamp = t < 1.0f ? t : 1.0f;

            if (Item_getSinglePrice(item) > 0) {
                int price;
                if (ringWorld) {
                    price = Item_getMaxPrice(item);
                } else {
                    int base = minPrice + (int)(clamp * (float)band);
                    int jitterMax = (int)((float)base * kJitter);
                    if (jitterMax < 2)
                        jitterMax = 1;
                    price = (base - jitterMax) + AERandom_nextInt2(*rng, jitterMax | 1);
                }
                if (*g_ccpPriceMod != 0 && **g_ccpPriceMod != 0) {
                    // apply a global price modifier when present.
                    price = (int)((float)price + (float)price * (float)(**g_ccpPriceMod));
                }
                Item_setPrice2(item, price);
            }
        }
        AERandom_reset2(*rng);
    }
}

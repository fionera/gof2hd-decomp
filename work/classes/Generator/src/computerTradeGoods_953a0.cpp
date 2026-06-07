#include "class.h"

static AbyssEngine::AERandom **volatile g_Generator_tradeRandom;

void Generator::computerTradeGoods(Station *station) {
    if ((Station_getIndex(station) != 0x6c) &&
        (station = (Station *)Station_getItems(station), station != 0)) {
        Array<Item *> *items = (Array<Item *> *)station;
        AbyssEngine::AERandom **random = g_Generator_tradeRandom;
        uint32_t i = 0;
        while (i < items->length) {
            int amount = Item_getAmount(items->data[i]);
            int take = AERandom_nextInt(*random, 3);
            if (take < amount) {
                Item_changeAmount(items->data[i], -take);
            }
            ++i;
        }
    }
}

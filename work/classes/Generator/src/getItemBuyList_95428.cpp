#include "class.h"

typedef Item *(*ItemFactory)(Item *, int, int);

extern "C" __attribute__((visibility("hidden"))) Status **volatile
    g_Generator_itemStatus;
extern "C" __attribute__((visibility("hidden"))) Array<Item *> **volatile
    g_Generator_allItems;
extern "C" __attribute__((visibility("hidden"))) void **volatile
    g_Generator_itemGalaxy;
extern "C" __attribute__((visibility("hidden"))) AbyssEngine::AERandom **volatile
    g_Generator_itemRandom;
extern "C" __attribute__((visibility("hidden"))) ItemFactory volatile
    g_Generator_introFactory;
extern "C" __attribute__((visibility("hidden"))) int *volatile
    g_Generator_jumpDriveBoost;
extern "C" __attribute__((visibility("hidden"))) int *volatile
    g_Generator_weaponBoost;
extern "C" __attribute__((visibility("hidden"))) uint8_t *volatile
    g_Generator_unlockFlags;
extern "C" __attribute__((visibility("hidden"))) int volatile
    g_Generator_kaamoAllowed[10];
extern "C" __attribute__((visibility("hidden"))) int volatile
    g_Generator_blockedItems[9];

Array<Item *> *Generator::getItemBuyList(Station *station) {
    Status *status = *g_Generator_itemStatus;
    int stationIndex = Station_getIndex(station);

    if (stationIndex == 0x4e &&
        Status_getCurrentCampaignMission(status) < 7) {
        Array<Item *> *items = (Array<Item *> *)operator new(0xc);
        Array_item_ptr_ctor(items);
        ArraySetLength_item_ptr(3, items);
        Array<Item *> *all = *g_Generator_allItems;
        ItemFactory makeIntro = g_Generator_introFactory;
        items->data[0] = makeIntro(all->data[0], 1, 0);
        items->data[1] = makeIntro(all->data[0x16], 1, 0);
        items->data[2] = makeIntro(all->data[0x37], 1, 0);
        return items;
    }

    if (Station_getIndex(station) == 0x6c ||
        Status_inSupernovaSystem(status) != 0) {
        return 0;
    }

    uint32_t stationId = Station_getIndex(station);
    Array<Item *> *result = (Array<Item *> *)operator new(0xc);
    Array_item_ptr_ctor(result);

    Array<Item *> *allItems = *g_Generator_allItems;
    void *galaxy = *g_Generator_itemGalaxy;
    Array<SolarSystem *> *systems = Galaxy_getSystems(galaxy);
    int stationTec = Station_getTecLevel(station);
    int minTec = stationTec / 2;
    if (stationTec < 4) {
        minTec = 1;
    }
    if ((stationId | 2) == 0x6b) {
        minTec = 0;
    }

    if (Status_getCurrentCampaignMission(status) == 0x8b &&
        Station_getSystem(station) == 0x19) {
        ArrayAdd_item_ptr(Item_makeItem(allItems->data[0xbe]), result);
    }

    if (stationId == 0x7e) {
        Item *item = Item_makeItem(allItems->data[0xd1]);
        int amount = 1;
        if (Status_getCurrentCampaignMission(status) != 0x75) {
            amount = AERandom_nextInt(*g_Generator_itemRandom, 10) + 1;
        }
        ArrayAdd_item_ptr(Item_setAmount(item, amount), result);
    }

    AbyssEngine::AERandom *random = *g_Generator_itemRandom;
    float campaignFactor =
        (float)(Status_getCurrentCampaignMission(status) + 0x19) / 100.0f;
    if (campaignFactor > 1.5f) {
        campaignFactor = 1.5f;
    }

    for (uint32_t i = 0; i < allItems->length; ++i) {
        Item *item = allItems->data[i];
        int itemIndex = Item_getIndex(item);
        bool systemGood = itemIndex >= 0x84 && itemIndex < 0x9a;
        bool forcedStation = false;

        if (Station_getIndex(station) == Item_getAttribute(item, 0x3d)) {
            if ((Item_getIndex(item) < 0xc4 || Item_getIndex(item) > 0xc4 ||
                 Status_getCurrentCampaignMission(status) > 0x8d) &&
                (Item_getIndex(item) < 0xc6 || Item_getIndex(item) > 0xc8 ||
                 Status_getCurrentCampaignMission(status) > 0x8d)) {
                forcedStation = true;
            }
        }

        if (stationId == 0x6a) {
            bool allowed = false;
            for (uint32_t j = 0; j < 10; ++j) {
                if (g_Generator_kaamoAllowed[j] == Item_getIndex(item) ||
                    systemGood) {
                    allowed = true;
                    break;
                }
            }
            if (!allowed) {
                continue;
            }
        }

        int occurrence = Item_getOccurence(item);
        if (Item_getIndex(item) == 0x7a && *g_Generator_jumpDriveBoost != 0) {
            occurrence = (int)((float)occurrence +
                               (float)(occurrence *
                                       *g_Generator_jumpDriveBoost) *
                                   0.05f);
        }
        if (Item_getType(item) == 1 && *g_Generator_weaponBoost != 0) {
            occurrence =
                (int)((float)occurrence +
                      (float)(occurrence * *g_Generator_weaponBoost) * 0.05f);
        }

        int sort = Item_getSort(item);
        int mission = Status_getCurrentCampaignMission(status);
        if (occurrence == 0 &&
            ((itemIndex > 0xc3 && g_Generator_unlockFlags[0x37] != 0) ||
             (itemIndex < 0xc4 && g_Generator_unlockFlags[0x35] != 0))) {
            occurrence = 0;
            if (itemIndex != 0x55 && Item_getType(item) != 4 &&
                Item_getIngredients(item) == 0 &&
                (itemIndex != 0xb5 || mission > 0x3a) &&
                (((sort != 0x22 && (sort | 2) != 0x23) || mission > 0x8d) &&
                 (sort != 0x24 && (mission > 0x8d || sort != 0x2b))) &&
                ((1 < (uint32_t)((itemIndex & 0xfffffff7) - 0xd1)) &&
                 (mission > 0x5d || itemIndex != 0xcd))) {
                bool blocked = false;
                for (uint32_t j = 0; j < 9; ++j) {
                    if (g_Generator_blockedItems[j] == itemIndex) {
                        blocked = true;
                    }
                }
                if (!blocked &&
                    (sort != 0x1d || Status_inBlackMarketSystem(status) != 0)) {
                    int roll = AERandom_nextInt(random, 30);
                    occurrence =
                        (int)((float)roll +
                              ((float)Item_getTecLevel(item) / -10.0f + 1.0f) *
                                  30.0f);
                }
            }
        }

        int itemTec = Item_getTecLevel(item);
        bool accept = forcedStation;
        if (!accept) {
            if (Item_getIngredients(item) == 0 &&
                (uint32_t)((i & 0x7fffffff) - 0xd9) > 1 && i != 0xa4 &&
                i != 0xaf && occurrence != 0 &&
                itemTec <= Station_getTecLevel(station) &&
                Item_getSinglePrice(item) != 0) {
                if (Item_getAttribute(item, 0x3c) == 1 &&
                    SolarSystem_getRace(systems->data[Station_getSystem(station)]) !=
                        1) {
                    continue;
                }
                if (stationId != 0x6a && systemGood &&
                    Item_getIndex(item) != Station_getSystem(station) + 0x84) {
                    continue;
                }
                accept = true;
            }
        }

        if (!accept) {
            continue;
        }
        if (Status_hardCoreMode(status) &&
            (Item_getSort(item) == 0x17 || Item_getSort(item) == 0x18)) {
            continue;
        }
        if (stationId == 0x6b && Item_getType(item) != 3) {
            continue;
        }
        if (stationId == 0x69 &&
            (!Item_isWeapon(item) && Item_getSort(item) != 0x1c)) {
            continue;
        }
        if (stationId == 0x65 && !Item_isWeapon(item)) {
            continue;
        }
        if (stationId == 0x6a && Item_getType(item) != 4) {
            continue;
        }

        int randomGate = (int)(campaignFactor * (float)occurrence);
        if (!forcedStation) {
            if ((itemTec > stationTec && !systemGood) ||
                AERandom_nextInt(random, 100) >= randomGate) {
                continue;
            }
            if (Item_getIndex(item) != 0x7a && itemTec < minTec &&
                AERandom_nextInt(random, 100) >= 0x3d) {
                continue;
            }
        }

        int minSystem = Item_getMinPriceSystem(item);
        int minX = SolarSystem_getX(systems->data[minSystem]);
        int minY = SolarSystem_getY(systems->data[minSystem]);
        int stationSystem = Station_getSystem(station);
        int stationX = SolarSystem_getX(systems->data[stationSystem]);
        int stationY = SolarSystem_getY(systems->data[stationSystem]);
        int distance =
            Galaxy_invDistancePercent(galaxy, stationX, stationY, minX, minY);
        int amount = AERandom_nextInt(random, 15) + 5;

        if (Item_getType(item) == 4 || Item_getType(item) == 1) {
            if (Item_getIndex(item) != 0x6d && Item_getType(item) == 4) {
                if (distance > 0x32) {
                    float hard = Status_hardCoreMode(status) ? 2.0f : 20.0f;
                    int scaled = (int)(((float)(distance - 0x32) / 100.0f) *
                                       hard);
                    if (scaled < 2) {
                        scaled = 1;
                    }
                    amount *= scaled;
                }
                if (Item_getIndex(item) == 0x6e &&
                    AERandom_nextInt(random, 10) + 10 < amount) {
                    amount = AERandom_nextInt(random, 10) + 10;
                }
            }
        } else {
            amount /= 5;
            if (amount < 1) {
                amount = 1;
            }
        }

        ArrayAdd_item_ptr(Item_setAmount(Item_makeItem(item), amount), result);
    }

    return result;
}

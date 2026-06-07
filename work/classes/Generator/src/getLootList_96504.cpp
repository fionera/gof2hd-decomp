#include "class.h"

extern "C" __attribute__((visibility("hidden"))) Array<Item *> **volatile
    g_Generator_lootItems;
extern "C" __attribute__((visibility("hidden"))) AbyssEngine::AERandom **volatile
    g_Generator_lootRandom;
extern "C" __attribute__((visibility("hidden"))) int volatile
    g_Generator_typeChances[8];
extern "C" __attribute__((visibility("hidden"))) Status **volatile
    g_Generator_status;

Array<int> *Generator::getLootList(int itemIndex, int amount) {
    if (itemIndex >= 0) {
        Array<int> *result = (Array<int> *)operator new(0xc);
        Array_int_ctor(result);
        ArrayAdd_int(itemIndex, result);
        ArrayAdd_int(amount, result);
        return result;
    }

    Array<Item *> **itemsPtr = g_Generator_lootItems;
    Array<Item *> *itemsForOccurence = *itemsPtr;
    Array<Item *> *items = *itemsPtr;
    AbyssEngine::AERandom **randomPtr = g_Generator_lootRandom;
    int itemCount = (int)itemsForOccurence->length;

    int pairCount = AERandom_nextInt(*randomPtr, 3);
    if (AERandom_nextInt(*randomPtr, 3) == 0) {
        return 0;
    }

    Array<int> *result = (Array<int> *)operator new(0xc);
    Array_int_ctor(result);
    uint32_t length = (uint32_t)(pairCount << 1);
    if (pairCount == 0) {
        length = 2;
    }
    ArraySetLength_int(length, result);

    for (uint32_t out = 0; out < result->length; out += 2) {
        bool found = false;
        int type = 0;
        int selected = 0;

        for (uint32_t tries = 0; !found && tries < 100; ++tries) {
            selected = AERandom_nextInt(*randomPtr, itemCount);
            Item *item = items->data[selected];
            type = Item_getType(item);
            if (Item_getIngredients(item) == 0 &&
                AERandom_nextInt(*randomPtr, 100) < g_Generator_typeChances[type]) {
                int roll = AERandom_nextInt(*randomPtr, 100);
                int occurrence =
                    Item_getOccurence(itemsForOccurence->data[selected]);
                if (roll < occurrence && Item_getSinglePrice(item) >= 1) {
                    found = true;
                    if ((uint32_t)(selected - 0xd9) < 2 || selected == 0xa4 ||
                        selected == 0xaf) {
                        found = false;
                    } else if (type != 4 && Item_getTecLevel(item) > 7) {
                        found = false;
                    }
                }
            }
        }

        int count;
        if (found) {
            result->data[out] = selected;
            if (type == 4) {
                count = AERandom_nextInt(*randomPtr, 9);
                type = 4;
            } else {
                count = AERandom_nextInt(*randomPtr, 3);
            }
        } else {
            result->data[out] = AERandom_nextInt(*randomPtr, 10) + 0x9a;
            count = AERandom_nextInt(*randomPtr, 9);
            type = 4;
        }
        result->data[out | 1] = count + 1;
    }

    Status **statusPtr = g_Generator_status;
    Ship *ship = Status_getShip(*statusPtr);
    if (Ship_hasJumpDrive(ship) != 0) {
        ship = Status_getShip(*statusPtr);
        if (Ship_hasCargo(ship, 0x7a, 1) == 0 &&
            AERandom_nextInt(*randomPtr, 100) < 10) {
            result->data[0] = 0x7a;
        }
    }
    return result;
}

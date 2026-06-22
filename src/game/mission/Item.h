#ifndef GOF2_ITEM_H
#define GOF2_ITEM_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

class Station;
class Item;

typedef Array<Item *> ItemArray;

typedef Array<int> IntArray;

// ItemTable: the backing store (Item* entry buffer) reached via *(int*)(*g_ag_itemTbl* + 4),
// i.e. the data_ pointer of the global Array<Item*> item table. Call sites index fixed entries
// off this buffer by raw byte offset (entry N lives at offset N*4). Each slot is an Item*.
// Only the named entries below are referenced via raw offsets; the leading slots are padding so
// the named members land at their exact byte offsets without shifting the buffer layout.
struct ItemTable {
    void *field_0x0; // entry[0x00]
    void *field_0x4; // entry[0x01]
    void *field_0x8; // entry[0x02]
    void *field_0xc; // entry[0x03]
    void *field_0x10; // entry[0x04]
    void *field_0x14; // entry[0x05]
    void *field_0x18; // entry[0x06]
    void *field_0x1c; // entry[0x07]
    void *field_0x20; // entry[0x08]
    void *field_0x24; // entry[0x09]
    void *field_0x28; // entry[0x0a]
    void *field_0x2c; // entry[0x0b]
    void *field_0x30; // entry[0x0c]
    void *field_0x34; // entry[0x0d]
    void *field_0x38; // entry[0x0e]
    void *field_0x3c; // entry[0x0f]
    void *field_0x40; // entry[0x10]
    void *field_0x44; // entry[0x11]
    void *itemTableEntry0x12; // @0x48 entry[0x12] (Item*) — EMP-gun item, getAttribute(0xa)
};

// ItemDatabase: the item-table singleton object reached opaquely via void** globals
// (e.g. g_liw_d_itemDB, g_ag_itemTblA/B). It is in fact an Array<Item*> holder: the
// leading 4-byte word @0x0 is the element count and @0x4 is the backing-store pointer
// (data_) — the Item** table base. Call sites index fixed entries off that base by raw
// byte offset (entry N at base + N*4). Modeled here as a named view so raw-offset reads
// of +0x4 become clean member access without shifting Array<Item*>'s real layout.
struct ItemDatabase {
    unsigned int field_0x0; // @0x0 element count (Array<Item*>::size_)
    Item **itemTable;       // @0x4 backing-store base (Array<Item*>::data_); entry N at itemTable[N]
};

class Item {
public:
    int index;
    int type;
    int sort;
    int tecLevel;
    int minPriceSystem;
    int maxPriceSystem;
    int price;
    int occurence;
    int minPrice;
    int maxPrice;
    IntArray *ingredients;
    IntArray *quantities;
    IntArray *attributes;
    int amount;
    int stationAmount;
    int blueprintAmount;
    int missingIngredients;
    bool unsaleable;

    Item(IntArray *ingredients, IntArray *quantities, IntArray *attributes);

    ~Item();

    void init();

    void setUnsaleable(bool value);

    bool canBeInstalledMultipleTimes();

    int getIndex();

    int getType();

    int getTecLevel();

    int getSort();

    int getSinglePrice();

    int getTotalPrice();

    int getMaxPrice();

    int getMinPrice();

    int getMaxPriceSystem();

    int getMinPriceSystem();

    void setPrice(int value);

    void setMinPrice(int value);

    void setMaxPrice(int value);

    float getPriceRate();

    void setAmount(int value);

    int getOccurence();

    int getAmount();

    void changeAmount(int delta);

    int getMissingIngredients();

    void setMissingIngredients(int value);

    void setStationAmount(int value);

    int getStationAmount();

    void changeStationAmount(int delta);

    void setBlueprintAmount(int value);

    int getBlueprintAmount();

    void changeBlueprintAmount(int delta);

    IntArray *getIngredients();

    IntArray *getQuantities();

    IntArray *getAttributes();

    int getAttribute(int attribute);

    int transaction(bool buy, int priceAdjustment, bool useCredits);

    int transactionBlueprint(bool fabricate, int mode);

    bool equals(Item *other);

    bool isWeapon();

    Item *makeItem();

    Item *makeItem(int amount);

    Item *makeItem(int amount, int price);

    Item *clone();

    bool checkCredits();

    void adjustPrice(Station *station);

    bool checkCargoSpace();

    bool isUnsaleable();

    static bool isInList(int index, int amount, ItemArray *items);

    static bool isInList(int index, ItemArray *items);

    static bool isInList(Item *item, ItemArray *items);

    static void fabricate(Item *item, ItemArray *items, int amount);

    static ItemArray *combineItems(ItemArray *items, ItemArray *stationItems);

    static ItemArray *extractItems(ItemArray *items, bool station);

    static void combineDuplicates(ItemArray *items);

    static ItemArray *mixItems(ItemArray *items, ItemArray *stationItems);
};

#endif

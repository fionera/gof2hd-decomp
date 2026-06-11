#ifndef GOF2_ITEM_H
#define GOF2_ITEM_H
#include "gof2/common.h"
// real struct kept from byte-match recovery (+ supporting decls)
struct Station;
struct Ship;
struct Item;

// Item lists are vectors of Item* (data() reinterpreted as int* for the packed
// attribute/quantity blobs loaded from the .bin tables).
typedef Array<Item*> ItemArray;

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
    ItemArray *ingredients;
    ItemArray *quantities;
    ItemArray *attributes;
    int amount;
    int stationAmount;
    int blueprintAmount;
    int missingIngredients;
    bool unsaleable;
    uint8_t pad45[3];

    Item(ItemArray *ingredients, ItemArray *quantities, ItemArray *attributes);
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
    ItemArray *getIngredients();
    ItemArray *getQuantities();
    ItemArray *getAttributes();
    int getAttribute(int attribute);
    int transaction(bool buy, int priceAdjustment, bool useCredits);
    int transactionBlueprint(bool fabricate);
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

// NB: original 32-bit layout asserts (sizeof==0x48, ingredients@0x28, amount@0x34,
// unsaleable@0x44) held for the ARM32 target; they no longer apply to a native
// 64-bit build where the Array<Item*> pointers are 8 bytes wide.
#endif

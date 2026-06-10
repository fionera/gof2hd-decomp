#ifndef GOF2_ITEM_H
#define GOF2_ITEM_H
#include "gof2/common.h"
// real struct kept from byte-match recovery (+ supporting decls)
struct Station;
struct Ship;

struct Item {
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
    Array *ingredients;
    Array *quantities;
    Array *attributes;
    int amount;
    int stationAmount;
    int blueprintAmount;
    int missingIngredients;
    bool unsaleable;
    uint8_t pad45[3];

    Item(Array *ingredients, Array *quantities, Array *attributes);
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
    Array *getIngredients();
    Array *getQuantities();
    Array *getAttributes();
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

    static bool isInList(int index, int amount, Array *items);
    static bool isInList(int index, Array *items);
    static bool isInList(Item *item, Array *items);
    static void fabricate(Item *item, Array *items, int amount);
    static Array *combineItems(Array *items, Array *stationItems);
    static Array *extractItems(Array *items, bool station);
    static void combineDuplicates(Array *items);
    static Array *mixItems(Array *items, Array *stationItems);
};

static_assert(sizeof(Item) == 0x48, "Item layout size");
static_assert(__builtin_offsetof(Item, ingredients) == 0x28, "Item ingredients offset");
static_assert(__builtin_offsetof(Item, amount) == 0x34, "Item amount offset");
static_assert(__builtin_offsetof(Item, unsaleable) == 0x44, "Item unsaleable offset");
#endif

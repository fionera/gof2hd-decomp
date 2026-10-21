#ifndef GOF2_BLUEPRINT_H
#define GOF2_BLUEPRINT_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "game/core/String.h"

// Galaxy on Fire 2 - BluePrint: an item-production blueprint. Tracks the
// remaining ingredient quantities, accumulated spend and the station the
// blueprint is currently being built at.

class Item;

class BluePrint {
public:
    Array<int>          *ingredientCounters;  // ingredient remaining-quantity counters
    int32_t              spentValue;          // value spent so far
    uint8_t              locked;              // unlock flag
    int32_t              productionCount;     // number of completed productions
    int32_t              stationIndex;        // station index (-1 if unset)
    AbyssEngine::String  stationName;         // station name
    int32_t              itemIndex;           // produced item index
    int32_t              batchMultiplier;     // batch multiplier
    int32_t              remainingBatch;      // remaining batch count

    explicit BluePrint(int item);
    ~BluePrint();

    void addItem(Item *item, int amount, int station);
    void complete();
    int getAutoCompletionPrice();
    float getCompletionRate();
    int getCurrentAmount(int item);
    int getIndex();
    int getStationIndex();
    int getQuantity();
    // Base (unscaled) batch quantity for one production cycle.
    int getBaseQuantity();
    // Accumulated spend on this blueprint.
    int getMoneySpent();
    void setMoneySpent(int value);
    bool isUnlocked();
    int getIngredientsValue();
    int getRemainingAmount(int item);
    AbyssEngine::String getStationName();
    int getTotalAmount(int item);
    bool isCompleted();
    bool isEmpty();
    void lock();
    void reset();
    void unlock();

private:
    Array<int> *getIngredientList();
    Array<int> *getQuantityList();
};
#endif

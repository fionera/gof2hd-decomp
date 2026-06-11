#ifndef GOF2_BLUEPRINT_H
#define GOF2_BLUEPRINT_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 - BluePrint class (item production blueprint).
// Android libgof2hdaa.so, armv7 Thumb. Top-level class (no AbyssEngine namespace),
// per each work-item Sig line. Field offsets recovered per-method from the target
// disassembly; access fields via byte-offset casts from `this`.
//
// Object layout (recovered from ctor / accessors):
//   +0x00  Array<int>* ingredients-remaining (quantity counters)
//   +0x04  int         spent value
//   +0x08  uint8_t     locked flag
//   +0x0c  int         production count
//   +0x10  int         station index (-1 if unset)
//   +0x14  String      station name (12 bytes)
//   +0x20  int         item index
//   +0x24  int         batch multiplier
//   +0x28  int         remaining batch


struct BluePrint;
struct Item;
struct Station;



// AbyssEngine::String - 12-byte trivially-copied value aggregate.


// Field accessor via byte offset.

class BluePrint {
public:
    Array<int>* ingredientCounters;              // +0x0  ingredient remaining-quantity counters
    int32_t spentValue;                  // +0x4  spent value
    uint8_t locked;                  // +0x8  locked flag
    int32_t productionCount;                  // +0xc  production count
    int32_t stationIndex;                 // +0x10 station index (-1 if unset)
    String  stationName;                 // +0x14 station name
    int32_t itemIndex;                 // +0x20 item index
    int32_t batchMultiplier;                 // +0x24 batch multiplier
    int32_t remainingBatch;                 // +0x28 remaining batch

    // ---- methods (converted from free functions) ----
    void addItem(Item *item, int amount, int station);
    void complete();
    int getAutoCompletionPrice();
    float getCompletionRate();
    int getCurrentAmount(int item);
    int getIngredientsValue();
    int getRemainingAmount(int item);
    AbyssEngine::String12 getStationName();
    int getTotalAmount(int item);
    bool isCompleted();
    bool isEmpty();
    void lock();
    void reset();
    void unlock();
};
#endif

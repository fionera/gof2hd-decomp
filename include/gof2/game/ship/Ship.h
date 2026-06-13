#ifndef GOF2_SHIP_H
#define GOF2_SHIP_H
#include "gof2/common.h"
// real struct kept from byte-match recovery (+ supporting decls)
// Shared layout + helper declarations for Galaxy on Fire 2 `Ship` class.
// Target: Android libgof2hdaa.so (Thumb-2, clang -O2). Recovered byte-exact.
//
// Each method .cpp #includes this header. The struct reproduces the field
// offsets observed in the target disassembly (verified across the simple
// getters/setters and the constructor). External engine/game helpers are
// declared `extern "C"` so the differ (which normalizes call targets) lets
// tail-calls become `return helper(...)`.

// ---------------------------------------------------------------------------
// AbyssEngine Array<T> container.
//   layout = { u32 size; T* data; u32 size2(capacity); }  (== AEArray)
// All pointer instantiations compile identically.


// Engine container primitives (defined in src/engine/array.cpp; matched).
template <class T> void  ArrayAdd(T item, Array<T>& a);
template <class T> void  ArraySetLength(unsigned int n, Array<T>& a);

// ---------------------------------------------------------------------------
// Ship layout. size = 0x80. Field offsets reproduced from the target.
// `Item` is opaque here; the recovered code only ever holds Item* and calls
// out to Item:: members via extern "C" thunks.
struct Item;

class Ship {
public:
    // Recovered real members (constructor / destructor + slot/cargo mutators).
    Ship(int index, int baseHP, int baseLoad, int value,
         int slot0, int slot1, int slot2, int slot3, float handling);
    ~Ship();

    void          recomputeAfterSlots();
    void          addCargo(Array<Item*>* items);
    void          addCargo(Item* item);
    int           removeCargo(int index, int amount);
    void          removeCargo(int index);
    void          removeCargo(Item* item);
    void          removeAllCargo();
    void          setEquipment(Item* item);
    void          setEquipment(Item* item, int slot);
    void          setEquipment(Array<Item*>* items);
    void          replaceEquipment(Array<Item*>* equipment);
    int           addEquipment(Item* item);
    void          removeEquipment(Item* item);
    Item*         getFirstEquipmentOfSort(int sort);
    int           getEquipmentValue();
    int           getUsedSlots(int type);
    int           getFreeSlots(int type);
    int           getSlots(int i);
    int           getSlotTypes();
    unsigned int  getSlotPos(Item* item);
    void          freeSlot(Item* item);
    void          freeSlot(Item* item, int slot);
    void          freeAllSlots();
    int           slotAvailable(int sort);
    bool          hasEquipment(int index, int amount);
    bool          hasSecondaryWeapons();

    Array<Item*>* getEquipment();
    Array<Item*>* getEquipment(int type);

    void          setCargo(Array<Item*>* cargo);
    void          replaceCargo(Array<Item*>* cargo);
    Array<Item*>* getCargo();
    Item*         getCargo(int index);
    bool          hasCargo(int index, int amount);
    bool          hasCargoType(int type);
    bool          hasVolatileGoods();
    int           getCargoValue();
    bool          spaceAvailable(int n);
    int           getFreeSpace();
    void          changeLoad(int delta);

    Ship*         clone();
    bool          equals(Ship* other);
    void          refreshValue();
    void          adjustPrice();
    void          priceDecline();

    void          addMod(int mod);
    void          setMods(Array<int>* mods);
    Array<int>*   getMods();
    bool          hasModInstalled(int mod);
    int           getModdedLoad();

    // Simple stat accessors.
    int           getIndex();
    int           getRace();
    void          setRace(int race);
    int           getSignatureRace();
    int           getBaseHP();
    int           getMaxHP();
    int           getCombinedHP();
    int           getMaxShieldHP();
    int           getMaxArmorHP();
    int           getShieldRegen();
    int           getBaseLoad();
    int           getMaxLoad();
    int           getCurrentLoad();
    int           getCargoPlus();
    int           getCompression();
    int           getValue();
    int           getPrice();
    void          setPrice(int price);
    float         getHandling();
    float         getHandlingForShop();
    int           getUnmoddedHandling();
    int           getFireRateFactor();
    int           getDamageFactor();
    int           getFirePower();
    int           getAgility();
    int           getRadarType();
    int           getRepairType();
    int           getMaxPassengers();
    int           getBoostSpeed();
    int           getBoostDelay();
    int           getBoostTime();
    bool          hasBooster();
    int           getCurrentWeaponSlot();
    void          setCurrentWeaponSlot(int slot);
    int           getNumAddedDeviceSlots();
    unsigned char hasEmergencySystem();
    bool          hasCloak();
    bool          hasCloakIntegrated();
    unsigned int  hasJumpDrive();
    unsigned int  hasJumpDriveIntegrated();
    Ship*         makeShip(int price);

    // Recovered decompiler-fragment members. Each is an alternate entry the
    // (not-yet-rewired) extern "C" shims dispatch to; their bodies match the
    // canonical members above but are kept as distinct named methods so the
    // mechanical wiring pass can attach the corresponding `Ship_*` call.
    Ship*         ctor_full(int index, int baseHP, int baseLoad, int value,
                            int slot0, int slot1, int slot2, int slot3, float handling);
    void          addCargo2(Array<Item*>* items);
    void          setEquipment1(Item* item);
    int           removeCargo3(int index, int amount);
    Array<Item*>* getEquipmentByType(unsigned int type);
    unsigned char hasCloakNeg();

    int          index;            // 0x00  ship id / type
    int          baseHP;           // 0x04
    int          value;            // 0x08
    int          baseLoad;         // 0x0c
    int          currentLoad;      // 0x10
    int          price;            // 0x14
    float        handling;         // 0x18
    int          maxShieldHP;      // 0x1c
    int          maxArmorHP;       // 0x20
    int          shieldRegen;      // 0x24
    int          cargoPlus;        // 0x28
    int          compression;      // 0x2c
    int          radarType;        // 0x30
    int          boostSpeed;       // 0x34
    int          boostDelay;       // 0x38
    int          boostTime;        // 0x3c
    int          agility;          // 0x40
    int          maxPassengers;    // 0x44
    int          firePower;        // 0x48
    int          repairType;       // 0x4c
    unsigned char hasJumpDriveFlag;// 0x50
    unsigned char hasCloakFlag;    // 0x51
    unsigned char pad52[2];        // 0x52
    float        fireRateFactor;   // 0x54
    float        damageFactor;     // 0x58
    unsigned char hasEmergency;    // 0x5c
    unsigned char pad5d[3];        // 0x5d
    int          signatureRace;    // 0x60
    int          race;             // 0x64
    int*         slots;            // 0x68  int[4] slot-count array
    Array<Item*>* equipment;       // 0x6c
    Array<Item*>* cargo;           // 0x70
    int          currentWeaponSlot;// 0x74
    Array<int>*  mods;             // 0x78
    int          numAddedDeviceSlots; // 0x7c
};
#endif

#ifndef GOF2_SHIP_H
#define GOF2_SHIP_H
#include "gof2/common.h"
// real struct kept from byte-match recovery
struct Ship {
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

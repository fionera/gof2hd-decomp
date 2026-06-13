#ifndef GOF2_ACHIEVEMENTS_H
#define GOF2_ACHIEVEMENTS_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 — Achievements class. Top-level, no namespace.
// Field offsets recovered from per-method target disassembly. We do NOT model a
// full layout: methods access fields via byte-offset casts from `this`.
//
// Known layout (from ctor/init/decompiles):
//   +0x00  int*  medals      (operator new[](0xb4) -> 45 ints)
//   +0x04  int*  newMedals   (operator new[](0xb4) -> 45 ints)
//   +0x08  int   kills
//   +0x0c  int   catches
//   +0x10  int   pirateKills
//   +0x14  int   ?
//   +0x18  bool  ?
//   +0x1c  int   credits
//   +0x20  u16   (gotAllMedals/gotAllGoldMedals bytes 0x20/0x21)
//   +0x22  bool  gotAllSupernovaMedals

struct Achievements;

// Field accessor via byte offset.

class Achievements {
public:
    int* medals;                     // +0x0
    int* newMedals;                     // +0x4
    int kills;                      // +0x8
    int catches;                      // +0xc
    int pirateKills;                     // +0x10
    int field_0x14;                     // +0x14
    uint8_t hasTurretAndWeapon;                 // +0x18
    int credits;                     // +0x1c
    uint8_t field_0x20;                 // +0x20
    uint8_t field_0x21;                 // +0x21
    uint8_t field_0x22;                 // +0x22
    int medalCount;                     // +0x24

    Achievements();
    ~Achievements();

    // ---- methods (converted from free functions) ----
    void applyNewMedals();
    void checkForNewMedal(PlayerEgo *ego);
    void countMedals();
    int getValue(int index, int sub);
    uint8_t gotAllGoldMedals();
    uint8_t gotAllMedals();
    uint8_t gotAllSupernovaMedals();
    int *getMedals();
    int *getNewMedals();
    uint8_t hasMedal(int index, int value);
    void incCatches();
    void incKills();
    void incPirateKills();
    int init();
    void initCheckEquipmentAndWeapons();
    uint8_t isEliteMedal(int index);
    void resetNewMedals();
    void resetPirateKills();
    void setMedal(int index, int value);
    void setMedals(int *src, int count);
    void updateCredits(int value);
};
#endif

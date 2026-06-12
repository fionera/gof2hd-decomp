#ifndef GOF2_STANDING_H
#define GOF2_STANDING_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 — Standing class (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class (no AbyssEngine:: namespace per the work-item Sig lines).
// Layout recovered from the constructor + per-method disassembly:
//   +0x00  int *standings;   // heap array (operator new[](8) -> 2 ints), [0]=0x1e initially
//   +0x04  int  currentRace; // -1 == "use raw standings[]" mode; >=0 == derived mode
// Most methods reach fields via byte-offset casts from `this`/self (which arrives
// in r0 exactly like the first explicit arg of an extern "C" fn). The ctor/dtor are
// written as real member functions so their demangled symbols carry the method name.




// Field accessor via byte offset.

class Standing {
public:
    int* standings;                     // +0x0
    int currentRace;                      // +0x4
    Standing();
    ~Standing();

    // ---- methods (converted from free functions) ----
    void applyDelict(unsigned kind, int severity);
    void applyDisable(int race);
    void applyKill(int kind);
    void applyMissionCompleted(int race);
    void applyPoints(int race, int delta);
    void applyStealCargo(int race);
    uint32_t getEnemyRace(unsigned idx);
    float getMissionBonus(unsigned race);
    int getStanding(int race);
    float getStandingRate(int race);
    bool isEnemy(int race);
    bool isEnemyWithAnyone();
    bool isFriend(int race);
    unsigned isNeutral(int race);
    void rehabilitate(int race);
    void setPlayerSignatureRace(int race);
    void setStanding(int race, int value);
};
#endif

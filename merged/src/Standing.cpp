#include "Standing.h"
#include <arm_neon.h>

extern "C" int Standing_getStanding(Standing *self, int race);
extern "C" bool Standing_isEnemy(Standing *self, int race);
extern "C" unsigned Standing_isFriend(Standing *self, int race);
extern "C" void Standing_applyPoints(Standing *self, int race, int delta);
extern "C" int Status_hardCoreMode(Status *self);
extern "C" int Status_inAlienOrbit(Status *self);
extern "C" SolarSystem *Status_getSystem(Status *self);
extern "C" int SolarSystem_getRace();

// ---- Standing_11d6a8.cpp ----
// Standing::Standing(): allocate a 2-int standings array {0x1e, 0}; currentRace = -1.
Standing::Standing() {
    int *p = new int[2];
    p[0] = 0x1e;
    p[1] = 0;
    F<int *>(this, 0) = p;
    F<int>(this, 4) = -1;
}

// ---- applyPoints_11d8a8.cpp ----
// Standing::applyPoints(int race, int delta): add delta to standings[race] and
// clamp the result into [-100, 100]. The clamping store only runs when the value
// leaves the band; otherwise it returns early.
extern "C" void Standing_applyPoints(Standing *self, int race, int delta) {
    int *p = F<int *>(self, 0);
    int v = delta + p[race];
    p[race] = v;
    int c;
    if (v > 0x64) {
        c = 100;
    } else {
        if (v >= -0x64) return;
        c = -100;
    }
    p[race] = c;
}

// ---- getStandingRate_11d728.cpp ----
// Standing::getStandingRate(int race): getStanding(race) normalized to [-1, 1].
extern "C" float Standing_getStandingRate(Standing *self, int race) {
    return (float)Standing_getStanding(self, race) / 100.0f;
}

// ---- isEnemyWithAnyone_11d780.cpp ----
// Standing::isEnemyWithAnyone(): true if either home-race standing is outside the
// non-hostile band, i.e. (standing + 0x46) does not fit in [0, 0x8c] as unsigned.
// Both comparisons are always evaluated (bitwise |) to mirror the branchless target.
extern "C" bool Standing_isEnemyWithAnyone(Standing *self) {
    int *p = F<int *>(self, 0);
    int a = p[0];
    int b = p[1];
    return (bool)(((unsigned)(b + 0x46) > 0x8c) | ((unsigned)(a + 0x46) > 0x8c));
}

// ---- getEnemyRace_11d890.cpp ----
// Lookup table of "enemy race" per faction index (4 entries), in .data.rel.ro.
__attribute__((visibility("hidden"))) extern const uint32_t Standing_enemyRaceTable[4];

// Standing::getEnemyRace(int idx): idx<4 ? table[idx] : 8.
extern "C" uint32_t Standing_getEnemyRace(Standing *self, unsigned idx) {
    (void)self;
    if (idx < 4)
        return Standing_enemyRaceTable[idx];
    return 8;
}

// ---- isNeutral_11d86e.cpp ----
// Standing::isNeutral(int race): neither enemy nor friend.
extern "C" unsigned Standing_isNeutral(Standing *self, int race) {
    if (Standing_isEnemy(self, race)) return 0;
    return Standing_isFriend(self, race) ^ 1;
}

// ---- _Standing_11d6c8.cpp ----
// Standing::~Standing(): delete[] the standings array, null the pointer.
Standing::~Standing() {
    int *p = F<int *>(this, 0);
    if (p) operator delete[](p);
    F<int *>(this, 0) = 0;
}

// ---- getStanding_11d6ee.cpp ----
// Standing::getStanding(int race) [static-style: this in r0, race in r1].
// In "derived" mode (currentRace >= 0) factions 0/1 map to fixed standings based
// on currentRace; otherwise the raw standings[race] value is returned.
extern "C" int Standing_getStanding(Standing *self, int race) {
    int cr = F<int>(self, 4);
    if (cr >= 0) {
        if (race == 0) {
            int v = 0x46;
            if (cr == 0) v = 100;
            if (cr == 1) v = -100;
            return v;
        }
        if (race == 1) {
            int v = 0x46;
            if (cr == 2) v = 100;
            if (cr == 3) v = -100;
            return v;
        }
    }
    return F<int *>(self, 0)[race];
}

// ---- applyMissionCompleted_11d992.cpp ----
// Standing::applyMissionCompleted(int race): +5 reputation (delta -5 fed to applyPoints).
extern "C" void Standing_applyMissionCompleted(Standing *self, int race) {
    Standing_applyPoints(self, race, -5);
}

// ---- setStanding_11d6de.cpp ----
// Standing::setStanding(int race, int value): standings[race] = value.
extern "C" void Standing_setStanding(Standing *self, int race, int value) {
    F<int *>(self, 0)[race] = value;
}

// ---- applyStealCargo_11d98c.cpp ----
// Standing::applyStealCargo(int race): -2 reputation toward that race.
extern "C" void Standing_applyStealCargo(Standing *self, int race) {
    Standing_applyPoints(self, race, 2);
}

// ---- isEnemy_11d7a2.cpp ----
// Standing::isEnemy(int race) [static-style: this in r0, race in r1].
// NOTE: best-effort. Logic matches the target exactly, but under -Oz clang keeps
// `this` in r0 / currentRace in r2, whereas the original swaps them (this->r2,
// cr->r0) so the "== 0" test lowers to clz/lsr in r0 — a register-allocation
// choice not reachable from portable C. Left as the faithful decompiled form.
extern "C" bool Standing_isEnemy(Standing *self, int race) {
    int iVar1 = F<int>(self, 4);
    if (-1 < iVar1) {
        if (race != 1) {
            if (race == 3) {
                iVar1 = iVar1 + -2;
            } else if (race == 2) {
                iVar1 = iVar1 + -3;
            } else {
                if (race != 0) return false;
                iVar1 = iVar1 + -1;
            }
        }
        return iVar1 == 0;
    }
    if (race == 1) {
        iVar1 = **(int **)self;
    } else {
        if (race != 3) {
            if (race == 2) {
                iVar1 = *(int *)(*(int *)self + 4);
            } else {
                if (race != 0) return false;
                iVar1 = **(int **)self;
            }
            return iVar1 < -0x46;
        }
        iVar1 = *(int *)(*(int *)self + 4);
    }
    return 0x46 < iVar1;
}

// ---- applyDelict_11d93c.cpp ----
struct Status;
// Singleton holder: **g_adl_status -> the live Status object.
__attribute__((visibility("hidden"))) extern Status **g_adl_status;

// Standing::applyDelict(int kind, int severity): a committed crime against a race.
// Severity doubles in hardcore mode; kinds 0/2 are "against the even slot", 1/3 the
// odd slot, with 0/2 applied as a penalty (negated) — all routed through applyPoints.
extern "C" void Standing_applyDelict(Standing *self, unsigned kind, int severity) {
    int hc = Status_hardCoreMode(*g_adl_status);
    int delta = severity << hc;
    int sign;
    switch (kind) {
    case 0:
        Standing_applyPoints(self, 0, -delta);
        return;
    case 1:
        Standing_applyPoints(self, 0, delta);
        return;
    case 2:
        Standing_applyPoints(self, 1, -delta);
        return;
    case 3:
        Standing_applyPoints(self, 1, delta);
        return;
    default:
        return;
    }
    (void)sign;
}

// ---- applyKill_11d8d0.cpp ----
struct Status;
struct SolarSystem;
// Status singleton holder (PC-rel double-indirect): *g_apk_status -> Status*.
__attribute__((visibility("hidden"))) extern Status **g_apk_status;
// "Enemy race" lookup keyed by the current system's race (4 entries).
__attribute__((visibility("hidden"))) extern const int g_apk_raceTable[4];


// Standing::applyKill(int kind): a kill committed against another ship. For police
// kills (kind 8) the offence is mapped to the local system's owning race (once),
// otherwise it costs 5 standing toward the victim's faction.
extern "C" void Standing_applyKill(Standing *self, int kind) {
    Status **holder = g_apk_status;
    unsigned sysRace;
    if (Status_inAlienOrbit(*holder) != 0) {
        sysRace = 9;
    } else {
        Status_getSystem(*holder);
        sysRace = SolarSystem_getRace();
    }
    int delta;
    if (kind == 8) {
        if (F<int>(self, 4) >= 0) return;
        delta = 1;
        if (sysRace < 4) {
            kind = g_apk_raceTable[sysRace];
        } else {
            kind = 8;
        }
    } else {
        delta = 5;
    }
    Standing_applyPoints(self, kind, delta);
}

// ---- isFriend_11d808.cpp ----
// Standing::isFriend(int race).
extern "C" bool Standing_isFriend(Standing *self, int race) {
    int iVar1 = F<int>(self, 4);
    if (-1 < iVar1) {
        if (race == 1) {
            iVar1 = iVar1 + -1;
        } else if (race == 3) {
            iVar1 = iVar1 + -3;
        } else if (race == 2) {
            iVar1 = iVar1 + -2;
        } else if (race != 0) {
            return false;
        }
        return iVar1 == 0;
    }
    if (race == 1) {
        iVar1 = **(int **)self;
    } else {
        if (race != 3) {
            if (race == 2) {
                iVar1 = *(int *)(*(int *)self + 4);
            } else {
                if (race != 0) return false;
                iVar1 = **(int **)self;
            }
            return 0x46 < iVar1;
        }
        iVar1 = *(int *)(*(int *)self + 4);
    }
    return iVar1 < -0x46;
}

// ---- getMissionBonus_11d9a0.cpp ----
// Standing::getMissionBonus(int race): standing toward the faction, normalized to
// [0, 1] (negative standings clamp to 0). Out-of-range race returns 0.
// NOTE: best-effort. The target loads positive factions' counts straight from memory
// into the FP register (vldr.32 + vcvt) while negating others through a GPR; clang
// under -Oz routes every case through a GPR before a shared vcvt, and encodes the 0.0
// floor as an immediate rather than a literal-pool load. Both are codegen choices not
// reachable from portable C / NEON intrinsics here.
extern "C" float Standing_getMissionBonus(Standing *self, unsigned race) {
    float s0;
    switch (race) {
    case 0:
        s0 = (float)F<int *>(self, 0)[0];
        break;
    case 1:
        s0 = (float)(-F<int *>(self, 0)[0]);
        break;
    case 2:
        s0 = (float)F<int *>(self, 0)[1];
        break;
    case 3:
        s0 = (float)(-F<int *>(self, 0)[1]);
        break;
    default:
        return 0.0f;
    }
    float r = s0 / 100.0f;
    return vget_lane_f32(vmax_f32(vdup_n_f32(r), vdup_n_f32(0.0f)), 0);
}

// ---- applyDisable_11d99a.cpp ----
// Standing::applyDisable(int race).
extern "C" void Standing_applyDisable(Standing *self, int race) {
    Standing_applyPoints(self, race, 2);
}

// ---- rehabilitate_11d750.cpp ----
// Standing::rehabilitate(int race): reset the standing toward a faction's home
// race to a fixed value (-35 for the "even" slot, +35 for the "odd" slot).
extern "C" void Standing_rehabilitate(Standing *self, int race) {
    if (race == 0) {
        F<int *>(self, 0)[0] = -35;
    } else if (race == 1) {
        F<int *>(self, 0)[0] = 0x23;
    } else if (race == 2) {
        F<int *>(self, 0)[1] = -35;
    } else if (race == 3) {
        F<int *>(self, 0)[1] = 0x23;
    }
}

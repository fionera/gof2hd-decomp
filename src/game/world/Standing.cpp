#include "gof2/game/world/Standing.h"
#include "gof2/game/mission/Status.h"
#include "gof2/game/world/SolarSystem.h"
#include <arm_neon.h>

// Standing::Standing(): allocate a 2-int standings array {0x1e, 0}; currentRace = -1.
Standing::Standing() {
    int *p = new int[2];
    p[0] = 0x1e;
    p[1] = 0;
    this->standings = p;
    this->currentRace = -1;
}

// Standing::applyPoints(int race, int delta): add delta to standings[race] and
// clamp the result into [-100, 100]. The clamping store only runs when the value
// leaves the band; otherwise it returns early.
void Standing::applyPoints(int race, int delta) {
    Standing *self = this;
    int *p = self->standings;
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

// Standing::getStandingRate(int race): getStanding(race) normalized to [-1, 1].
float Standing::getStandingRate(int race) {
    Standing *self = this;
    return (float)((Standing *)(self))->getStanding(race) / 100.0f;
}

// Standing::isEnemyWithAnyone(): true if either home-race standing is outside the
// non-hostile band, i.e. (standing + 0x46) does not fit in [0, 0x8c] as unsigned.
// Both comparisons are always evaluated (bitwise |) to mirror the branchless target.
bool Standing::isEnemyWithAnyone() {
    Standing *self = this;
    int *p = self->standings;
    int a = p[0];
    int b = p[1];
    return (bool)(((unsigned)(b + 0x46) > 0x8c) | ((unsigned)(a + 0x46) > 0x8c));
}

// Lookup table of "enemy race" per faction index (4 entries), in .data.rel.ro.
__attribute__((visibility("hidden"))) extern const uint32_t Standing_enemyRaceTable[4];

// Standing::getEnemyRace(int idx): idx<4 ? table[idx] : 8.
uint32_t Standing::getEnemyRace(unsigned idx) {
    Standing *self = this;
    (void)self;
    if (idx < 4)
        return Standing_enemyRaceTable[idx];
    return 8;
}

// Standing::isNeutral(int race): neither enemy nor friend.
unsigned Standing::isNeutral(int race) {
    Standing *self = this;
    if (((Standing *)(self))->isEnemy(race)) return 0;
    return ((Standing *)(self))->isFriend(race) ^ 1;
}

// Standing::~Standing(): delete[] the standings array, null the pointer.
Standing::~Standing() {
    int *p = this->standings;
    if (p) operator delete[](p);
    this->standings = 0;
}

// Standing::getStanding(int race) [static-style: this in r0, race in r1].
// In "derived" mode (currentRace >= 0) factions 0/1 map to fixed standings based
// on currentRace; otherwise the raw standings[race] value is returned.
int Standing::getStanding(int race) {
    Standing *self = this;
    int cr = self->currentRace;
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
    return self->standings[race];
}

// Standing::applyMissionCompleted(int race): +5 reputation (delta -5 fed to applyPoints).
void Standing::applyMissionCompleted(int race) {
    Standing *self = this;
    ((Standing *)(self))->applyPoints(race, -5);
}

// Standing::setStanding(int race, int value): standings[race] = value.
void Standing::setStanding(int race, int value) {
    Standing *self = this;
    self->standings[race] = value;
}

// Standing::setPlayerSignatureRace(int race): records the player's signature race in
// currentRace, switching the table into "derived" mode. With a non-negative race set,
// getStanding() returns fixed standings for factions 0/1 keyed off this value instead
// of reading the raw standings[] array (see getStanding above).
void Standing::setPlayerSignatureRace(int race) {
    this->currentRace = race;
}

// Standing::applyStealCargo(int race): -2 reputation toward that race.
void Standing::applyStealCargo(int race) {
    Standing *self = this;
    ((Standing *)(self))->applyPoints(race, 2);
}

// Standing::isEnemy(int race) [static-style: this in r0, race in r1].
// NOTE: best-effort. Logic matches the target exactly, but under -Oz clang keeps
// `this` in r0 / currentRace in r2, whereas the original swaps them (this->r2,
// cr->r0) so the "== 0" test lowers to clz/lsr in r0 — a register-allocation
// choice not reachable from portable C. Left as the faithful decompiled form.
bool Standing::isEnemy(int race) {
    Standing *self = this;
    int iVar1 = self->currentRace;
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

struct Status;
// Singleton holder: **g_adl_status -> the live Status object.
__attribute__((visibility("hidden"))) extern Status **g_adl_status;

// Standing::applyDelict(int kind, int severity): a committed crime against a race.
// Severity doubles in hardcore mode; kinds 0/2 are "against the even slot", 1/3 the
// odd slot, with 0/2 applied as a penalty (negated) — all routed through applyPoints.
void Standing::applyDelict(unsigned kind, int severity) {
    Standing *self = this;
    int hc = ((Status *)(*g_adl_status))->hardCoreMode();
    int delta = severity << hc;
    int sign;
    switch (kind) {
    case 0:
        ((Standing *)(self))->applyPoints(0, -delta);
        return;
    case 1:
        ((Standing *)(self))->applyPoints(0, delta);
        return;
    case 2:
        ((Standing *)(self))->applyPoints(1, -delta);
        return;
    case 3:
        ((Standing *)(self))->applyPoints(1, delta);
        return;
    default:
        return;
    }
    (void)sign;
}

struct Status;
struct SolarSystem;
// Status singleton holder (PC-rel double-indirect): *g_apk_status -> Status*.
__attribute__((visibility("hidden"))) extern Status **g_apk_status;
// "Enemy race" lookup keyed by the current system's race (4 entries).
__attribute__((visibility("hidden"))) extern const int g_apk_raceTable[4];

// Standing::applyKill(int kind): a kill committed against another ship. For police
// kills (kind 8) the offence is mapped to the local system's owning race (once),
// otherwise it costs 5 standing toward the victim's faction.
void Standing::applyKill(int kind) {
    Standing *self = this;
    Status **holder = g_apk_status;
    unsigned sysRace;
    if (((Status *)(*holder))->inAlienOrbit() != 0) {
        sysRace = 9;
    } else {
        // In the original ABI Status::getSystem() leaves the current SolarSystem* in r0,
        // which SolarSystem::getRace() then reads. Express that as a chained method call.
        sysRace = ((SolarSystem *)(long)((Status *)(*holder))->getSystem())->getRace();
    }
    int delta;
    if (kind == 8) {
        if (self->currentRace >= 0) return;
        delta = 1;
        if (sysRace < 4) {
            kind = g_apk_raceTable[sysRace];
        } else {
            kind = 8;
        }
    } else {
        delta = 5;
    }
    ((Standing *)(self))->applyPoints(kind, delta);
}

// Standing::isFriend(int race).
bool Standing::isFriend(int race) {
    Standing *self = this;
    int iVar1 = self->currentRace;
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

// Standing::getMissionBonus(int race): standing toward the faction, normalized to
// [0, 1] (negative standings clamp to 0). Out-of-range race returns 0.
// NOTE: best-effort. The target loads positive factions' counts straight from memory
// into the FP register (vldr.32 + vcvt) while negating others through a GPR; clang
// under -Oz routes every case through a GPR before a shared vcvt, and encodes the 0.0
// floor as an immediate rather than a literal-pool load. Both are codegen choices not
// reachable from portable C / NEON intrinsics here.
float Standing::getMissionBonus(unsigned race) {
    Standing *self = this;
    float s0;
    switch (race) {
    case 0:
        s0 = (float)self->standings[0];
        break;
    case 1:
        s0 = (float)(-self->standings[0]);
        break;
    case 2:
        s0 = (float)self->standings[1];
        break;
    case 3:
        s0 = (float)(-self->standings[1]);
        break;
    default:
        return 0.0f;
    }
    float r = s0 / 100.0f;
    return vget_lane_f32(vmax_f32(vdup_n_f32(r), vdup_n_f32(0.0f)), 0);
}

// Standing::applyDisable(int race).
void Standing::applyDisable(int race) {
    Standing *self = this;
    ((Standing *)(self))->applyPoints(race, 2);
}

// Standing::rehabilitate(int race): reset the standing toward a faction's home
// race to a fixed value (-35 for the "even" slot, +35 for the "odd" slot).
void Standing::rehabilitate(int race) {
    Standing *self = this;
    if (race == 0) {
        self->standings[0] = -35;
    } else if (race == 1) {
        self->standings[0] = 0x23;
    } else if (race == 2) {
        self->standings[1] = -35;
    } else if (race == 3) {
        self->standings[1] = 0x23;
    }
}

// Standing::getStandings(): return the raw per-race standings array.
int *Standing::getStandings() {
    return this->standings;
}

// Standing::setStandings(int*): replace the standings array pointer (used when
// restoring a Standing from a saved record).
void Standing::setStandings(int *arr) {
    this->standings = arr;
}

// Standing::create(): heap factory (operator new + Standing::Standing). Used by
// RecordHandler when reconstructing a Standing from a save slot.
Standing *Standing::create() {
    return new Standing();
}

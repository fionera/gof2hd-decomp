#include "gof2/game/world/Standing.h"
#include "gof2/game/mission/Status.h"
#include "gof2/game/world/SolarSystem.h"

// Allocate a 2-int standings array {0x1e, 0}; currentRace = -1.
Standing::Standing() {
    int* p = new int[2];
    p[0] = 0x1e;
    p[1] = 0;
    this->standings = p;
    this->currentRace = -1;
}

// delete[] the standings array, null the pointer.
Standing::~Standing() {
    delete[] this->standings;
    this->standings = nullptr;
}

// Heap factory used by RecordHandler when reconstructing a Standing from a save slot.
Standing* Standing::create() {
    return new Standing();
}

int* Standing::getStandings() {
    return this->standings;
}

// Replace the standings array pointer (used when restoring a Standing from a saved record).
void Standing::setStandings(int* arr) {
    this->standings = arr;
}

// Add delta to standings[race], clamping the result into [-100, 100].
void Standing::applyPoints(int race, int delta) {
    int* p = this->standings;
    int v = delta + p[race];
    p[race] = v;
    if (v > 100) {
        p[race] = 100;
    } else if (v < -100) {
        p[race] = -100;
    }
}

// getStanding(race) normalized to [-1, 1].
float Standing::getStandingRate(int race) {
    return (float)this->getStanding(race) / 100.0f;
}

// True if either home-race standing is outside the non-hostile band, i.e.
// (standing + 0x46) does not fit in [0, 0x8c] as unsigned.
bool Standing::isEnemyWithAnyone() {
    int a = this->standings[0];
    int b = this->standings[1];
    return ((unsigned)(b + 0x46) > 0x8c) || ((unsigned)(a + 0x46) > 0x8c);
}

// Lookup table of "enemy race" per faction index (4 entries).
extern const uint32_t Standing_enemyRaceTable[4];

// idx<4 ? table[idx] : 8.
uint32_t Standing::getEnemyRace(unsigned idx) {
    if (idx < 4)
        return Standing_enemyRaceTable[idx];
    return 8;
}

// Neither enemy nor friend.
unsigned Standing::isNeutral(int race) {
    if (this->isEnemy(race)) return 0;
    return this->isFriend(race) ? 0 : 1;
}

// In "derived" mode (currentRace >= 0) factions 0/1 map to fixed standings based on
// currentRace; otherwise the raw standings[race] value is returned.
int Standing::getStanding(int race) {
    int cr = this->currentRace;
    if (cr >= 0) {
        if (race == 0) {
            if (cr == 0) return 100;
            if (cr == 1) return -100;
            return 0x46;
        }
        if (race == 1) {
            if (cr == 2) return 100;
            if (cr == 3) return -100;
            return 0x46;
        }
    }
    return this->standings[race];
}

// +5 reputation toward the given race.
void Standing::applyMissionCompleted(int race) {
    this->applyPoints(race, -5);
}

void Standing::setStanding(int race, int value) {
    this->standings[race] = value;
}

// Record the player's signature race, switching getStanding() into "derived" mode where
// it returns fixed standings for factions 0/1 keyed off this value instead of the raw array.
void Standing::setPlayerSignatureRace(int race) {
    this->currentRace = race;
}

// -2 reputation toward that race.
void Standing::applyStealCargo(int race) {
    this->applyPoints(race, 2);
}

bool Standing::isEnemy(int race) {
    int cr = this->currentRace;
    if (cr >= 0) {
        if (race != 1) {
            if (race == 3) {
                cr = cr - 2;
            } else if (race == 2) {
                cr = cr - 3;
            } else {
                if (race != 0) return false;
                cr = cr - 1;
            }
        }
        return cr == 0;
    }
    if (race == 1 || race == 0) {
        return this->standings[0] < -0x46;
    }
    if (race == 2) {
        return this->standings[1] < -0x46;
    }
    if (race == 3) {
        return 0x46 < this->standings[1];
    }
    return false;
}

// A committed crime against a race. Severity doubles in hardcore mode; kinds 0/2 are
// "against the even slot", 1/3 the odd slot, with 0/2 applied as a penalty (negated).
void Standing::applyDelict(unsigned kind, int severity) {
    int hc = gStatus->hardCoreMode();
    int delta = severity << hc;
    switch (kind) {
    case 0:
        this->applyPoints(0, -delta);
        return;
    case 1:
        this->applyPoints(0, delta);
        return;
    case 2:
        this->applyPoints(1, -delta);
        return;
    case 3:
        this->applyPoints(1, delta);
        return;
    default:
        return;
    }
}

// "Enemy race" lookup keyed by the current system's race (4 entries).
extern const int g_apk_raceTable[4];

// A kill committed against another ship. For police kills (kind 8) the offence is
// mapped to the local system's owning race (once); otherwise it costs 5 standing
// toward the victim's faction.
void Standing::applyKill(int kind) {
    Status* status = gStatus;
    unsigned sysRace;
    if (status->inAlienOrbit() != 0) {
        sysRace = 9;
    } else {
        sysRace = ((SolarSystem*)(long)status->getSystem())->getRace();
    }
    int delta;
    if (kind == 8) {
        if (this->currentRace >= 0) return;
        delta = 1;
        if (sysRace < 4) {
            kind = g_apk_raceTable[sysRace];
        } else {
            kind = 8;
        }
    } else {
        delta = 5;
    }
    this->applyPoints(kind, delta);
}

bool Standing::isFriend(int race) {
    int cr = this->currentRace;
    if (cr >= 0) {
        if (race == 1) {
            cr = cr - 1;
        } else if (race == 3) {
            cr = cr - 3;
        } else if (race == 2) {
            cr = cr - 2;
        } else if (race != 0) {
            return false;
        }
        return cr == 0;
    }
    if (race == 1 || race == 0) {
        return 0x46 < this->standings[0];
    }
    if (race == 2) {
        return 0x46 < this->standings[1];
    }
    if (race == 3) {
        return this->standings[1] < -0x46;
    }
    return false;
}

// Standing toward the faction, normalized to [0, 1] (negative standings clamp to 0).
// Out-of-range race returns 0.
float Standing::getMissionBonus(unsigned race) {
    float s0;
    switch (race) {
    case 0:
        s0 = (float)this->standings[0];
        break;
    case 1:
        s0 = (float)(-this->standings[0]);
        break;
    case 2:
        s0 = (float)this->standings[1];
        break;
    case 3:
        s0 = (float)(-this->standings[1]);
        break;
    default:
        return 0.0f;
    }
    float r = s0 / 100.0f;
    return r > 0.0f ? r : 0.0f;
}

void Standing::applyDisable(int race) {
    this->applyPoints(race, 2);
}

// Reset the standing toward a faction's home race to a fixed value
// (-35 for the "even" slot, +35 for the "odd" slot).
void Standing::rehabilitate(int race) {
    if (race == 0) {
        this->standings[0] = -35;
    } else if (race == 1) {
        this->standings[0] = 35;
    } else if (race == 2) {
        this->standings[1] = -35;
    } else if (race == 3) {
        this->standings[1] = 35;
    }
}

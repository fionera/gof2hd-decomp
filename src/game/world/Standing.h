#ifndef GOF2_STANDING_H
#define GOF2_STANDING_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

class Standing {
public:
    int *standings; // heap array of per-race standing values
    int currentRace; // player signature race; -1 == use raw standings[]

    Standing();

    ~Standing();

    int *getStandings(); // raw per-race standings array
    void setStandings(int *arr); // replace the standings array pointer

    void applyDelict(int kind, int severity);

    void applyDisable(int race);

    void applyKill(int kind);

    void applyMissionCompleted(int race);

    void applyPoints(int race, int delta);

    void applyStealCargo(int race);

    uint32_t getEnemyRace(int idx);

    float getMissionBonus(int race);

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

#ifndef GOF2_PLAYERCREATURE_H
#define GOF2_PLAYERCREATURE_H

#include "gof2/common.h"
#include "gof2/game/ship/KIPlayer.h"

class Player;
class AEGeometry;

// A creature the player can hook and reel in: a KIPlayer-driven object that fights
// back by enraging (shaking its geometry and draining the player's hitpoints) until
// its endurance runs out and it is either caught or calmed.
class PlayerCreature : public KIPlayer {
public:
    AEGeometry* renderGeometry;
    int         creatureType;
    int         lastElapsed;
    uint16_t    raging;
    uint8_t     hooked;
    uint8_t     caught;
    float       rageScale;
    int         rageTimer;
    int         maxEndurance;
    int         endurance;
    int         lastHitpoints;
    int         itemIndex;
    Matrix      rageMatrix;   // rage-shake rotation matrix

    PlayerCreature(int kind, int itemIndex, Player* player, AEGeometry* geometry,
                   float x, float y, float z);

    uint8_t isHooked();
    void    calmDown();
    void    unhook();
    void    render();
    int     getEndurance();
    int     getWeight();
    uint8_t isCaught();
    void    rage(int amount);
    int     getMaxEndurance();
    int     getItemIndex();
    void    reset();
    void    hook(int value);
    void    update(int elapsed);
};

#endif

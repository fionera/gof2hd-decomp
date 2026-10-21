#ifndef GOF2_TRACTORBEAM_H
#define GOF2_TRACTORBEAM_H
#include "engine/core/Array.h"
#include "../../engine/core/AEString.h"
#include "game/ship/KIPlayer.h"
#include "engine/math/Vector.h"

class Radar;

class AEGeometry;
class Hud;
class KIPlayer;
class Level;

class TractorBeam {
public:
    // Crate position relative to the player (pull direction once normalised).
    Vector direction;
    KIPlayer *grabbedCrate;
    uint8_t active;
    uint8_t soundPlaying;
    AEGeometry *beamGeometry;
    int storedHitpoints;

    TractorBeam(AEGeometry *unused, int kind);

    ~TractorBeam();

    void render();

    void update(int frameTime, Radar *radar, Level *level, Hud *hud);
};

#endif

#ifndef GOF2_TRACTORBEAM_H
#define GOF2_TRACTORBEAM_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

// Galaxy on Fire 2 -- TractorBeam.
// A ship module that locks onto a nearby cargo crate, draws a visible beam from
// the player ship toward it, pulls it in, and captures it once close enough.

class AEGeometry;
class KIPlayer;
class Level;
class Hud;
class Radar;

class TractorBeam {
public:
    float        dirX;            // working direction/offset vector .x
    float        dirY;            // working direction/offset vector .y
    float        dirZ;            // working direction/offset vector .z
    KIPlayer*    grabbedCrate;    // currently grabbed crate (null = none)
    uint8_t      active;          // beam is active
    uint8_t      soundPlaying;    // pull sound is currently playing
    AEGeometry*  beamGeometry;    // beam mesh geometry
    int          storedHitpoints; // grabbed crate's hitpoints snapshot

    // Constructs the beam mesh from base mesh id 0x3798 offset by `kind`.
    TractorBeam(AEGeometry* unused, int kind);
    ~TractorBeam();

    void render();
    void update(int frameTime, Radar* radar, Level* level, Hud* hud);
};

#endif

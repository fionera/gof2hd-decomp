#ifndef GOF2_REPAIRBEAM_H
#define GOF2_REPAIRBEAM_H
#include "common.h"
#include "mathtypes.h"

class AEGeometry;   // ::AEGeometry, engine/render/AEGeometry.h; pointer only

// Repair / heal / shield beam emitter. Maintains a fixed pool of beam geometries
// (sized to the equipped module's target-count attribute) and, on each update,
// assigns nearby enemy targets, animates the beams toward them and applies the
// heal / shield / damage effect for the configured equipment sort.
class RepairBeam {
public:
    int                                 shipIndex;     // sound-event array index
    AbyssEngine::AEMath::Vector         beamPosition;  // current beam endpoint
    Array<AEGeometry*>*                 geometries;    // one beam geometry per target slot
    Array<int>*                         targetIds;     // target enemy slot, -1 = empty
    Array<float>*                       charges;       // per-target charge accumulator
    int                                 sort;          // equipment sort (0x25 heal / 0x29 shield)
    int                                 timer;         // re-arm countdown timer

    RepairBeam(int shipIndex, int sort);
    ~RepairBeam();

    void render();
    void update(int dt, void* level, void* hud);

    // Allocate and construct a RepairBeam for the given ship index and equipment
    // sort (0x25 heal / 0x29 shield). Used by PlayerEgo when a repair-beam module
    // is equipped.
    static RepairBeam* create(int shipIndex, int sort);
};
#endif

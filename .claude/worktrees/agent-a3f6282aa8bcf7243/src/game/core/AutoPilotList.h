#ifndef GOF2_AUTOPILOTLIST_H
#define GOF2_AUTOPILOTLIST_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
// String (the 12-byte by-value AbyssEngine::String) and the SolarSystem class are sourced
// from a single header to avoid the duplicate-String ODR clash that arises when
// SolarSystem.h and Station.h are pulled into one TU. getTargetString() returns String by
// value, so its full definition must be in scope here.
#include "game/world/SolarSystem.h"

class Level;

// On-screen autopilot destination picker: a small modal list of warp targets (current
// station, warp gate, mission target, "cancel", route waypoint) the player can step
// through with up()/down() and confirm via touch().
class AutoPilotList {
public:
    int selected;                 // currently highlighted row
    int x;                        // window left
    int y;                        // window top
    int width;                    // window width (widest entry + padding)
    Array<String*>* entries;      // owned list of destination labels (released in dtor)
    int count;                    // number of non-empty entries

    AutoPilotList(Level* level);
    ~AutoPilotList();

    void draw();
    void up();
    void down();
    String getTargetString();
    int touch(int px, int py);
    int fire();                   // returns the highlighted row index (DeepOpen: getSelection())
};
#endif

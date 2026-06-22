#ifndef GOF2_AUTOPILOTLIST_H
#define GOF2_AUTOPILOTLIST_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

#include "game/world/SolarSystem.h"

class Level;

class AutoPilotList {
public:
    int selected; // currently highlighted row
    int x; // window left
    int y; // window top
    int width; // window width (widest entry + padding)
    Array<String *> *entries; // owned list of destination labels (released in dtor)
    int count; // number of non-empty entries

    AutoPilotList(Level *level);

    ~AutoPilotList();

    void draw();

    void up();

    void down();

    String getTargetString();

    int touch(int px, int py);

    int fire(); // returns the highlighted row index (DeepOpen: getSelection())
};
#endif

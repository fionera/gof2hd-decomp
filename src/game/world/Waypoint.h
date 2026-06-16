#ifndef GOF2_WAYPOINT_H
#define GOF2_WAYPOINT_H

#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "game/ship/KIPlayer.h"

struct Route;

// A single navigation node on a Route. A Waypoint is a stationary KIPlayer whose
// underlying Player marks the target position the route steers the ship toward.
class Waypoint : public KIPlayer {
public:
    int32_t x;
    int32_t y;
    int32_t z;
    uint16_t state;
    Route* route;

    Waypoint(int x, int y, int z, Route* route);

    void setActive(bool active);
    void reached();
    void activate();
    float advance(bool active);
    Vector getPosition();
    void reset();
};

#endif

#ifndef GOF2_ROUTE_H
#define GOF2_ROUTE_H
#include "gof2/common.h"
// Galaxy on Fire 2 - Route class (waypoint navigation path).
// Top-level class (no AbyssEngine namespace). Real named struct; field_0xNN
// member names are kept so cross-class references resolve. No byte-offset access.
//
// Object layout (recovered):
//   +0x00  int                   current waypoint index
//   +0x04  uint8_t               looping flag
//   +0x0c  Array<Waypoint*>*     waypoints
//   +0x10  Array<KIPlayer*>*     docking targets
//   +0x14  Array<int>*           docking times

struct Route;
struct Waypoint;
struct KIPlayer;

// AbyssEngine::AEMath::Vector - 3 floats. Passed by const ref / value.
typedef AbyssEngine::AEMath::Vector Vector;

class Route {
public:
    int32_t              currentIndex;   // +0x0  current waypoint index
    uint8_t              loop;           // +0x4  looping flag
    Array<Waypoint *>   *waypoints;      // +0xc  waypoints
    Array<KIPlayer *>   *dockingTargets; // +0x10 docking targets
    Array<int>          *dockingTimes;   // +0x14 docking times

    // ---- constructors / destructor (demangle to Route::Route / ~Route) ----
    Route(int *coords, int count);
    Route(int *coords, Array<KIPlayer *> *targets, int *times, int count);
    ~Route();
    int getCurrent();
    void setLoop(bool loop);
    Route * clone();
    KIPlayer * getDockingTarget();
    KIPlayer * getDockingTarget_i(int index);
    int getDockingTime();
    int getDockingTimeAt(int index);
    Route * getExactClone();
    Waypoint * getLastWaypoint();
    Waypoint * getWaypoint();
    Waypoint * getWaypointAt(int index);
    int length();
    void reachWaypoint(int index);
    void reset();
    void setNewCoords(float x, float y, float z);
    void translate(const Vector &v);
    void update(const Vector &v);
    float update_xyz(float x, float y, float z);
    bool waypointDefined();
};

#endif

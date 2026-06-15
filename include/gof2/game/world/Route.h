#ifndef GOF2_ROUTE_H
#define GOF2_ROUTE_H
#include "gof2/common.h"

// Galaxy on Fire 2 - Route: an ordered waypoint navigation path with optional
// docking targets/times, looping support, and an active-waypoint cursor.

struct Waypoint;
struct KIPlayer;

typedef AbyssEngine::AEMath::Vector Vector;

class Route {
public:
    int32_t              currentIndex;
    uint8_t              loop;
    Array<Waypoint *>   *waypoints;
    Array<KIPlayer *>   *dockingTargets;
    Array<int>          *dockingTimes;

    Route(int *coords, int count);
    Route(int *coords, Array<KIPlayer *> *targets, int *times, int count);
    ~Route();

    int getCurrent();
    void setLoop(bool loop);
    Route *clone();
    KIPlayer *getDockingTarget();
    KIPlayer *getDockingTarget_i(int index);
    int getDockingTime();
    int getDockingTimeAt(int index);
    Route *getExactClone();
    Waypoint *getLastWaypoint();
    Waypoint *getWaypoint();
    Waypoint *getWaypointAt(int index);
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

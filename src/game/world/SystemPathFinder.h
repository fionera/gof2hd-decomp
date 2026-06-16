#ifndef GOF2_SYSTEMPATHFINDER_H
#define GOF2_SYSTEMPATHFINDER_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "engine/core/Node.h"
#include "game/world/SolarSystem.h"
#include "game/mission/Status.h"

// The persistent player/game state used by getSystemPath() to query which
// systems are currently visible. Resolved through the engine's global Status
// singleton holder.
extern "C" __attribute__((visibility("hidden"))) Status **volatile
    g_SystemPathFinder_status;

// Galaxy on Fire 2 -- breadth-first/A* search over the galaxy jump graph.
//
// SystemPathFinder is stateless: every search() builds and tears down its
// open/closed node sets locally, so both the constructor and destructor are
// trivial.
class SystemPathFinder {
public:
    SystemPathFinder();
    ~SystemPathFinder();

    int contains(Array<Node *> *nodes, Node *node);
    Array<Node *> *search(Node *start, Node *goal);
    int getJumpDistance(Array<SolarSystem *> *systems, int from, int to);
    Array<Node *> *constructPath(Node *node);
    Array<int> *getSystemPath(Array<SolarSystem *> *systems, int from, int to);
};
#endif

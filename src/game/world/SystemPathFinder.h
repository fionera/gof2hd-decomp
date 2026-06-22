#ifndef GOF2_SYSTEMPATHFINDER_H
#define GOF2_SYSTEMPATHFINDER_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "engine/core/Node.h"
#include "game/world/SolarSystem.h"
#include "game/mission/Status.h"

extern "C" __attribute__ ((visibility
(
"hidden"
)
)
)
Status **volatile
g_SystemPathFinder_status;

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

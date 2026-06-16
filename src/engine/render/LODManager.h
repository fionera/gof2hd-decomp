#ifndef GOF2_LODMANAGER_H
#define GOF2_LODMANAGER_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "mathtypes.h"

class AEGeometry;   // global engine type, used by pointer only

// Galaxy on Fire 2 -- LODManager.
// Tracks the active camera position and periodically updates the level-of-detail
// of every registered renderable geometry.
class LODManager {
public:
    Array<AEGeometry*>* objects;
    AEMath::Vector      cameraPos;
    int                 timer;

    LODManager();
    ~LODManager();

    void addObject(AEGeometry *g);
    void removeObject(AEGeometry *g);
    void forceUpdate(int dt, bool useParent);
    void update(int dt);
};
#endif

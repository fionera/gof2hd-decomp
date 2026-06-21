#ifndef GOF2_LODMANAGER_H
#define GOF2_LODMANAGER_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "mathtypes.h"
#include "engine/render/AEGeometry.h"   // Array<AEGeometry*> objects + addObject/removeObject

// A single live touch point, as tracked by the Android input bridge: screen x/y plus
// the pointer id and action code. GetTouch() returns one of these by value.
struct Touch {
    int x;
    int y;
    int id;
    int action;
};

// Android touch-input bridge. The current/maximum touch counts and the touch buffer
// pointer live in .bss (curTouchSize / maxTouchSize / touches), populated by the JNI
// layer; these free functions are the engine-side accessors.
void AddTouch(int x, int y, int id, int action);
int  GetTouchCount();
void RemoveTouches();
Touch GetTouch(int index);

// Free the five native item-information string lists (five groups of five list ids on
// the Globals singleton) and null their slots. Called when the item database is rebuilt.
// C-linkage JNI-style bridge entry point (the "ndk_" prefix marks it as such).
extern "C" void ndk_resetNativeItemInformationList();

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

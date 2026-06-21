#ifndef GOF2_LODMANAGER_H
#define GOF2_LODMANAGER_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "mathtypes.h"

class AEGeometry;   // global engine type, used by pointer only

// Erase every element equal to `item` from `a` (in-place compaction), then shrink the
// backing store to the new count (min one slot). The engine's out-of-line container
// helper: _Z11ArrayRemoveI...EvT_R5ArrayIS2_E in the binary, paired with ArrayAdd<T>.
template <class T>
void ArrayRemove(T item, Array<T> &a)
{
    unsigned int write = 0;
    for (unsigned int read = 0; read < a.size_; ++read) {
        T cur = a.data_[read];
        if (cur != item)
            a.data_[write++] = cur;
    }
    a.size_ = write;
    unsigned int cap = write ? write : 1;
    a.capacity_ = cap;
    a.data_ = static_cast<T *>(realloc(a.data_, cap * sizeof(T)));
}

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

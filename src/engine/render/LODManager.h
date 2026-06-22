#ifndef GOF2_LODMANAGER_H
#define GOF2_LODMANAGER_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "mathtypes.h"
#include "engine/render/AEGeometry.h"   // Array<AEGeometry*> objects + addObject/removeObject

struct Touch {
    int x;
    int y;
    int id;
    int action;
};

void AddTouch(int x, int y, int id, int action);

int GetTouchCount();

void RemoveTouches();

Touch GetTouch(int index);

extern "C" void ndk_resetNativeItemInformationList();

class LODManager {
public:
    Array<AEGeometry *> *objects;
    AEMath::Vector cameraPos;
    int timer;

    LODManager();

    ~LODManager();

    void addObject(AEGeometry *g);

    void removeObject(AEGeometry *g);

    void forceUpdate(int dt, bool useParent);

    void update(int dt);
};
#endif

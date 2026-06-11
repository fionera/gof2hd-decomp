#ifndef GOF2_LODMANAGER_H
#define GOF2_LODMANAGER_H
#include "gof2/common.h"
#include "gof2/math.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 — LODManager. Top-level, no namespace.
// Field offsets recovered from per-method target disassembly.
//
// Known fields:
//   +0x00  Array<AEGeometry*>* objects   (length at *self, data at self[1])
//   +0x04  Vector              cameraPos (3 floats: 0x04,0x08,0x0c)
//   +0x10  int                 timer     (init 0x3e9)


struct AEGeometry;


struct LODManager {
    Array<AEGeometry*>* objects;        // +0x0
    AEMath::Vector cameraPos;           // +0x4 (field_0x4/0x8/0xc)
    int timer;                     // +0x10 (timer)

    LODManager();
    ~LODManager();
    void addObject(AEGeometry *g);
    void removeObject(AEGeometry *g);
    void forceUpdate(int dt, bool useParent);
    void update(int dt);
};
#endif

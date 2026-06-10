#ifndef GOF2_LODMANAGER_H
#define GOF2_LODMANAGER_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 — LODManager. Top-level, no namespace.
// Field offsets recovered from per-method target disassembly.
//
// Known fields:
//   +0x00  Array<AEGeometry*>* objects   (length at *self, data at self[1])
//   +0x04  Vector              cameraPos (3 floats: 0x04,0x08,0x0c)
//   +0x10  int                 timer     (init 0x3e9)


struct LODManager;
struct AEGeometry;
struct Matrix;



// Field accessor via byte offset.

struct LODManager {
    int field_0x4;                      // +0x4
    int field_0x8;                      // +0x8
    int field_0xc;                      // +0xc
    int field_0x10;                     // +0x10
};
#endif

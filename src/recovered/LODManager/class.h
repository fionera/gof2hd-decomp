#ifndef WORK_CLASSES_LODMANAGER_SRC_CLASS_H
#define WORK_CLASSES_LODMANAGER_SRC_CLASS_H

// Galaxy on Fire 2 — LODManager. Top-level, no namespace.
// Field offsets recovered from per-method target disassembly.
//
// Known fields:
//   +0x00  Array<AEGeometry*>* objects   (length at *self, data at self[1])
//   +0x04  Vector              cameraPos (3 floats: 0x04,0x08,0x0c)
//   +0x10  int                 timer     (init 0x3e9)

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

struct LODManager;
struct AEGeometry;
struct Matrix;

struct Vector {
    float x;
    float y;
    float z;
};

// Field accessor via byte offset.
template <class T>
static inline T &F(void *self, unsigned off) { return *(T *)((char *)self + off); }

#endif

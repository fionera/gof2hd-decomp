#ifndef WORK_CLASSES_AMESHMERGER_SRC_CLASS_H
#define WORK_CLASSES_AMESHMERGER_SRC_CLASS_H

// Galaxy on Fire 2 -- AMeshMerger class (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class. Owns an Array<AbyssEngine::Mesh*> embedded at +0x08.
// render() tail-calls an engine draw routine through a PC-relative global with
//   (this->field_14, this->field_1c, 0).
// Field offsets recovered from the per-method target disassembly; accessed via
// byte-offset casts from `this`.
//
//   0x08  Array<Mesh*> meshes (embedded; ~Array called in dtor)
//   0x14  uint32_t     field_14 (drawn arg 0)
//   0x1c  uint32_t     field_1c (drawn arg 1)

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

extern "C" {
// AbyssEngine::Array<AbyssEngine::Mesh*>::~Array(this) -- 0x0006f760.
void AMeshMerger_ArrayMesh_dtor(void *self);
// Engine draw routine reached via the PC-relative global at 0x1abda8 (b.w 0x1abd98).
void AMeshMerger_drawMeshes(uint32_t a, uint32_t b, uint32_t c);
}

// Field accessors via byte offset.
static inline uint32_t &u32(void *self, uint32_t off) { return *(uint32_t *)((char *)self + off); }
static inline int32_t &i32(void *self, uint32_t off) { return *(int32_t *)((char *)self + off); }
static inline void *&pp(void *self, uint32_t off) { return *(void **)((char *)self + off); }

#endif

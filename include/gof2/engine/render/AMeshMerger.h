#ifndef GOF2_AMESHMERGER_H
#define GOF2_AMESHMERGER_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// @portable-fields

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

// Engine Mesh is opaque/namespaced; a pointer to it is all the embedded Array needs.
namespace AbyssEngine { class Mesh; }

extern "C" {
// Engine draw routine reached via the PC-relative global at 0x1abda8 (b.w 0x1abd98).
void AMeshMerger_drawMeshes(uint32_t a, uint32_t b, uint32_t c);
}

class AMeshMerger {
public:
    unsigned char _pad_0[8];
    // +0x08  embedded Array<Mesh*>; auto-destructs via ~AMeshMerger().
    Array<AbyssEngine::Mesh*> meshes;
    unsigned char _pad_10[4];
    uint32_t field_0x14;   // +0x14  (drawMeshes arg 0)
    unsigned char _pad_18[4];
    uint32_t field_0x1c;   // +0x1c  (drawMeshes arg 1)
    void render();
    ~AMeshMerger();
};
#endif

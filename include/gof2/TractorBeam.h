#ifndef GOF2_TRACTORBEAM_H
#define GOF2_TRACTORBEAM_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 -- TractorBeam (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class (no AbyssEngine namespace), per each work-item Sig line.
// Field offsets recovered per-method from the target disassembly; access fields
// via byte-offset casts from `this`.
//
// Object layout (recovered from ctor / update):
//   +0x00  Vector      working direction/offset vector (3 floats)
//   +0x0c  KIPlayer*   currently grabbed crate object (0 = none)
//   +0x10  uint8_t     active flag
//   +0x11  uint8_t     sound-playing flag
//   +0x14  AEGeometry* beam mesh geometry
//   +0x18  int         stored hitpoints snapshot


struct TractorBeam;
struct AEGeometry;
struct KIPlayer;
struct PlayerEgo;
struct Radar;
struct Level;
struct Hud;
struct PaintCanvas;

void *operator new(__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;


namespace AbyssEngine {
namespace AEMath {



// AEMath free functions (resolved blx targets).
void VectorNormalize(void *out, const Vector *v);   // 0x0006ec80
float VectorLength(const Vector *v);                // 0x0006ec44
Vector operator-(const Vector &a, const Vector &b); // 0x0006ec38

} // namespace AEMath

    // ---- methods (converted from free functions) ----
    void ctor(AEGeometry * /*unused*/, int param2);
    void render();
    void update(int frameTime, Radar *radar, Level *level, Hud *hud);
} // namespace AbyssEngine

using AbyssEngine::AEMath::Vector;

class TractorBeam {
public:
    float dirX;                    // +0x00  working vector .x
    float dirY;                    // +0x04  working vector .y
    float dirZ;                    // +0x08  working vector .z
    KIPlayer* grabbedCrate;                // +0x0c  grabbed crate (0 = none)
    uint8_t active;                 // +0x10  active flag
    uint8_t soundPlaying;                 // +0x11  sound-playing flag
    uint8_t _pad_0x12[2];               // +0x12  padding
    AEGeometry* beamGeometry;             // +0x14  beam mesh geometry
    int storedHitpoints;                     // +0x18  stored hitpoints snapshot

    // ---- methods (converted from free functions) ----
    void ctor(AEGeometry * /*unused*/, int param2);
    void render();
    void update(int frameTime, Radar *radar, Level *level, Hud *hud);
};
#endif

#ifndef GOF2_PLAYERSTATIC_H
#define GOF2_PLAYERSTATIC_H
#include "gof2/common.h"
// Galaxy on Fire 2 - PlayerStatic (a stationary KIPlayer subclass).
// Derives from KIPlayer; only a few fields are touched directly:
//   +0x00 vtable pointer
//   +0x08 AEGeometry* geometry
//   +0x124/0x128/0x12c integer x/y/z fallback position
namespace AbyssEngine {
namespace AEMath {
} // namespace AEMath
} // namespace AbyssEngine

using Vector = AbyssEngine::AEMath::Vector;

struct AEGeometry;

class PlayerStatic {
public:
    void *field_0x0;                    // +0x0 vtable
    char field_pad_4[0x4];              // +0x4
    AEGeometry *field_0x8;              // +0x8 geometry
    char field_pad_c[0x124 - 0xc];      // +0xc .. +0x123 (KIPlayer/Player state)
    int field_0x124;                    // +0x124 x
    int field_0x128;                    // +0x128 y
    int field_0x12c;                    // +0x12c z

    PlayerStatic(int playerId, AEGeometry *geometry, float x, float y, float z);
    ~PlayerStatic();
    void render();
    Vector getPosition();
};

#endif

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
    PlayerStatic(int playerId, AEGeometry *geometry);
    ~PlayerStatic();
    void render();
    Vector getPosition();

    // In-place construction body shared by the PlayerStaticFar constructor.
    void ctor(int playerId, AEGeometry *geometry, float x, float y, float z);

    // Decompiler fragments of render()/the destructor chain:
    //   render_geometry  -> renders the owned AEGeometry (tail of render())
    //   dtor_tail        -> operator delete tail of the deleting destructor
    //   base_dtor        -> PlayerStatic base destructor (chains KIPlayer)
    //   base_dtor_thunk  -> b.w veneer that tail-calls base_dtor
    void render_geometry(void *geometry);
    void *dtor_tail();
    void *base_dtor();
    void *base_dtor_thunk();
};

#endif

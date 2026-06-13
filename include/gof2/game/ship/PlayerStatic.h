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
    void *vtable;                       // +0x0  vtable pointer
    char field_pad_4[0x4];              // +0x4
    AEGeometry *geometry;               // +0x8  owned render geometry
    char field_pad_c[0x124 - 0xc];      // +0xc .. +0x123 (KIPlayer/Player state)
    int32_t posX;                       // +0x124 fallback x position
    int32_t posY;                       // +0x128 fallback y position
    int32_t posZ;                       // +0x12c fallback z position

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

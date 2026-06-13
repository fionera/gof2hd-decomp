#ifndef GOF2_PLAYERJUMPGATE_H
#define GOF2_PLAYERJUMPGATE_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 -- PlayerJumpgate class. Extends PlayerStaticFar (0x140-byte
// base subobject); only the few fields the recovered methods touch are named, the
// rest of the inherited base layout is preserved as padding so the byte offsets
// stay correct. Trailing comments are the original 32-bit field offsets, kept for
// cross-reference; the live layout is natural 64-bit.
struct AEGeometry;
struct BoundingVolume;
struct BoundingSphere;
struct PlayerJumpgate;
struct PlayerStaticFar;

class PlayerJumpgate {
public:
    // ---- inherited PlayerStaticFar base subobject (named where touched) ----
    void* vtable;                        // +0x0    vtable pointer
    void* player;                        // +0x4    Player*
    AEGeometry* geometry;                // +0x8    owned geometry
    byte  _pad_0xc[0xf5-0xc];            // +0xc
    uint8_t visible;                     // +0xf5   visible / update-active flag
    byte  _pad_0xf6[0x124-0xf6];         // +0xf6
    int32_t posX;                        // +0x124  integer position x
    int32_t posY;                        // +0x128  integer position y
    int32_t posZ;                        // +0x12c  integer position z
    Array<BoundingVolume*>* boundingVolumes; // +0x130 bounding-volume container (inherited)
    byte  _pad_0x134[0x140-0x134];       // +0x134
    // ---- PlayerJumpgate own fields ----
    uint8_t activated;                   // +0x140  jump animation activated flag
    uint32_t transformHandle;            // +0x144  animation transform handle (0xffffffff = none)

    PlayerJumpgate(int playerId, AEGeometry *geometry, float x, float y, float z, bool visible);
    ~PlayerJumpgate();
    void *operatorDeleteTail();         // deleting-dtor tail: operator delete(this)
    bool timeToJump();
    void activate();
    void addJumpAnimationHandle(uint32_t handle);
    bool animationEnded();
    void update(int delta);
    void setPosition(float x, float y, float z);
};
#endif

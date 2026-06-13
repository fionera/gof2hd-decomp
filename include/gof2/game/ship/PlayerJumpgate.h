#ifndef GOF2_PLAYERJUMPGATE_H
#define GOF2_PLAYERJUMPGATE_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
struct AEGeometry;
struct BoundingVolume;
struct BoundingSphere;
struct PlayerJumpgate;
struct PlayerStaticFar;

class PlayerJumpgate {
public:
    void* field_0x0;                    // +0x0
    void* field_0x4;                    // +0x4
    AEGeometry* field_0x8;              // +0x8
    uint8_t field_0xf5;                 // +0xf5
    int32_t field_0x124;                // +0x124
    int32_t field_0x128;                // +0x128
    int32_t field_0x12c;                // +0x12c
    Array<BoundingVolume*>* field_0x130; // +0x130
    uint8_t field_0x140;                // +0x140
    uint32_t field_0x144;               // +0x144

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

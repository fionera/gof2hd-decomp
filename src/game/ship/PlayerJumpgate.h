#ifndef GOF2_PLAYERJUMPGATE_H
#define GOF2_PLAYERJUMPGATE_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "game/ship/PlayerStaticFar.h"

class AEGeometry;

class PlayerJumpgate : public PlayerStaticFar {
public:
    // Whether the jump animation has been triggered.
    uint8_t activated;
    // Animation transform handle (0xffffffff = none assigned yet).
    uint32_t transformHandle;

    PlayerJumpgate(int playerId, AEGeometry *geometry, float x, float y, float z, bool visible);

    ~PlayerJumpgate();

    bool timeToJump();

    void activate();

    void addJumpAnimationHandle(uint32_t handle);

    bool animationEnded();

    void update(int delta) override;

    void setPosition(float x, float y, float z) override; // actor vtable slot +0x48
};
#endif

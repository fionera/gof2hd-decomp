#ifndef GOF2_PLAYERSTATICFAR_H
#define GOF2_PLAYERSTATICFAR_H

#include <cstdint>
#include <vector>

#include "engine/core/Array.h"
#include "../../engine/core/AEString.h"
#include "game/ship/PlayerStatic.h"

#include "engine/math/Vector.h"

class Player;

class AEGeometry;
class BoundingVolume;

class PlayerStaticFar : public PlayerStatic {
public:
    // ASM ground truth: player@4, initPosition@0x58, cameraPosition@0x90 and objectPosition@0x9c are
    // all KIPlayer base fields (our decomp duplicated them, adding 0x28=40 bytes and shifting
    // boundingVolumes/viewDirection). `player` is just inherited; the three position vectors alias the
    // KIPlayer fields at their real offsets. boundingVolumes now lands @0x130, viewDirection @0x134.
    Array<BoundingVolume *> *boundingVolumes;
    Vector viewDirection;

    Vector &initPosition() { return reinterpret_cast<Vector &>(this->posX); }
    Vector &cameraPosition() { return reinterpret_cast<Vector &>(this->field_0x90); }
    Vector &objectPosition() { return reinterpret_cast<Vector &>(this->field_0x9c); }

    PlayerStaticFar(int playerId, AEGeometry *geometry, float x, float y, float z);

    ~PlayerStaticFar();

    Vector getProjectionVector(const Vector &value) override;

    void render() override;

    Vector projectCollisionOnSurface(const Vector &value) override;

    int outerCollide(float x, float y, float z) override;

    int outerCollide(Vector value);

    Vector getInitPosition(Vector value);

    void setYRotation(int yRotation);

    void update(int delta) override;

    int collide(float x, float y, float z) override;
};

#endif

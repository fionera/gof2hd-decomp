#ifndef GOF2_BEAMGUN_H
#define GOF2_BEAMGUN_H
#include <cstdint>
#include "engine/core/Array.h"
#include "../../engine/core/AEString.h"
#include "Gun.h"
#include "game/world/Level.h"

#include "engine/math/Vector.h"

class Player;

class AEGeometry;
class Gun;
class Level;


class BeamGun {
public:
    int32_t field_0x4;
    Gun *gun;
    Level *level;
    int32_t owner;
    int32_t meshKind;
    AEGeometry *primaryGeometry;
    AEGeometry *secondaryGeometry;
    uint8_t hasSecondary;
    uint8_t secondaryVisible;

    BeamGun(int owner, Gun *gun, int meshKind, Level *level);

    virtual ~BeamGun();

    virtual void setEnemies(Array<Player *> *enemies);

    virtual void setEnemy(Player *enemy);

    virtual void update(int elapsed);

    virtual void render();

    void replaceGun(unsigned int mesh, int unused);

    void translate(const Vector &v);
};
#endif

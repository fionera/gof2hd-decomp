#ifndef GOF2_ABSTRACTGUN_H
#define GOF2_ABSTRACTGUN_H

#include "engine/core/Array.h"
#include "engine/math/Vector.h"

class Player;

class AbstractGun {
public:
    // Virtual methods in EXACT original AbstractGun vtable order (verified via _ZTV ABS32 relocs):
    // setEnemies, setEnemy, update, render, translate, replaceGun precede the is* predicates.
    virtual ~AbstractGun() {}

    virtual void setEnemies(Array<Player *> *enemies) = 0;

    virtual void setEnemy(Player *enemy) = 0;

    virtual void update(int dt) = 0;

    virtual void render() = 0;

    virtual void translate(const AbyssEngine::AEMath::Vector &v) = 0;

    virtual void replaceGun(unsigned int mesh, int unused) = 0;

    virtual int isRocketGun() { return 0; }

    virtual int isBombGun() { return 0; }

    virtual int isMineGun() { return 0; }

    virtual int isAutoTurret() { return 0; }
};
#endif

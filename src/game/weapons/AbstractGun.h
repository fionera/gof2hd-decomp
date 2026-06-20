#ifndef GOF2_ABSTRACTGUN_H
#define GOF2_ABSTRACTGUN_H

// Galaxy on Fire 2 -- AbstractGun: the polymorphic base of the renderable gun
// hierarchy (ObjectGun, RocketGun, MineGun, BombGun, SentryGun, ...). It declares
// the weapon-kind predicate quartet as virtuals that all answer "no" by default;
// each concrete gun overrides the single predicate that describes it (RocketGun
// answers isRocketGun, BombGun answers isBombGun, MineGun answers isMineGun, and
// the auto-turret answers isAutoTurret). Top-level class (no namespace).
class AbstractGun {
public:
    // (the +0x0 vptr is the compiler-managed C++ vtable; the class is polymorphic)

    virtual int isRocketGun();
    virtual int isBombGun();
    virtual int isMineGun();
    virtual int isAutoTurret();
};
#endif
